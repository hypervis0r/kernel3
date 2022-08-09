#include "boot/file.h"

EFI_STATUS BtFileGetInfo(EFI_BOOT_SERVICES* BootServices, EFI_FILE_PROTOCOL* FileHandle, EFI_FILE_INFO** FileInfo)
{
    EFI_STATUS Status = 0;
    EFI_GUID FileInfoGuid = EFI_FILE_INFO_ID;
    UINTN InfoBufferSize = 0;
    EFI_FILE_INFO* TempFileInfo = NULL;

    Status = FileHandle->GetInfo(FileHandle, &FileInfoGuid, &InfoBufferSize, NULL);
    if (Status == EFI_BUFFER_TOO_SMALL)
        ; // Do Nothing
    else if (EFI_ERROR(Status))
        return Status;

    Status = BootServices->AllocatePool(EfiLoaderData, InfoBufferSize, &TempFileInfo);
    if (EFI_ERROR(Status))
        return Status;

    Status = FileHandle->GetInfo(FileHandle, &FileInfoGuid, &InfoBufferSize, TempFileInfo);
    if (EFI_ERROR(Status))
    {
        BootServices->FreePool(TempFileInfo);
        return Status;
    }

    *FileInfo = TempFileInfo;

    return EFI_SUCCESS;
}

EFI_STATUS BtLoadFile(EFI_SYSTEM_TABLE* SystemTable, CHAR16* Path, VOID** Output, UINTN* Size)
{
    EFI_STATUS Status = 0;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* SfsProtocol = NULL;
    EFI_FILE_PROTOCOL* RootDirectory = NULL;
    EFI_FILE_PROTOCOL* FileHandle = NULL;

    EFI_BOOT_SERVICES* BootServices = SystemTable->BootServices;

    // Get SFS Protocol
    EFI_GUID SfsGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    Status = BootServices->LocateProtocol(&SfsGuid, NULL, &SfsProtocol);
    if (EFI_ERROR(Status))
	    return Status;

    Status = SfsProtocol->OpenVolume(SfsProtocol, &RootDirectory);
    if (EFI_ERROR(Status))
	    return Status;

    Status = RootDirectory->Open(RootDirectory, &FileHandle, Path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY | EFI_FILE_SYSTEM);
    if (EFI_ERROR(Status))
	    return Status;

    EFI_FILE_INFO* FileInfo = NULL;
    Status = BtFileGetInfo(BootServices, FileHandle, &FileInfo);
    if (EFI_ERROR(Status))
	    return Status;

    UINTN FileSize = FileInfo->FileSize;

    Status = BootServices->FreePool(FileInfo);
    if (EFI_ERROR(Status))
	    return Status;

    VOID* FileBuffer = NULL;
    Status = BootServices->AllocatePool(EfiLoaderData, FileSize, &FileBuffer);
    if (EFI_ERROR(Status))
	    return Status;

    Status = FileHandle->Read(FileHandle, &FileSize, FileBuffer);
    if (EFI_ERROR(Status))
	    return Status;

    *Output = FileBuffer;
    *Size = FileSize;

    return EFI_SUCCESS;
}