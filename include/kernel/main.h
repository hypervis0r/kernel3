#pragma once

// Lets keep this at the bottom for now
#include "kernel/doedef.h"

#include "boot/loader.h"

#include "kernel/graphics/graphics.h"
#include "kernel/graphics/psf_font.h"

#include "kernel/tty.h"

#include "kernel/arch/amd64/port.h"
#include "kernel/arch/amd64/i8259.h"
#include "kernel/arch/amd64/interrupt.h"

#include "kernel/drivers/i8042.h" // PS/2 Keyboard Controller
#include "kernel/drivers/i8253.h" // PIT

#include "kernel/panic.h"

//#include "boot/boot.h"
//#include "boot/graphics.h"
