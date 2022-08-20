#pragma once

#include "kernel/doedef.h"

#include "kernel/arch/amd64/interrupt.h"
#include "kernel/panic.h"

#define EXC_COUNT 32

#define EXCEPTION_HANDLER(FunctionName, Code, Message) \ 
    INTERRUPT_HANDLER(FunctionName) \
    { \
        KePanic(0, Message); \
    }

#define EXCEPTION_HANDLER_WITH_ERROR_CODE(FunctionName, Cd, Message) \ 
    INTERRUPT_HANDLER_WITH_ERROR_CODE(FunctionName) \
    { \
        KePanic(ErrorCode, Message); \
    }

extern ULONG_PTR ExcIsrTable[EXC_COUNT];