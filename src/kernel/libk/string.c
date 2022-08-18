#include "kernel/libk/string.h"

VOID* memset(VOID*  dst, LONG c, SIZE_T n)
{
    CHAR*  q   = dst;
    CHAR*  end = q + n;
    for (;;) {
        if (q >= end) break; *q++ = (CHAR) c;
        if (q >= end) break; *q++ = (CHAR) c;
        if (q >= end) break; *q++ = (CHAR) c;
        if (q >= end) break; *q++ = (CHAR) c;
    }
    return dst;
}