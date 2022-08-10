#include "kernel/panic.h"

VOID KePanic(DOE_GFX_BUFFER* GfxBuffer, DOE_STATUS StatusCode, BYTE* Message)
{
    KeGfxClearScreen(GfxBuffer, 0x00FF0000);

    for (;;) KeHalHaltProcessor();
}