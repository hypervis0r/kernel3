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

	UINTN MemoryMapSize = 0;
	EFI_MEMORY_DESCRIPTOR* MemoryMap = NULL;

	UINTN MapKey;
	UINTN DescriptorSize;
	UINT32 DescriptorVersion;

	Status = SystemTable->BootServices->GetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
	if (Status != EFI_BUFFER_TOO_SMALL)
		return Status;	

	MemoryMapSize = MemoryMapSize + (2 * DescriptorSize);

	Status = SystemTable->BootServices->AllocatePool(EfiLoaderData, MemoryMapSize, &MemoryMap);
	if (EFI_ERROR(Status))
		return Status;

	Status = SystemTable->BootServices->GetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
	if (EFI_ERROR(Status))
		return Status;

	Status = SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);
	if (EFI_ERROR(Status))
		return Status;

	SOA_KERNEL_INFORMATION KernelInfo;

	KernelInfo.Framebuffer = Framebuffer;
	KernelInfo.FramebufferSize = FramebufferSize;
	KernelInfo.KernelBase = KernelPeAddress;
	KernelInfo.GraphicsInfo = GopModeInfo;
	KernelInfo.SystemTable = SystemTable;

	Status = BtPeExecuteEntryPoint(SystemTable, KernelPeAddress, &KernelInfo);
	if (EFI_ERROR(Status))
		return Status;
 
    return Status;
}
