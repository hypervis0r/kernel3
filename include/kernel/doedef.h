#ifndef _KERNEL_DOEDEF_H
#define _KERNEL_DOEDEF_H

#include <stdint.h>

typedef uint8_t  BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef uint64_t QWORD;

typedef int8_t  CHAR;
typedef int16_t SHORT;
typedef int32_t LONG;
typedef int64_t LONGLONG;

// We use fast32 for performance reasons, platforms could upgrade
// to 64 for performance gain
typedef int_fast32_t BOOL;

#ifdef TRUE
#undef TRUE
#endif
#define TRUE 1

#ifdef FALSE
#undef FALSE
#endif
#define FALSE 0

#ifdef __amd64__
typedef uint64_t SIZE_T;
#else
#error "Unsupported processor type"
#endif

typedef SIZE_T ULONG_PTR;

#ifdef VOID
#undef VOID
#endif
typedef void VOID;

typedef DWORD DOE_STATUS;

#define DOE_SUCCESS 0
#define DOE_ERROR 1

typedef DWORD ARGB_COLOR;

#ifdef __GNUC__

#undef PACKED
#define PACKED __attribute__((packed))

#define INTERRUPT_HANDLER __attribute__((interrupt))
#define INLINE inline

#else
#error "Unsupported compiler, see kernel/doedef.h for things that need definitions"
#endif

#endif