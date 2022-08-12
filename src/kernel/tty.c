#include "kernel/tty.h"

DOE_STATUS KeTermInitialize(PDOE_TTY Tty, PDOE_GFX_BUFFER GfxBuffer, PDOE_GFX_PSF_FONT Font)
{
    Tty->CharactersPerLine = (GfxBuffer->HorizontalResolution) / Font->FontInfo.width;
    Tty->MaxFramebufferLineCount = GfxBuffer->VerticalResolution / Font->FontInfo.height;

    Tty->BufferSize = sizeof(Tty->Buffer);

    Tty->CurrentX = 0;
    Tty->CurrentY = 0;

    Tty->Font = Font;
    Tty->GfxBuffer = GfxBuffer;

    return DOE_SUCCESS;
}

DOE_STATUS KeTermPutChar(PDOE_TTY Tty, BYTE c, ARGB_COLOR fg, ARGB_COLOR bg)
{
    if (Tty->CurrentX >= Tty->CharactersPerLine)
    {
        Tty->CurrentY += 1;
        Tty->CurrentX = 0;
    }

    KeGfxPsfPrintGlyph(Tty->Font, c, Tty->CurrentX++, Tty->CurrentY, fg, bg);

    return DOE_SUCCESS;
}

DOE_STATUS KeTermPutString(PDOE_TTY Tty, BYTE* Str, ARGB_COLOR fg, ARGB_COLOR bg)
{
    BYTE c = 0;
    DOE_STATUS Status = 0;

    while ((c = *Str++))
    {
        Status = KeTermPutChar(Tty, c, fg, bg);
        if (Status != DOE_SUCCESS)
            return Status;
    }

    return DOE_SUCCESS;
}