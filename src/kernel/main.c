#include "kernel/main.h"

EFI_STATUS KeMain(EFI_SYSTEM_TABLE* SystemTable)
{
    EFI_STATUS Status = 0;

    //KeBootClearScreen(BootloaderInfo->lpGopInfo, 0xFF0000);

    Status = SystemTable->ConOut->OutputString(SystemTable->ConOut, L"[+] WE IN THE KERNEL BITCHES\r\n");
	if (EFI_ERROR(Status))
	    return Status;

    for (;;) asm volatile("hlt");

    return EFI_SUCCESS;
}
