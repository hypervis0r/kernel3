#include "kernel/graphics/psf_font.h"

DOE_STATUS KeGfxPsfInitializeFont(PDOE_GFX_PSF_FONT PsfFont, VOID* FontBuffer, SIZE_T FontBufferSize)
{
    PsfFont->FontBuffer = FontBuffer;
    PsfFont->FontBufferSize = FontBufferSize;
    PsfFont->IsUnicode = FALSE;

    if (((PSFv1FontFileHeader*)FontBuffer)->magic == PSF1_FONT_MAGIC)
    {
        PSFv1FontFileHeader* tmp = (PSFv1FontFileHeader*)FontBuffer;

        PsfFont->FontInfo.headersize = sizeof(tmp);
        
        if (tmp->mode & PSF1_MODE512)
		    PsfFont->FontInfo.numglyph = 512;
	    else
		    PsfFont->FontInfo.numglyph = 256;

        PsfFont->FontInfo.flags = tmp->mode;
        PsfFont->FontInfo.bytesperglyph = tmp->charsize;
        PsfFont->FontInfo.height = tmp->charsize;
        PsfFont->FontInfo.width = 8;
    }
    else if (((PSFv2FontFileHeader*)FontBuffer)->magic == PSF2_FONT_MAGIC)
    {
        // TODO: replace with memcpy

        PSFv2FontFileHeader* tmp = (PSFv2FontFileHeader*)FontBuffer;

        PsfFont->FontInfo.magic = tmp->magic;                   /* magic bytes to identify PSF */
        PsfFont->FontInfo.version = tmp->version;               /* zero */
        PsfFont->FontInfo.headersize = tmp->headersize;         /* offset of bitmaps in file, 32 */
        PsfFont->FontInfo.flags = tmp->flags;                   /* 0 if there's no unicode table */
        PsfFont->FontInfo.numglyph = tmp->numglyph;             /* number of glyphs */
        PsfFont->FontInfo.bytesperglyph = tmp->bytesperglyph;   /* size of each glyph */
        PsfFont->FontInfo.height = tmp->height;                 /* height in pixels */
        PsfFont->FontInfo.width = tmp->width;                   /* width in pixels */
    }
    else
        return DOE_ERROR;

    //KeGfxPsfDecodeUnicodeTable(PsfFont);

    return DOE_SUCCESS;
}

VOID KeGfxPsfDecodeUnicodeTable(PDOE_GFX_PSF_FONT PsfFont)
{
    uint16_t glyph = 0;
    /* cast the address to PSF header struct */
    PSFv2FontFileHeader *font = &PsfFont->FontInfo;

    /* is there a unicode table?  TODO: This will bite me in the ass l8r */
    if (font->flags) {
        PsfFont->IsUnicode = FALSE;
        return; 
    }
 
    PsfFont->IsUnicode = TRUE;

    /* get the offset of the table */
    char *s = (char *)(
    (unsigned char*)PsfFont->FontBuffer +
      font->headersize +
      font->numglyph * font->bytesperglyph
    );
    /* allocate memory for translation table */
    while(s>((CHAR*)PsfFont->FontBuffer + PsfFont->FontBufferSize)) 
    {
        uint16_t uc = (uint16_t)((unsigned char *)s[0]);
        if(uc == 0xFF) {
            glyph++;
            s++;
            continue;
        } else if(uc & 128) {
            /* UTF-8 to unicode */
            if((uc & 32) == 0 ) {
                uc = ((s[0] & 0x1F)<<6)+(s[1] & 0x3F);
                s++;
            } else
            if((uc & 16) == 0 ) {
                uc = ((((s[0] & 0xF)<<6)+(s[1] & 0x3F))<<6)+(s[2] & 0x3F);
                s+=2;
            } else
            if((uc & 8) == 0 ) {
                uc = ((((((s[0] & 0x7)<<6)+(s[1] & 0x3F))<<6)+(s[2] & 0x3F))<<6)+(s[3] & 0x3F);
                s+=3;
            } else
                uc = 0;
        }
        /* save translation */
        PsfFont->UnicodeTable[uc] = glyph;
        s++;
    }
}

VOID KeGfxPsfPrintGlyph(PDOE_GFX_PSF_FONT PsfFont, WORD c, DWORD cx, DWORD cy, ARGB_COLOR fg, ARGB_COLOR bg)
{
    DWORD dwBytesPerScanLine = g_ScreenGraphicsBuffer.PixelsPerScanLine * sizeof(ARGB_COLOR);

    /* cast the address to PSF header struct */
    PSFv2FontFileHeader *font = &PsfFont->FontInfo;
    /* we need to know how many bytes encode one row */
    int bytesperline = (font->width + 7) / 8;
    /* unicode translation */
    if(PsfFont->IsUnicode != FALSE) 
    {
        c = PsfFont->UnicodeTable[c];
    }
    /* get the glyph for the character. If there's no
       glyph for a given character, we'll display the first glyph. */
    unsigned char *glyph =
     ((unsigned char*)PsfFont->FontBuffer) +
     font->headersize +
     ((c > 0 && c < font->numglyph) ? c : 0) * font->bytesperglyph;
    /* calculate the upper left corner on screen where we want to display.
       we only do this once, and adjust the offset later. This is faster. */
    int offs =
        (cy * font->height * dwBytesPerScanLine) +
        (cx * (font->width) * sizeof(ARGB_COLOR));
    /* finally display pixels according to the bitmap */
    int x, y, line, mask;
    for( y=0; y < font->height; y++)
    {
        /* save the starting position of the line */
        line=offs;
        mask=1<<(font->width - 1);
        /* display a row */
        for( x=0; x < font->width; x++)
        {
            *((ARGB_COLOR*)(g_ScreenGraphicsBuffer.Framebuffer + line)) = *((unsigned int*)glyph) & mask ? fg : bg;
            /* adjust to the next pixel */
            mask >>= 1;
            line += sizeof(ARGB_COLOR);
        }
        /* adjust to the next line */
        glyph += bytesperline;
        offs  += dwBytesPerScanLine;
    }
}