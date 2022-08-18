#include "kernel/drivers/i8042.h"

DOE_STATUS Drv8042Initialize()
{
    // Disable both PS/2 ports
    Drv8042DisablePorts(0);

    // Empty the buffer by recieving data
    Drv8042ReadData();

    PS2_CONFIGURATION_BYTE Config = Drv8042ReadConfigurationByte();
    BOOL IsDualChannel = 0;

    // Configure new config info
    Config.Port1Interrupt = 0;
    Config.Port2Interrupt = 0;
    Config.Port1Translation = 0;

    IsDualChannel = !Config.Port2Clock; // Shit is fucked in intel land

    // Write the new config byte
    Drv8042WriteConfigurationByte(Config);

    // Run self test to ensure shit works
    if (Drv8042RunSelfTest() == FALSE)
    {
        return DOE_ERROR;
    }

    // Apparently on some hardware, self test wipes the config byte.
    // Let's restore it.
    Drv8042WriteConfigurationByte(Config);

    if (IsDualChannel)
    {
        Drv8042EnablePorts(2);

        Config = Drv8042ReadConfigurationByte();

        IsDualChannel = !Config.Port2Clock; // Shit is fucked in intel land

        if (IsDualChannel)
        {
            Drv8042DisablePorts(2);
        }
    }

    BOOL PortOneOk = FALSE, PortTwoOk = FALSE;
    
    PortOneOk = Drv8042RunPortTest(1);

    if (IsDualChannel)
        PortTwoOk = Drv8042RunPortTest(2);

    if (PortOneOk == FALSE && PortTwoOk == FALSE)
        return DOE_ERROR;

    if (PortOneOk)
        Drv8042EnablePorts(1);
    if (PortTwoOk)
        Drv8042EnablePorts(2);
    
    Config = Drv8042ReadConfigurationByte();

    Config.Port1Interrupt = PortOneOk != FALSE;
    Config.Port2Interrupt = PortTwoOk != FALSE;

    Drv8042WriteConfigurationByte(Config);

    if (PortOneOk)
        PortOneOk = Drv8042ResetDevice(1);
    if (PortTwoOk)
        PortTwoOk = Drv8042ResetDevice(2);

    if (PortOneOk == FALSE && PortTwoOk == FALSE)
        return DOE_ERROR;

    return DOE_SUCCESS;
}

BOOL Drv8042ResetDevice(BYTE Port)
{
    BYTE Status = 0;
    
    switch (Port)
    {
        case 1:
        {
            Drv8042WriteData(0xFF);

            Drv8042PollOutputBufferStatus(0);

            Status = Drv8042ReadData();

            break;
        }
        case 2:
        {
            KeHalPortWrite(PS2_COMMAND_PORT, PS2_CMD_SEND_TO_PORT2);

            Drv8042WriteData(0xFF);

            Drv8042PollOutputBufferStatus(0);

            Status = Drv8042ReadData();

            break;
        }
        default:
        {
            return FALSE;
        }
    }

    // TODO: Make this a define
    if (Status != 0xFA)
        return FALSE;
    
    return TRUE;
}

INLINE BYTE Drv8042ReadData()
{
    return KeHalPortRead(PS2_DATA_PORT);
}

INLINE VOID Drv8042WriteData(BYTE Data)
{
    KeHalPortWrite(PS2_DATA_PORT, Data);
}

INLINE PS2_STATUS_REGISTER Drv8042ReadStatusRegister()
{
    return (PS2_STATUS_REGISTER)KeHalPortRead(PS2_STATUS_PORT);
}

INLINE PS2_CONFIGURATION_BYTE Drv8042ReadConfigurationByte()
{
    return (PS2_CONFIGURATION_BYTE)Drv8042SendCommandAndRecvResponse(PS2_CMD_READ_CONFIG_BYTE);
}

INLINE VOID Drv8042WriteConfigurationByte(PS2_CONFIGURATION_BYTE Config)
{
    Drv8042SendTwoByteCommand(PS2_CMD_WRITE_CONFIG_BYTE, Config.Data);
}

VOID Drv8042PollOutputBufferStatus(BYTE Check)
{
    PS2_STATUS_REGISTER Status = { 0 };

    do
    {
        Status = Drv8042ReadStatusRegister();
    } while (Status.OutputStatus == Check);
}

VOID Drv8042PollInputBufferStatus(BYTE Check)
{
    PS2_STATUS_REGISTER Status = { 0 };

    do
    {
        Status = Drv8042ReadStatusRegister();
    } while (Status.InputStatus == Check);
}

BYTE Drv8042SendCommandAndRecvResponse(BYTE Cmd)
{
    KeHalPortWrite(PS2_COMMAND_PORT, Cmd);

    // Wait for output buffer
    Drv8042PollOutputBufferStatus(0);

    return Drv8042ReadData();
}

VOID Drv8042SendTwoByteCommand(BYTE Cmd, BYTE Byte2)
{
    KeHalPortWrite(PS2_COMMAND_PORT, PS2_CMD_WRITE_CONFIG_BYTE);

    // Wait for input buffer to be empty
    Drv8042PollInputBufferStatus(1);

    Drv8042WriteData(Byte2);
}

INLINE BOOL Drv8042RunSelfTest()
{
    BOOL Result = Drv8042SendCommandAndRecvResponse(PS2_CMD_TEST_CONTROLLER);

    return Result == PS2_RESP_SELF_TEST_SUCCESS ? 1 : 0;
}

BOOL Drv8042RunPortTest(BYTE Port)
{
    BYTE Result = 0;

    switch (Port)
    {
        case 1:
        {
            Result = Drv8042SendCommandAndRecvResponse(PS2_CMD_TEST_PORT1);
            break;            
        }
        case 2:
        {
            Result = Drv8042SendCommandAndRecvResponse(PS2_CMD_TEST_PORT2);
            break;  
        }
        default:
        {
            return FALSE;
        }
    }

    if (Result != PS2_RESP_PORT_TEST_SUCCESS)
        return FALSE;
    
    return TRUE;
}

VOID Drv8042DisablePorts(BYTE Port)
{   
    switch (Port)
    {
        case 0:
        {
            KeHalPortWrite(PS2_COMMAND_PORT, PS2_CMD_DISABLE_PORT1);
            KeHalPortWrite(PS2_COMMAND_PORT, PS2_CMD_DISABLE_PORT2);
            break;
        }
        case 1:
        {
            KeHalPortWrite(PS2_COMMAND_PORT, PS2_CMD_DISABLE_PORT1);
            break;
        }
        case 2:
        {
            KeHalPortWrite(PS2_COMMAND_PORT, PS2_CMD_DISABLE_PORT2);
            break;
        }
    }
}

VOID Drv8042EnablePorts(BYTE Port)
{   
    switch (Port)
    {
        case 0:
        {
            KeHalPortWrite(PS2_COMMAND_PORT, PS2_CMD_ENABLE_PORT1);
            KeHalPortWrite(PS2_COMMAND_PORT, PS2_CMD_ENABLE_PORT2);
            break;
        }
        case 1:
        {
            KeHalPortWrite(PS2_COMMAND_PORT, PS2_CMD_ENABLE_PORT1);
            break;
        }
        case 2:
        {
            KeHalPortWrite(PS2_COMMAND_PORT, PS2_CMD_ENABLE_PORT2);
            break;
        }
    }
}