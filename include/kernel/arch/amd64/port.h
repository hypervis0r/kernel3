#pragma once

#include "kernel/doedef.h"

BYTE KeHalPortRead(DWORD Port);
VOID KeHalPortWrite(DWORD Port, BYTE Byte);