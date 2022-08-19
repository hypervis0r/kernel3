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

    KeHalGdtInitialize();

    KeHalDisableInterrupts();

    

    //KeBootClearScreen(BootloaderInfo->lpGopInfo, 0xFF0000);

    KeGfxClearScreen(&g_ScreenGraphicsBuffer, 0x0000FF00);

    Status = KeGfxPsfInitializeFont(&g_LoadedPsfFont, KernelInfo->FontBuffer, KernelInfo->FontBufferSize);
    if (Status != DOE_SUCCESS)
        KePanic(Status, "Failed to initialize PSF font");

    DOE_TTY Tty;

    KeTermInitialize(&Tty, &g_ScreenGraphicsBuffer, &g_LoadedPsfFont);

    for (BYTE c = 0; c < 255; c++)
    {
        KeTermPutChar(&Tty, c, 0xFFFFFFFF, 0xFF0000FF);
    }

    //

    // if (Drv8042Initialize() != DOE_SUCCESS)
    // {
    //     KeGfxClearScreen(&g_ScreenGraphicsBuffer, 0x000000FF);
    // }

    Drv8253Initialize();

    KeHalIdtInitialize();

    //KeHalIrqClearMask(0);
    //KeHalIrqClearMask(1);

    //KeGfxClearScreen(&g_ScreenGraphicsBuffer, 0x000000FF);

    KeHalEnableInterrupts();

    //KeHalTriggerSoftwareInterrupt();

    

    //KeHalTriggerSoftwareInterrupt();

    for (;;) asm volatile("hlt");

    return EFI_SUCCESS;
}
