#pragma once

#include "kernel/main.h"

BYTE KeHalPortRead(DWORD Port);
VOID KeHalPortWrite(DWORD Port, BYTE Byte);