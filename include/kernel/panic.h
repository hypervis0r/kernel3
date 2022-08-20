#pragma once

#include "kernel/doedef.h"
#include "kernel/tty.h"
#include "kernel/arch/amd64/halt.h"

VOID KePanic(DOE_STATUS StatusCode, BYTE* Message);