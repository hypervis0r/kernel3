#include "kernel/arch/amd64/i8259.h"

VOID KeHal8259SendEoi(BYTE Irq)
{
    if (Irq >= 8)
        KeHalPortWrite(PIC2_COMMAND_PORT, PIC_EOI);

    KeHalPortWrite(PIC1_COMMAND_PORT, PIC_EOI);
}

VOID KeHal8259Remap(DWORD MasterOffset, DWORD SlaveOffset)
{
    BYTE mask1, mask2;

    mask1 = KeHalPortRead(PIC1_DATA_PORT);
    mask2 = KeHalPortRead(PIC2_DATA_PORT);

    KeHalPortWrite(PIC1_COMMAND_PORT, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	KeHalPortWrite(PIC2_COMMAND_PORT, ICW1_INIT | ICW1_ICW4);

	KeHalPortWrite(PIC1_DATA_PORT, MasterOffset);                 // ICW2: Master PIC vector offset
	KeHalPortWrite(PIC2_DATA_PORT, SlaveOffset);                 // ICW2: Slave PIC vector offset

	KeHalPortWrite(PIC1_DATA_PORT, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	KeHalPortWrite(PIC2_DATA_PORT, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
 
	KeHalPortWrite(PIC1_DATA_PORT, ICW4_8086);
	KeHalPortWrite(PIC2_DATA_PORT, ICW4_8086);

 
	KeHalPortWrite(PIC1_DATA_PORT, mask1);   // restore saved masks.
	KeHalPortWrite(PIC2_DATA_PORT, mask2);
}

VOID KeHalIrqMaskAllLines()
{
    KeHalPortWrite(PIC2_DATA_PORT, 0xFF);
    KeHalPortWrite(PIC1_DATA_PORT, 0xFF);
}

VOID KeHalIrqSetMask(BYTE IrqLine)
{
    DWORD port;
    BYTE value;

    if (IrqLine < 8) 
    {
        port = PIC1_DATA_PORT;
    } 
    else 
    {
        port = PIC2_DATA_PORT;
        IrqLine -= 8;
    }

    value = KeHalPortRead(port) | (1 << IrqLine);
    KeHalPortWrite(port, value);
}

VOID KeHalIrqClearMask(BYTE IrqLine)
{
    DWORD port;
    BYTE value;

    if (IrqLine < 8) 
    {
        port = PIC1_DATA_PORT;
    } 
    else 
    {
        port = PIC2_DATA_PORT;
        IrqLine -= 8;
    }

    value = KeHalPortRead(port) & ~(1 << IrqLine);
    KeHalPortWrite(port, value);
}