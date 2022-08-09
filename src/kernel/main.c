#include "kernel/main.h"

EFI_STATUS KeMain(SOA_KERNEL_INFORMATION* KernelInfo)
{
    EFI_STATUS Status = 0;

    //KeBootClearScreen(BootloaderInfo->lpGopInfo, 0xFF0000);

    Status = KernelInfo->SystemTable->ConOut->OutputString(KernelInfo->SystemTable->ConOut, L"[+] WE IN THE KERNEL BITCHES\r\n");
	if (EFI_ERROR(Status))
	    return Status;

    for (;;) asm volatile("hlt");

    return EFI_SUCCESS;
}
