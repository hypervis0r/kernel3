#include "kernel/arch/amd64/gdt.h"

HAL_AMD64_GDT_REGISTER g_GDTR;

HAL_AMD64_SEGMENT_DESCRIPTOR g_GDT[] =
{
    /* Null Descriptor*/
    {
        .Limit = 0,
        .Base_1 = 0,
        .Base_2 = 0,
        .Access = 0,
        .HAHA_FUNNY = 0,
        .Base_3 = 0
    },
    /* Kernel Mode Code Segment */
    {
        .Limit = 0xFFFF,
        .Base_1 = 0,
        .Base_2 = 0,
        .Access = 0b10011000, // TODO: Make these less magic
        .HAHA_FUNNY = 0b10101111,
        .Base_3 = 0
    },
    /* Kernel Mode Data Segment */
    {
        .Limit = 0xFFFF,
        .Base_1 = 0,
        .Base_2 = 0,
        .Access = 0b10010010, // TODO: Make these less magic
        .HAHA_FUNNY = 0b10101111,
        .Base_3 = 0
    }
};

VOID KeHalGdtInitialize()
{
    g_GDTR.BaseAddress = &g_GDT[0];
    g_GDTR.Limit = sizeof(g_GDT);

    __asm__ volatile ("lgdt %0" : : "m"(g_GDTR)); // load the new GDT

    KeHalGdtReloadSegments();
}