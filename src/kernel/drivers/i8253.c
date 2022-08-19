#include "kernel/drivers/i8253.h"

SIZE_T g_TicksPassedSinceBoot = 0;

VOID Drv8253Initialize()
{
    Drv8253SetPhase(1000);

    g_TicksPassedSinceBoot = 0;
}

VOID Drv8253SetPhase(DWORD Hz)
{
    // Disable interrupts so that no funky business happens
    KeHalDisableInterrupts();

    WORD divisor = PIT_FREQ / Hz;       /* Calculate our divisor */
    KeHalPortWrite(PIT_COMMAND_PORT, 0x36);             /* Set our command byte 0x36 */
    KeHalPortWrite(PIT_CH0_PORT, divisor & 0xFF);   /* Set low byte of divisor */
    KeHalPortWrite(PIT_CH0_PORT, divisor >> 8);     /* Set high byte of divisor */

    KeHalEnableInterrupts();
}

INTERRUPT_HANDLER
VOID Drv8253IrqHandler(struct HAL_AMD64_INTERRUPT_FRAME* Frame)
{
    ++g_TicksPassedSinceBoot;
    KeGfxClearScreen(&g_ScreenGraphicsBuffer, g_TicksPassedSinceBoot);

    KeHal8259SendEoi(0);
}