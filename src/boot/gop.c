#include "boot/gop.h"

EFI_STATUS BtGopGetFramebuffer(
    EFI_SYSTEM_TABLE* SystemTable,
    // TODO: This is dumb and will break on certain firmwares 
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION** GopModeInfo, 
    EFI_PHYSICAL_ADDRESS* FramebufferAddress, 
    UINTN* FramebufferSize)
{
    EFI_STATUS Status = 0;

    EFI_GUID GopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL* GopProtocol = NULL;
    
    Status = SystemTable->BootServices->LocateProtocol(&GopGuid, NULL, &GopProtocol);
    if (EFI_ERROR(Status))
        return Status;

    *FramebufferAddress = GopProtocol->Mode->FrameBufferBase;
    *FramebufferSize = GopProtocol->Mode->FrameBufferSize;
    *GopModeInfo = GopProtocol->Mode->Info;

    return EFI_SUCCESS;
}