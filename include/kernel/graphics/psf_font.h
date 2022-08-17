#pragma once

#include "kernel/doedef.h"
#include "kernel/graphics/graphics.h"

#define PSF1_FONT_MAGIC     0x0436

#define PSF1_MODE512    0x01
#define PSF1_MODEHASTAB 0x02
#define PSF1_MODEHASSEQ 0x04
#define PSF1_MAXMODE    0x05

#define PSF1_SEPARATOR  0xFFFF
#define PSF1_STARTSEQ   0xFFFE

#define PSF2_FONT_MAGIC 0x864ab572

enum {
	PSF2_MAGIC0 = 0x72,
	PSF2_MAGIC1 = 0xb5,
	PSF2_MAGIC2 = 0x4a,
	PSF2_MAGIC3 = 0x86,
	PSF2_HAS_UNICODE_TABLE = 0x01,
	PSF2_MAXVERSION = 0,
	PSF2_STARTSEQ = 0xfe,
	PSF2_SEPARATOR = 0xff
};
 
typedef struct PSFv2FontFileHeader 
{
    DWORD magic;         /* magic bytes to identify PSF */
    DWORD version;       /* zero */
    DWORD headersize;    /* offset of bitmaps in file, 32 */
    DWORD flags;         /* 0 if there's no unicode table */
    DWORD numglyph;      /* number of glyphs */
    DWORD bytesperglyph; /* size of each glyph */
    DWORD height;        /* height in pixels */
    DWORD width;         /* width in pixels */
} PSFv2FontFileHeader;

typedef struct PSFv1FontFileHeader
{
    WORD magic;     /* Magic number */
    BYTE mode;         /* PSF font mode */
    BYTE charsize;     /* Character size */
} PSFv1FontFileHeader;

typedef struct DOE_GFX_PSF_FONT
{
    PSFv2FontFileHeader FontInfo;
    VOID* FontBuffer;
    SIZE_T FontBufferSize;
    WORD UnicodeTable[4000];
    BOOL IsUnicode;
} DOE_GFX_PSF_FONT, *PDOE_GFX_PSF_FONT;

extern DOE_GFX_PSF_FONT g_LoadedPsfFont;

DOE_STATUS KeGfxPsfInitializeFont(PDOE_GFX_PSF_FONT PsfFont, VOID* FontBuffer, SIZE_T FontBufferSize);

VOID KeGfxPsfDecodeUnicodeTable(PDOE_GFX_PSF_FONT PsfFont);

VOID KeGfxPsfPrintGlyph(PDOE_GFX_PSF_FONT PsfFont, WORD c, DWORD cx, DWORD cy, ARGB_COLOR fg, ARGB_COLOR bg);