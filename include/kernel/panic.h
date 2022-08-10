#pragma once

#include "kernel/main.h"
#include "kernel/graphics/graphics.h"
#include "kernel/arch/amd64/halt.h"

VOID KePanic(DOE_STATUS StatusCode, BYTE* Message);