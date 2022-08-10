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

    KePanic(&GfxBuffer, 0x1, "Fucked");

    KeGfxClearScreen(&GfxBuffer, 0x000000FF);

    for (;;) asm volatile("hlt");

    return EFI_SUCCESS;
}
