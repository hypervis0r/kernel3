#pragma once

#include "kernel/main.h"
#include "kernel/graphics/graphics.h"

#define PSF_FONT_MAGIC 0x864ab572
 
typedef struct {
    DWORD magic;         /* magic bytes to identify PSF */
    DWORD version;       /* zero */
    DWORD headersize;    /* offset of bitmaps in file, 32 */
    DWORD flags;         /* 0 if there's no unicode table */
    DWORD numglyph;      /* number of glyphs */
    DWORD bytesperglyph; /* size of each glyph */
    DWORD height;        /* height in pixels */
    DWORD width;         /* width in pixels */
} PSFv2FontFileHeader;

typedef struct DOE_GFX_PSF_FONT
{
    VOID* FontBuffer;
    SIZE_T FontBufferSize;
    WORD UnicodeTable[4000];
    BOOL IsUnicode;
} DOE_GFX_PSF_FONT, *PDOE_GFX_PSF_FONT;

DOE_STATUS KeGfxPsfInitializeFont(PDOE_GFX_PSF_FONT PsfFont, VOID* FontBuffer, SIZE_T FontBufferSize);

VOID KeGfxPsfDecodeUnicodeTable(PDOE_GFX_PSF_FONT PsfFont);

VOID KeGfxPsfPrintGlyph(PDOE_GFX_PSF_FONT PsfFont, WORD c, DWORD cx, DWORD cy, ARGB_COLOR fg, ARGB_COLOR bg);