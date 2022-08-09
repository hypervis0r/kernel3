#include "kernel/main.h"

EFI_STATUS KeMain(SOA_KERNEL_INFORMATION* KernelInfo)
{
    EFI_STATUS Status = 0;
    DOE_GFX_BUFFER GfxBuffer;

    GfxBuffer.Framebuffer = KernelInfo->Framebuffer;
    GfxBuffer.FramebufferSize = KernelInfo->FramebufferSize;
    GfxBuffer.HorizontalResolution = KernelInfo->GraphicsInfo->HorizontalResolution;
    GfxBuffer.VerticalResolution = KernelInfo->GraphicsInfo->VerticalResolution;
    GfxBuffer.PixelFormat = KernelInfo->GraphicsInfo->PixelFormat;
    GfxBuffer.PixelsPerScanLine = KernelInfo->GraphicsInfo->PixelsPerScanLine;

    //KeBootClearScreen(BootloaderInfo->lpGopInfo, 0xFF0000);

    KeGfxClearScreen(&GfxBuffer, 0x0000FF00);

    for (;;) asm volatile("hlt");

    return EFI_SUCCESS;
}
