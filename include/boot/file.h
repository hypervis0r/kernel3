#pragma once

#include <Uefi.h>
#include <Protocol/SimpleFileSystem.h>
#include <Guid/FileInfo.h>
#include <Library/PrintLib.h>

EFI_STATUS BtLoadFile(EFI_SYSTEM_TABLE* SystemTable, CHAR16* Path, VOID** Output, UINTN* Size);