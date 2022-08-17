#include "kernel/arch/amd64/interrupt.h"

#include "kernel/graphics/graphics.h"

static HAL_AMD64_IDT_REGISTER g_IDTR;
static HAL_AMD64_GATE_DESCRIPTOR g_IDT[IDT_MAX_DESCRIPTORS];

struct interrupt_frame
{
    SIZE_T ip;
    SIZE_T cs;
    SIZE_T flags;
    SIZE_T sp;
    SIZE_T ss;
};
 
INTERRUPT_HANDLER
void interrupt_handler(struct interrupt_frame *frame)
{
    unsigned char scan_code = KeHalPortRead(0x60);

    KeGfxClearScreen(&g_ScreenGraphicsBuffer, 0xFFFF0000);

    KeHal8259SendEoi(1);
}

VOID KeHalGateDescriptorInitialize(PHAL_AMD64_GATE_DESCRIPTOR Desc, ULONG_PTR Target, BYTE Attributes)
{
    Desc->Offset_1 = Target & 0xFFFF;
    Desc->Selector = GDT_OFFSET_KERNEL_CODE;
    Desc->IST = 0;
    Desc->Attributes = Attributes;
    Desc->Offset_2 = (Target >> 16) & 0xFFFF;
    Desc->Offset_3 = (Target >> 32) & 0xFFFFFFFF;
    Desc->Reserved = 0;
}

VOID KeHalIdtInitialize()
{
    g_IDTR.BaseAddress = (ULONG_PTR)&g_IDT[0];
    g_IDTR.Limit = sizeof(HAL_AMD64_GATE_DESCRIPTOR) * IDT_MAX_DESCRIPTORS - 1;

    KeHalGateDescriptorInitialize(&g_IDT[0x20], (ULONG_PTR)interrupt_handler, INTERRUPT_GATE_ATTRIB);

    KeHalIdtLoad(&g_IDTR);
}