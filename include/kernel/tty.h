#pragma once

#include "kernel/doedef.h"

#include "kernel/graphics/psf_font.h"
#include "kernel/graphics/graphics.h"

#include "kernel/libk/string.h"

typedef struct DOE_TTY
{
    SIZE_T CharactersPerLine;
    SIZE_T MaxFramebufferLineCount;
    BYTE Buffer[4000];
    SIZE_T BufferSize;
    SIZE_T BufferCount;
    SIZE_T CurrentX;
    SIZE_T CurrentY;

    PDOE_GFX_PSF_FONT Font;
    PDOE_GFX_BUFFER GfxBuffer;
} DOE_TTY, *PDOE_TTY;

extern DOE_TTY g_MainTty;

DOE_STATUS KeTermInitialize(PDOE_TTY Tty, PDOE_GFX_BUFFER GfxBuffer, PDOE_GFX_PSF_FONT Font);

DOE_STATUS KeTermClear(PDOE_TTY Tty, ARGB_COLOR Fg);

BOOL KeTermHandleSpecialChar(PDOE_TTY Tty, BYTE c);

DOE_STATUS KeTermPlaceCharIntoBuffer(PDOE_TTY Tty, BYTE c);

DOE_STATUS KeTermPutChar(PDOE_TTY Tty, BYTE c, ARGB_COLOR fg, ARGB_COLOR bg);

DOE_STATUS KeTermPutString(PDOE_TTY Tty, BYTE* Str, ARGB_COLOR fg, ARGB_COLOR bg);