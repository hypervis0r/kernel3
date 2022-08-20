#include "kernel/tty.h"

DOE_TTY g_MainTty;

DOE_STATUS KeTermInitialize(PDOE_TTY Tty, PDOE_GFX_BUFFER GfxBuffer, PDOE_GFX_PSF_FONT Font)
{
    Tty->CharactersPerLine = (GfxBuffer->HorizontalResolution) / Font->FontInfo.width;
    Tty->MaxFramebufferLineCount = GfxBuffer->VerticalResolution / Font->FontInfo.height;

    Tty->BufferSize = sizeof(Tty->Buffer);
    Tty->BufferCount = 0;

    memset(Tty->Buffer, 0, Tty->BufferSize);

    Tty->CurrentX = 0;
    Tty->CurrentY = 0;

    Tty->Font = Font;
    Tty->GfxBuffer = GfxBuffer;

    return DOE_SUCCESS;
}

DOE_STATUS KeTermClear(PDOE_TTY Tty, ARGB_COLOR Fg)
{
    KeGfxClearScreen(Tty->GfxBuffer, Fg);

    memset(Tty->Buffer, 0, Tty->BufferSize);

    Tty->BufferCount = 0;

    Tty->CurrentX = 0;
    Tty->CurrentY = 0;

    return DOE_SUCCESS;
}

DOE_STATUS KeTermPutChar(PDOE_TTY Tty, BYTE c, ARGB_COLOR fg, ARGB_COLOR bg)
{
    if (Tty->CurrentX >= Tty->CharactersPerLine)
    {
        Tty->CurrentY += 1;
        Tty->CurrentX = 0;
    }

    KeTermPlaceCharIntoBuffer(Tty, c);

    KeGfxPsfPrintGlyph(Tty->Font, c, Tty->CurrentX++, Tty->CurrentY, fg, bg);

    return DOE_SUCCESS;
}

DOE_STATUS KeTermPlaceCharIntoBuffer(PDOE_TTY Tty, BYTE c)
{
    if (Tty->BufferCount + 1 >= Tty->BufferSize)
        return DOE_ERROR;

    Tty->Buffer[Tty->BufferCount++] = c;
}

BOOL KeTermHandleSpecialChar(PDOE_TTY Tty, BYTE c)
{
    switch (c)
    {
        case '\n':
        {
            Tty->CurrentY += 1;
            Tty->CurrentX = 0;

            KeTermPlaceCharIntoBuffer(Tty, c);

            return TRUE;
        }
        // TODO: Handle more types
    }

    return FALSE;
}

DOE_STATUS KeTermPutString(PDOE_TTY Tty, BYTE* Str, ARGB_COLOR fg, ARGB_COLOR bg)
{
    BYTE c = 0;
    DOE_STATUS Status = 0;
    BOOL Special = 0;

    while ((c = *Str++))
    {
        Special = KeTermHandleSpecialChar(Tty, c);
        if (Special != FALSE)
            continue;

        Status = KeTermPutChar(Tty, c, fg, bg);
        if (Status != DOE_SUCCESS)
            return Status;
    }

    return DOE_SUCCESS;
}