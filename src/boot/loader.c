#include "boot/loader.h"

EFI_STATUS MapPeSections(EFI_SYSTEM_TABLE *SystemTable, VOID* Buffer, EFI_PHYSICAL_ADDRESS* BaseAddress)
{
    EFI_STATUS Status = 0;
    PIMAGE_NT_HEADERS pNtHeaders;
	PIMAGE_DATA_DIRECTORY pDataDir;
	PIMAGE_BASE_RELOCATION pRelocation;
	PIMAGE_SECTION_HEADER pSectionHeader;
    PIMAGE_DOS_HEADER pDosHeader;

    pDosHeader = (PIMAGE_DOS_HEADER)Buffer;

    if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
        return 1;

	pNtHeaders = RVA(
		PIMAGE_NT_HEADERS,
		Buffer,
		((PIMAGE_DOS_HEADER)Buffer)->e_lfanew
	);

    if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE)
        return 2;

    if (pNtHeaders->FileHeader.Machine != IMAGE_FILE_MACHINE_AMD64)
        return 3;

    if (pNtHeaders->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC)
        return 4;

    UINTN RegionSize = pNtHeaders->OptionalHeader.SizeOfImage;
    UINTN RegionPagesCount = EFI_SIZE_TO_PAGES(RegionSize);

    EFI_PHYSICAL_ADDRESS PeBuffer = pNtHeaders->OptionalHeader.ImageBase;

    Status = SystemTable->BootServices->AllocatePages(AllocateAnyPages, EfiLoaderData, RegionPagesCount, &PeBuffer);
    if (EFI_ERROR(Status))
	    return Status;

    // copy across the headers
	for (UINT32 i = 0; i < pNtHeaders->OptionalHeader.SizeOfHeaders; i++)
	{
		((UINT8*)PeBuffer)[i] = ((UINT8*)Buffer)[i];
	}

	// copy across the sections
	pSectionHeader = IMAGE_FIRST_SECTION(pNtHeaders);

	for (UINT32 i = 0; i < pNtHeaders->FileHeader.NumberOfSections; i++, pSectionHeader++)
	{
		for (UINT32 j = 0; j < pSectionHeader->SizeOfRawData; j++)
		{
			((UINT8*)(PeBuffer + pSectionHeader->VirtualAddress))[j] = ((UINT8*)(Buffer + pSectionHeader->PointerToRawData))[j];
		}
    }

    // if we havent got our prefered base address, relocate
	UINTN pulBaseOffset = PeBuffer - pNtHeaders->OptionalHeader.ImageBase;
	pDataDir = &pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];

	if ((PeBuffer - pNtHeaders->OptionalHeader.ImageBase) && pDataDir && pDataDir->VirtualAddress != 0)
	{
		pRelocation = RVA(
			PIMAGE_BASE_RELOCATION,
			PeBuffer,
			pDataDir->VirtualAddress
		);

		do
		{
			PIMAGE_RELOC pRelocList = (PIMAGE_RELOC)(pRelocation + 1);

			do
			{
				if (pRelocList->type == IMAGE_REL_BASED_DIR64)
				{
					*(UINTN*)((UINT8*)PeBuffer + pRelocation->VirtualAddress + pRelocList->offset) += pulBaseOffset;
				}
				else if (pRelocList->type == IMAGE_REL_BASED_HIGHLOW)
				{
					*(UINTN*)((UINT8*)PeBuffer + pRelocation->VirtualAddress + pRelocList->offset) += (UINT32)pulBaseOffset;
				}
				else if (pRelocList->type == IMAGE_REL_BASED_HIGH)
				{
					*(UINTN*)((UINT8*)PeBuffer + pRelocation->VirtualAddress + pRelocList->offset) += HIWORD(pulBaseOffset);
				}
				else if (pRelocList->type == IMAGE_REL_BASED_LOW)
				{
					*(UINTN*)((UINT8*)PeBuffer + pRelocation->VirtualAddress + pRelocList->offset) += LOWORD(pulBaseOffset);
				}

				pRelocList++;

			} while ((UINT8*)pRelocList != (UINT8*)pRelocation + pRelocation->SizeOfBlock);

			pRelocation = (PIMAGE_BASE_RELOCATION)pRelocList;

		} while (pRelocation->VirtualAddress);
	}
	pNtHeaders->OptionalHeader.ImageBase = PeBuffer; // set the prefered base to the real base

    *BaseAddress = PeBuffer;

    return EFI_SUCCESS;
}

EFI_STATUS BtPeExecuteEntryPoint(EFI_SYSTEM_TABLE *SystemTable, EFI_PHYSICAL_ADDRESS PeAddress, SOA_KERNEL_INFORMATION* KernelInfo)
{
    EFI_STATUS Status = 0;
    PIMAGE_NT_HEADERS pNtHeaders;
	PIMAGE_DATA_DIRECTORY pDataDir;
	PIMAGE_BASE_RELOCATION pRelocation;
	PIMAGE_SECTION_HEADER pSectionHeader;
    PIMAGE_DOS_HEADER pDosHeader;

    pDosHeader = (PIMAGE_DOS_HEADER)PeAddress;

    if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
        return 1;

	pNtHeaders = RVA(
		PIMAGE_NT_HEADERS,
		PeAddress,
		((PIMAGE_DOS_HEADER)PeAddress)->e_lfanew
	);

    if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE)
        return 2;

    if (pNtHeaders->FileHeader.Machine != IMAGE_FILE_MACHINE_AMD64)
        return 3;

    if (pNtHeaders->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC)
        return 4;

    EFI_STATUS (*EntryPoint)(SOA_KERNEL_INFORMATION*) = RVA(UINTN, PeAddress, pNtHeaders->OptionalHeader.AddressOfEntryPoint);

    return EntryPoint(KernelInfo);
}

EFI_STATUS BtPeLoad(EFI_SYSTEM_TABLE *SystemTable, CHAR16* Path, EFI_PHYSICAL_ADDRESS* PeAddress)
{
    EFI_STATUS Status = 0;

    EFI_BOOT_SERVICES* BootServices = SystemTable->BootServices;

    VOID* FileBuffer = NULL;
    UINTN FileSize = 0;

    // Read PE file into memory from disk
    Status = BtLoadFile(SystemTable, Path, &FileBuffer, &FileSize);
    if (EFI_ERROR(Status))
	    return Status;

    Status = SystemTable->ConOut->OutputString(SystemTable->ConOut, L"[+] File loaded into memory\r\n");
	if (EFI_ERROR(Status))
	    return Status;

    // Map PE sections into memory and process relocations
    EFI_PHYSICAL_ADDRESS LoadedPeFile = 0;
    Status = MapPeSections(SystemTable, FileBuffer, &LoadedPeFile);
    if (Status != EFI_SUCCESS)
        return Status;

    Status = SystemTable->ConOut->OutputString(SystemTable->ConOut, L"[+] PE File Loaded\r\n");
	if (EFI_ERROR(Status))
	    return Status;

    *PeAddress = LoadedPeFile;

    Status = BootServices->FreePool(FileBuffer);
    if (EFI_ERROR(Status))
	    return Status;

    return EFI_SUCCESS;
}