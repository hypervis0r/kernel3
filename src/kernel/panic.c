#include "kernel/panic.h"

VOID KePanic(DOE_STATUS StatusCode, BYTE* Message)
{
    KeTermClear(&g_MainTty, 0x00000000);

    KeTermPutString(&g_MainTty, Message, 0xFFFF0000, 0x00000000);

    for (;;) KeHalHaltProcessor();
}