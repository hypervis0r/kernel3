#include "kernel/graphics/graphics.h"

DOE_GFX_BUFFER g_ScreenGraphicsBuffer;

inline DOE_STATUS KeGfxDrawPixel(DOE_GFX_BUFFER* Buffer, SIZE_T x, SIZE_T y, ARGB_COLOR Color)
{
    *((UINT32*)(Buffer->Framebuffer + 4 * Buffer->PixelsPerScanLine * y + 4 * x)) = Color;

    return DOE_SUCCESS;
}

inline DOE_STATUS KeGfxClearScreen(DOE_GFX_BUFFER* Buffer, ARGB_COLOR Color)
{
    for (int y = 0; y < Buffer->VerticalResolution; y++)
	{
		for (int x = 0; x < Buffer->HorizontalResolution; x++)
		{
			KeGfxDrawPixel(Buffer, x, y, Color);
		}
	}
}