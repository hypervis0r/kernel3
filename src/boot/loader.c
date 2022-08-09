#include "boot/loader.h"

EFI_STATUS BtLoadPeFile(EFI_SYSTEM_TABLE *SystemTable, CHAR16* Path)
{
    EFI_STATUS Status = 0;

    EFI_BOOT_SERVICES* BootServices = SystemTable->BootServices;

    VOID* FileBuffer = NULL;
    UINTN FileSize = 0;

    Status = BtLoadFile(SystemTable, Path, &FileBuffer, &FileSize);
    if (EFI_ERROR(Status))
	    return Status;

    Status = SystemTable->ConOut->OutputString(SystemTable->ConOut, L"[+] File loaded into memory\r\n"); // EFI Applications use Unicode and CRLF, a la Windows
	if (EFI_ERROR(Status))
	    return Status;

    Status = BootServices->FreePool(FileBuffer);
    if (EFI_ERROR(Status))
	    return Status;

    return EFI_SUCCESS;
}