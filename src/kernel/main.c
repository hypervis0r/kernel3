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

    KeGfxDrawRect(&g_ScreenGraphicsBuffer, 0, 0, 500, 400, 0x0000FF00);

    // DOE_GFX_PSF_FONT GfxFont;

    // Status = KeGfxPsfInitializeFont(&GfxFont, KernelInfo->FontBuffer, KernelInfo->FontBufferSize);
    // if (Status != DOE_SUCCESS)
    //     KePanic(Status, "Failed to initialize PSF font");

    // KeGfxPsfPrintGlyph(&GfxFont, 'h', 10, 10, 0xFFFFFFFF, 0xFF0000FF);

    for (;;) asm volatile("hlt");

    return EFI_SUCCESS;
}
