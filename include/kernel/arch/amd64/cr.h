/*
    Wrapper code for AMD64 System Control Registers
*/

#pragma once

#include "kernel/doedef.h"

typedef union HAL_AMD64_CR0_REGISTER
{
	struct
	{
		DWORD ProtectedModeEnable   : 1;
		DWORD MonitorCoprocessor    : 1;
		DWORD EmulateCoprocessor    : 1;
		DWORD TaskSwitched          : 1;
		DWORD ExtensionType         : 1; // Read only
		DWORD NumericError          : 1;
		DWORD Reserved1             : 10; // Do not change
		DWORD WriteProtect          : 1;
		DWORD Reserved2             : 1; // Do not change
		DWORD AlignmentMask         : 1;
		DWORD Reserved3             : 10; // Do not change
		DWORD NotWritethrough       : 1; // Legacy, value can be whatever
		DWORD CacheDisable          : 1;
		DWORD PagingEnable          : 1;
		DWORD Reserved4; // Must be zero
	};

	QWORD Data;
} HAL_AMD64_CR0_REGISTER;

// CR2 contains virtual address during #PF exceptions
typedef QWORD HAL_AMD64_CR2_REGISTER; 

typedef union HAL_AMD64_CR3_REGISTER
{
	struct
	{
		DWORD ProcessorContextId : 12;
        DWORD Pml4TableBaseAddress : 40;
        DWORD Reserved : 12;
	};

	QWORD Data;
} HAL_AMD64_CR3_REGISTER;

typedef union HAL_AMD64_CR4_REGISTER
{
	struct
	{
		DWORD Virtual8086ModeExtensions         : 1;
		DWORD ProtectedModeVirtualInterrupts    : 1;
		DWORD TimeStampDisable                  : 1;
		DWORD DebuggingExtensions               : 1;
		DWORD PageSizeExtensions                : 1; // Ignored in long mode
		DWORD PhysicalAddressExtension          : 1;
		DWORD MachineCheckEnable                : 1;
		DWORD PageGlobalEnable                  : 1;
		DWORD PerfMonitorCounterEnable          : 1; 
		DWORD FxSupport                         : 1;
		DWORD SimdFpExceptionSupport            : 1; 
		DWORD UserModeInstructionPrevention     : 1; 
        DWORD Reserved1                         : 4;
        DWORD FsGsBase                          : 1;
		DWORD ProcessorContextIdentifierEnable  : 1;
		DWORD ExtendedStatesEnable              : 1;
        DWORD Reserved2                         : 1;
        DWORD SupervisorModeExecutionPrevention : 1;
        DWORD SupervisorModeAccessPrevention    : 1;
        DWORD ProtectionKeyEnable               : 1;
        DWORD CetEnable                         : 1; 
        DWORD Reserved3                         : 8;
        DWORD Reserved4;
	};

	QWORD Data;
} HAL_AMD64_CR4_REGISTER;

typedef union HAL_AMD64_CR8_REGISTER
{
	struct
	{
		QWORD TaskPriority : 4;
        QWORD Reserved : 60;
	};

	QWORD Data;
} HAL_AMD64_CR8_REGISTER;

// CR0 Access
VOID KeHalCr0Load(HAL_AMD64_CR0_REGISTER Cr);
HAL_AMD64_CR0_REGISTER KeHalCr0Read();

// CR2 Access
VOID KeHalCr2Load(HAL_AMD64_CR2_REGISTER Cr);
HAL_AMD64_CR2_REGISTER KeHalCr2Read();

// CR3 Access
VOID KeHalCr3Load(HAL_AMD64_CR3_REGISTER Cr);
HAL_AMD64_CR3_REGISTER KeHalCr3Read();

// CR4 Access
VOID KeHalCr4Load(HAL_AMD64_CR4_REGISTER Cr);
HAL_AMD64_CR4_REGISTER KeHalCr4Read();

// CR8 Access
VOID KeHalCr8Load(HAL_AMD64_CR8_REGISTER Cr);
HAL_AMD64_CR8_REGISTER KeHalCr8Read();