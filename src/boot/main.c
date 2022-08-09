#include <Uefi.h>

#include "boot/loader.h"
 
EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
	EFI_STATUS Status;
	EFI_INPUT_KEY Key;
 
    /* 
		Say hi 
	*/
	Status = SystemTable->ConOut->OutputString(SystemTable->ConOut, L"[+] Scourge of Armagon online\r\n"); // EFI Applications use Unicode and CRLF, a la Windows
	if (EFI_ERROR(Status))
	    return Status;

	/*
		Disable Watchdog Timer
	*/
    Status = SystemTable->BootServices->SetWatchdogTimer(0, 0, 0, NULL);
    if (EFI_ERROR(Status))
	    return Status;
 
    /* 
		Now wait for a keystroke before continuing, otherwise your
    	message will flash off the screen before you see it.
 
    	First, we need to empty the console input buffer to flush
    	out any keystrokes entered before this point 
	*/
	Status = SystemTable->ConIn->Reset(SystemTable->ConIn, FALSE);
	if (EFI_ERROR(Status))
		return Status;
 
	BtLoadPeFile(SystemTable, L"\\system\\kernel3.exe");

    /* 
		Now wait until a key becomes available.  This is a simple
       	polling implementation.  You could try and use the WaitForKey
       	event instead if you like 
	*/
    while ((Status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key)) == EFI_NOT_READY) ;
 
    return Status;
}
