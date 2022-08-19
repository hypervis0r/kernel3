/*
    Programmable Interval Timer / Intel 8253/8254
*/

#pragma once

#include "kernel/doedef.h"

#include "kernel/arch/amd64/port.h"
#include "kernel/arch/amd64/interrupt.h"

#include "kernel/graphics/graphics.h"

#define PIT_CH0_PORT 0x40
#define PIT_CH1_PORT 0x41
#define PIT_CH2_PORT 0x42
#define PIT_COMMAND_PORT 0x43

#define PIT_FREQ 1193182

extern SIZE_T g_TicksPassedSinceBoot;

VOID Drv8253Initialize();

VOID Drv8253SetPhase(DWORD Hz);

INTERRUPT_HANDLER(Drv8253IrqHandler);