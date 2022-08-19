/*
    PS/2 Keyboard Controller / Intel 8259
*/

#pragma once

#include "kernel/doedef.h"

#include "kernel/arch/amd64/port.h"
#include "kernel/arch/amd64/interrupt.h"

#include "kernel/tty.h"

#define PS2_DATA_PORT       0x60
#define PS2_STATUS_PORT     0x64 // Read
#define PS2_COMMAND_PORT    0x64 // Write

/*
    These define commands to be sent over the PS/2
    command port
*/

#define PS2_CMD_READ_CONFIG_BYTE 0x20
#define PS2_CMD_WRITE_CONFIG_BYTE 0x60 

#define PS2_CMD_TEST_CONTROLLER 0xAA

#define PS2_CMD_ENABLE_PORT1 0xAE
#define PS2_CMD_DISABLE_PORT1 0xAD

#define PS2_CMD_ENABLE_PORT2 0xA8
#define PS2_CMD_DISABLE_PORT2 0xA7

#define PS2_CMD_TEST_PORT1 0xAB
#define PS2_CMD_TEST_PORT2 0xA9

#define PS2_CMD_SEND_TO_PORT2 0xD4

/*
    These define responses returned after certain
    commands
*/
#define PS2_RESP_PORT_TEST_SUCCESS 0x00

#define PS2_RESP_SELF_TEST_SUCCESS 0x55
#define PS2_RESP_SELF_TEST_FAILED  0xFD

/*
    These are common structs used in the PS/2 Controller
*/
typedef union PS2_CONFIGURATION_BYTE
{
    struct
    {
        BYTE Port1Interrupt     : 1; // 1 - Enabled, 0 - Disabled
        BYTE Port2Interrupt     : 1; // 1 - Enabled, 0 - Disabled
        BYTE SystemFlag         : 1; // 1 - System passed POST, 0 - Your system's fucked
        BYTE Reserved1          : 1; // Must be zero
        BYTE Port1Clock         : 1; // 1 - Disabled, 0 - Enabled
        BYTE Port2Clock         : 1; // 1 - Disabled, 0 - Enabled
        BYTE Port1Translation   : 1; // 1 - Enabled, 0 - Disabled
        BYTE Reserved2          : 1; // Must be zero
    };
    BYTE Data;
} PS2_CONFIGURATION_BYTE;

typedef union PS2_STATUS_REGISTER
{
    struct
    {
        BYTE OutputStatus   : 1;
        BYTE InputStatus    : 1;
        BYTE SystemFlag     : 1;
        BYTE CommandOrData  : 1;
        BYTE Unknown1       : 1;
        BYTE Unknown2       : 1;
        BYTE TimeoutError   : 1;
        BYTE ParityError    : 1;
    };
    BYTE Data;
} PS2_STATUS_REGISTER;

#define KBD_KEYMAP_SIZE 512

DOE_STATUS Drv8042Initialize();

/*
    Disable/Enable PS/2 Ports

    Port == 0 // Port 1+2
    Port == 1 // Port 1
    Port == 2 // Port 2
*/
VOID Drv8042DisablePorts(BYTE Port);
VOID Drv8042EnablePorts(BYTE Port);

/*
    Read and write data to PS/2 Data port
*/
BYTE Drv8042ReadData();
VOID Drv8042WriteData(BYTE Data);

/*
    Run a blocking poll loop until status register bit is 
    NOT EQUAL to Check.

    Check == 0 // Poll until buffer is full
    Check == 1 // Poll until buffer is empty
*/
VOID Drv8042PollOutputBufferStatus(BYTE Check);
VOID Drv8042PollInputBufferStatus(BYTE Check);

PS2_STATUS_REGISTER Drv8042ReadStatusRegister();

PS2_CONFIGURATION_BYTE Drv8042ReadConfigurationByte();
VOID Drv8042WriteConfigurationByte(PS2_CONFIGURATION_BYTE Config);

VOID Drv8042SendTwoByteCommand(BYTE Cmd, BYTE Byte2);
BYTE Drv8042SendCommandAndRecvResponse(BYTE Cmd);

BOOL Drv8042RunSelfTest();
BOOL Drv8042RunPortTest(BYTE Port);

BOOL Drv8042ResetDevice(BYTE Port);

INTERRUPT_HANDLER
VOID Drv8042IrqHandler(struct HAL_AMD64_INTERRUPT_FRAME* frame);

BYTE Drv8042GetLastScancode();
BYTE Drv8042PopLastScancode();
BYTE Drv8042TranslateScancode(BYTE Scancode);