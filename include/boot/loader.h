#pragma once

#include <Uefi.h>

#include "boot/file.h"

EFI_STATUS BtLoadPeFile(EFI_SYSTEM_TABLE *SystemTable, CHAR16* Path);