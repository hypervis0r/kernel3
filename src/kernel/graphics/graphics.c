#include "kernel/graphics/graphics.h"

inline DOE_STATUS KeGfxDrawPixel(DOE_GFX_BUFFER* Buffer, SIZE_T x, SIZE_T y, ARGB_COLOR Color)
{
    *((UINT32*)(Buffer->Framebuffer + 4 * Buffer->PixelsPerScanLine * y + 4 * x)) = Color;

    return DOE_SUCCESS;
}