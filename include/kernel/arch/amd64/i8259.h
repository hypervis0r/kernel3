/*
    Programmable Interrupt Controller / Intel 8259
*/

#pragma once

#include "kernel/doedef.h"

#include "kernel/arch/amd64/port.h"

/* Ports for master PIC */
#define PIC1_COMMAND_PORT	0x20
#define PIC1_DATA_PORT	    0x21

/* Ports for slave PIC */
#define PIC2_COMMAND_PORT	0xA0
#define PIC2_DATA_PORT	    0xA1

#define PIC_EOI		0x20		/* End-of-interrupt command code */

#define ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */
 
#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

VOID KeHal8259SendEoi(BYTE irq);

VOID KeHal8259Remap(BYTE MasterOffset, BYTE SlaveOffset);

VOID KeHalIrqMaskAllLines();

VOID KeHalIrqSetMask(BYTE IrqLine);
VOID KeHalIrqClearMask(BYTE IrqLine);