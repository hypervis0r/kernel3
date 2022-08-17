#pragma once

#include "kernel/doedef.h"

#define GDT_OFFSET_KERNEL_CODE 0x08

typedef struct PACKED HAL_AMD64_GDT_REGISTER
{
    WORD Limit;
    ULONG_PTR BaseAddress;
} HAL_AMD64_GDT_REGISTER;

typedef struct PACKED HAL_AMD64_SEGMENT_DESCRIPTOR {
    WORD Limit;
    WORD Base_1;
    BYTE Base_2;
    BYTE Access;
    BYTE HAHA_FUNNY; // 4-bit Limit + 4-bit Flags
    BYTE Base_3;
} HAL_AMD64_SEGMENT_DESCRIPTOR, *PHAL_AMD64_SEGMENT_DESCRIPTOR;

VOID KeHalGdtInitialize();

VOID KeHalGdtReloadSegments();