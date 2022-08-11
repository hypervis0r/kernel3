#include "kernel/graphics/graphics.h"

DOE_GFX_BUFFER g_ScreenGraphicsBuffer;

inline DOE_STATUS KeGfxDrawPixel(DOE_GFX_BUFFER* Buffer, SIZE_T x, SIZE_T y, ARGB_COLOR Color)
{
    if (x > Buffer->HorizontalResolution ||
		y > Buffer->VerticalResolution)
		return DOE_ERROR;
	
	*((UINT32*)(Buffer->Framebuffer + 4 * Buffer->PixelsPerScanLine * y + 4 * x)) = Color;

    return DOE_SUCCESS;
}

DOE_STATUS KeGfxDrawRect(DOE_GFX_BUFFER* Buffer, SIZE_T x, SIZE_T y, SIZE_T l, SIZE_T w, ARGB_COLOR Color)
{
	SIZE_T x2 = 0;
	SIZE_T w_Corner = w + y;
	SIZE_T l_Corner = x + l;

	for (; y < w_Corner; y++)
	{
		for (x2 = x; x2 < l_Corner; x2++)
		{
			if (KeGfxDrawPixel(Buffer, x2, y, Color) != DOE_SUCCESS)
				return DOE_ERROR;
		}
	}

	return DOE_SUCCESS;
}

inline DOE_STATUS KeGfxClearScreen(DOE_GFX_BUFFER* Buffer, ARGB_COLOR Color)
{
    KeGfxDrawRect(Buffer, 0, 0, Buffer->HorizontalResolution, Buffer->VerticalResolution, Color);

	return DOE_SUCCESS;
}