#include "kernel/main.h"

EFI_STATUS KeMain(SOA_KERNEL_INFORMATION* KernelInfo)
{
    EFI_STATUS Status = 0;

    g_ScreenGraphicsBuffer.Framebuffer = KernelInfo->Framebuffer;
    g_ScreenGraphicsBuffer.FramebufferSize = KernelInfo->FramebufferSize;
    g_ScreenGraphicsBuffer.HorizontalResolution = KernelInfo->GraphicsInfo->HorizontalResolution;
    g_ScreenGraphicsBuffer.VerticalResolution = KernelInfo->GraphicsInfo->VerticalResolution;
    g_ScreenGraphicsBuffer.PixelFormat = KernelInfo->GraphicsInfo->PixelFormat;
    g_ScreenGraphicsBuffer.PixelsPerScanLine = KernelInfo->GraphicsInfo->PixelsPerScanLine;

    //KeBootClearScreen(BootloaderInfo->lpGopInfo, 0xFF0000);

    KeGfxClearScreen(&g_ScreenGraphicsBuffer, 0x0000FF00);

    DOE_GFX_PSF_FONT GfxFont;

    KeGfxPsfInitializeFont(&GfxFont, KernelInfo->FontBuffer, KernelInfo->FontBufferSize);

    KeGfxPsfPrintGlyph(&GfxFont, 'h', 10, 10, 0xFFFFFFFF, 0xFF0000FF);

    for (;;) asm volatile("hlt");

    return EFI_SUCCESS;
}
