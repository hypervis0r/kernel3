#include "kernel/main.h"

EFI_STATUS KeMain(void)
{
    //KeBootClearScreen(BootloaderInfo->lpGopInfo, 0xFF0000);

    asm volatile("hlt");

    return EFI_SUCCESS;
}
