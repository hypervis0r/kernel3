#include "kernel/main.h"

VOID ___chkstk_ms(VOID)
{
    return;
}

EFI_STATUS KeMain(SOA_KERNEL_INFORMATION* KernelInfo)
{
    DOE_STATUS Status = 0;

    g_ScreenGraphicsBuffer.Framebuffer = KernelInfo->Framebuffer;
    g_ScreenGraphicsBuffer.FramebufferSize = KernelInfo->FramebufferSize;
    g_ScreenGraphicsBuffer.HorizontalResolution = KernelInfo->GraphicsInfo->HorizontalResolution;
    g_ScreenGraphicsBuffer.VerticalResolution = KernelInfo->GraphicsInfo->VerticalResolution;
    g_ScreenGraphicsBuffer.PixelFormat = KernelInfo->GraphicsInfo->PixelFormat;
    g_ScreenGraphicsBuffer.PixelsPerScanLine = KernelInfo->GraphicsInfo->PixelsPerScanLine;

    //KeBootClearScreen(BootloaderInfo->lpGopInfo, 0xFF0000);

    KeGfxClearScreen(&g_ScreenGraphicsBuffer, 0x0000FF00);

    Status = KeGfxPsfInitializeFont(&g_LoadedPsfFont, KernelInfo->FontBuffer, KernelInfo->FontBufferSize);
    if (Status != DOE_SUCCESS)
        KePanic(Status, "Failed to initialize PSF font");

    char x = 0;
    char y = 0;

    char maxX = g_ScreenGraphicsBuffer.HorizontalResolution / g_LoadedPsfFont.FontInfo.width;

    for (unsigned char c = 0; c < 255; c++)
    {
        if (x >= maxX)
        {
            x = 0;
            ++y;
        }

        KeGfxPsfPrintGlyph(&g_LoadedPsfFont, c, x++, y, 0xFFFFFFFF, 0xFF0000FF);
    }

    KeGfxPsfPrintGlyph(&g_LoadedPsfFont, 'H', 10, 10, 0xFFFFFFFF, 0xFF0000FF);
    KeGfxPsfPrintGlyph(&g_LoadedPsfFont, 'E', 11, 10, 0xFFFFFFFF, 0xFF0000FF);
    KeGfxPsfPrintGlyph(&g_LoadedPsfFont, 'L', 12, 10, 0xFFFFFFFF, 0xFF0000FF);
    KeGfxPsfPrintGlyph(&g_LoadedPsfFont, 'L', 13, 10, 0xFFFFFFFF, 0xFF0000FF);
    KeGfxPsfPrintGlyph(&g_LoadedPsfFont, 'O', 14, 10, 0xFFFFFFFF, 0xFF0000FF);

    for (;;) asm volatile("hlt");

    return EFI_SUCCESS;
}
