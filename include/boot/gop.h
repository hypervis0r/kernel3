#pragma once

#include <Uefi.h>
#include <Protocol/GraphicsOutput.h>

EFI_STATUS BtGopGetFramebuffer(
    EFI_SYSTEM_TABLE* SystemTable,
    // TODO: This is dumb and will break on certain firmwares 
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION** GopModeInfo, 
    EFI_PHYSICAL_ADDRESS* FramebufferAddress, 
    UINTN* FramebufferSize);