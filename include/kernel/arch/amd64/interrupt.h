#pragma once

#include "kernel/doedef.h"

#include "kernel/arch/amd64/gdt.h"
#include "kernel/arch/amd64/port.h"
#include "kernel/arch/amd64/i8259.h"

typedef struct PACKED HAL_AMD64_IDT_REGISTER
{
    WORD Limit;
    ULONG_PTR BaseAddress;
} HAL_AMD64_IDT_REGISTER;

typedef struct HAL_AMD64_GATE_DESCRIPTOR {
   WORD Offset_1;       // offset bits 0..15
   WORD Selector;       // a code segment selector in GDT or LDT
   BYTE  IST;           // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
   BYTE  Attributes;    // gate type, dpl, and p fields
   WORD Offset_2;       // offset bits 16..31
   DWORD Offset_3;      // offset bits 32..63
   DWORD Reserved;      // reserved, must be set to zero
} HAL_AMD64_GATE_DESCRIPTOR, *PHAL_AMD64_GATE_DESCRIPTOR;

#define INTERRUPT_GATE_ATTRIB   0x8E // (p=1, dpl=0b00, type=0b1110 => INTERRUPT_DESCRIPTOR->Attributes=0b1000_1110 = 0x8E)
#define TRAP_GATE_ATTRIB        0x8F // (p=1, dpl=0b00, type=0b1111 => INTERRUPT_DESCRIPTOR->Attributes=0b1000_1111b = 0x8F)

#define IDT_MAX_DESCRIPTORS 256

VOID KeHalGateDescriptorInitialize(PHAL_AMD64_GATE_DESCRIPTOR Desc, ULONG_PTR Target, BYTE Attributes);

VOID KeHalIdtInitialize();

// using ULONG_PTR like this is so fucking dirty but fuck off
VOID KeHalIdtLoad(HAL_AMD64_IDT_REGISTER* Idtr);

VOID KeHalDisableInterrupts();
VOID KeHalEnableInterrupts();