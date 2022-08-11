#pragma once

#include <stdint.h>
#include <Uefi.h>
#include <Protocol/GraphicsOutput.h>

#include "kernel/main.h"

typedef struct DOE_GFX_BUFFER
{
    EFI_PHYSICAL_ADDRESS        Framebuffer;
    SIZE_T                      FramebufferSize;
    DWORD                       HorizontalResolution;
    DWORD                       VerticalResolution;
    EFI_GRAPHICS_PIXEL_FORMAT   PixelFormat;
    DWORD                       PixelsPerScanLine;
} DOE_GFX_BUFFER, *PDOE_GFX_BUFFER;

extern DOE_GFX_BUFFER g_ScreenGraphicsBuffer;

DOE_STATUS KeGfxDrawPixel(DOE_GFX_BUFFER* Buffer, SIZE_T x, SIZE_T y, ARGB_COLOR Color);

DOE_STATUS KeGfxDrawRect(DOE_GFX_BUFFER* Buffer, SIZE_T x, SIZE_T y, SIZE_T l, SIZE_T w, ARGB_COLOR Color);

DOE_STATUS KeGfxClearScreen(DOE_GFX_BUFFER* Buffer, ARGB_COLOR Color);
