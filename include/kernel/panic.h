#pragma once

#include "kernel/main.h"
#include "kernel/graphics/graphics.h"
#include "kernel/arch/amd64/halt.h"

VOID KePanic(struct DOE_GFX_BUFFER* GfxBuffer, DOE_STATUS StatusCode, BYTE* Message);