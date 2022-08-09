#include <Uefi.h>

#include "boot/loader.h"
#include "boot/gop.h"
 
EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
	EFI_STATUS Status;
	EFI_INPUT_KEY Key;
	
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* GopModeInfo = NULL;
	EFI_PHYSICAL_ADDRESS Framebuffer = NULL;
	UINTN FramebufferSize = 0;
	Status = BtGopGetFramebuffer(SystemTable, &GopModeInfo, &Framebuffer, &FramebufferSize);
	if (EFI_ERROR(Status))
	    return Status;

	for (int y = 0; y < GopModeInfo->VerticalResolution; y++)
	{
		for (int x = 0; x < GopModeInfo->HorizontalResolution; x++)
		{
			*((UINT32*)(Framebuffer + 4 * GopModeInfo->PixelsPerScanLine * y + 4 * x)) = 0x00FF0000;
		}
	}

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
 
	EFI_PHYSICAL_ADDRESS KernelPeAddress = NULL;

	Status = BtPeLoad(SystemTable, L"\\system\\kernel3.exe", &KernelPeAddress);
	if (EFI_ERROR(Status))
		return Status;

	Status = BtPeExecuteEntryPoint(SystemTable, KernelPeAddress);
	if (EFI_ERROR(Status))
		return Status;

    /* 
		Now wait until a key becomes available.  This is a simple
       	polling implementation.  You could try and use the WaitForKey
       	event instead if you like 
	*/
    while ((Status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key)) == EFI_NOT_READY) ;
 
    return Status;
}
