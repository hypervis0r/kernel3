#include "kernel/graphics/graphics.h"

DOE_GFX_BUFFER g_ScreenGraphicsBuffer;

inline DOE_STATUS KeGfxDrawPixel(DOE_GFX_BUFFER* Buffer, SIZE_T x, SIZE_T y, ARGB_COLOR Color)
{
    *((UINT32*)(Buffer->Framebuffer + 4 * Buffer->PixelsPerScanLine * y + 4 * x)) = Color;

    return DOE_SUCCESS;
}

DOE_STATUS KeGfxDrawRect(DOE_GFX_BUFFER* Buffer, SIZE_T x, SIZE_T y, SIZE_T l, SIZE_T w, ARGB_COLOR Color)
{
	SIZE_T x2 = 0;

	for (; y < w; y++)
	{
		for (x2 = x; x2 < l; x2++)
		{
			KeGfxDrawPixel(Buffer, x2, y, Color);
		}
	}

	return DOE_SUCCESS;
}

inline DOE_STATUS KeGfxClearScreen(DOE_GFX_BUFFER* Buffer, ARGB_COLOR Color)
{
    KeGfxDrawRect(Buffer, 0, 0, Buffer->HorizontalResolution, Buffer->VerticalResolution, Color);

	return DOE_SUCCESS;
}