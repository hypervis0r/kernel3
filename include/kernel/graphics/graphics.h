#pragma once

#include <stdint.h>
#include <Uefi.h>
#include <Protocol/GraphicsOutput.h>

#include "kernel/main.h"

typedef DWORD ARGB_COLOR;

typedef struct DOE_GFX_BUFFER
{
    EFI_PHYSICAL_ADDRESS        Framebuffer;
    SIZE_T                      FramebufferSize;
    DWORD                       HorizontalResolution;
    DWORD                       VerticalResolution;
    EFI_GRAPHICS_PIXEL_FORMAT   PixelFormat;
    DWORD                       PixelsPerScanLine;
} DOE_GFX_BUFFER, *PDOE_GFX_BUFFER;

DOE_STATUS KeGfxDrawPixel(DOE_GFX_BUFFER* Buffer, SIZE_T x, SIZE_T y, ARGB_COLOR Color);

DOE_STATUS KeGfxClearScreen(DOE_GFX_BUFFER* Buffer, ARGB_COLOR Color);
