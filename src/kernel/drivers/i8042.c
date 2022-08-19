#include "kernel/drivers/i8042.h"

const WORD g_Drv8042KbdSet2Keymap[KBD_KEYMAP_SIZE] = 
{
    0, 67, 65, 63, 61, 59, 60, 88,  0, 68, 66, 64, 62, 15, 41,117,
    0, 56, 42, 93, 29, 16,  2,  0,  0,  0, 44, 31, 30, 17,  3,  0,
    0, 46, 45, 32, 18,  5,  4, 95,  0, 57, 47, 33, 20, 19,  6,183,
    0, 49, 48, 35, 34, 21,  7,184,  0,  0, 50, 36, 22,  8,  9,185,
    0, 51, 37, 23, 24, 11, 10,  0,  0, 52, 53, 38, 39, 25, 12,  0,
    0, 89, 40,  0, 26, 13,  0,  0, 58, 54, 28, 27,  0, 43,  0, 85,
    0, 86, 91, 90, 92,  0, 14, 94,  0, 79,124, 75, 71,121,  0,  0,
    82, 83, 80, 76, 77, 72,  1, 69, 87, 78, 81, 74, 55, 73, 70, 99,

    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    217,100,255,  0, 97,165,  0,  0,156,  0,  0,  0,  0,  0,  0,125,
    173,114,  0,113,  0,  0,  0,126,128,  0,  0,140,  0,  0,  0,127,
    159,  0,115,  0,164,  0,  0,116,158,  0,172,166,  0,  0,  0,142,
    157,  0,  0,  0,  0,  0,  0,  0,155,  0, 98,  0,  0,163,  0,  0,
    226,  0,  0,  0,  0,  0,  0,  0,  0,255, 96,  0,  0,  0,143,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,107,  0,105,102,  0,  0,112,
    110,111,108,112,106,103,  0,119,  0,118,109,  0, 99,104,119,  0,

    0,  0,  0, 65, 99,
};

BYTE g_LastScancode = 0;

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

 
INTERRUPT_HANDLER
VOID Drv8042IrqHandler(struct HAL_AMD64_INTERRUPT_FRAME* frame)
{
    BYTE Scancode = Drv8042ReadData();

    if((Scancode & 128) == 128);
        //g_LastScancode = NULL; // Released // TODO: Idk what to do here yet, so :/
    else
        g_LastScancode = Scancode; // Pressed

    KeHal8259SendEoi(1);
}

BYTE Drv8042GetLastScancode()
{
    // TODO: Mutexes :)
    return g_LastScancode;
}

BYTE Drv8042TranslateScancode(BYTE Scancode)
{
    return g_Drv8042KbdSet2Keymap[Scancode];
}