#ifndef _KERNEL_DOEDEF_H
#define _KERNEL_DOEDEF_H

#include <Uefi.h>
#include <stdint.h>

typedef uint8_t  BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef uint64_t QWORD;

typedef int8_t  CHAR;
typedef int16_t SHORT;
typedef int32_t LONG;
typedef int64_t LONGLONG;

typedef BYTE BOOL;

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

#endif