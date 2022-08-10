#include "kernel/panic.h"

VOID KePanic(DOE_STATUS StatusCode, BYTE* Message)
{
    KeGfxClearScreen(&g_ScreenGraphicsBuffer, 0x00FF0000);

    for (;;) KeHalHaltProcessor();
}