#include "kernel/main.h"

EFI_STATUS KeMain(SOA_KERNEL_INFORMATION* KernelInfo)
{
    EFI_STATUS Status = 0;

    //KeBootClearScreen(BootloaderInfo->lpGopInfo, 0xFF0000);

    for (int y = 0; y < KernelInfo->GraphicsInfo->VerticalResolution; y++)
	{
		for (int x = 0; x < KernelInfo->GraphicsInfo->HorizontalResolution; x++)
		{
			*((UINT32*)(KernelInfo->Framebuffer + 4 * KernelInfo->GraphicsInfo->PixelsPerScanLine * y + 4 * x)) = 0x0000FF00;
		}
	}

    for (;;) asm volatile("hlt");

    return EFI_SUCCESS;
}
