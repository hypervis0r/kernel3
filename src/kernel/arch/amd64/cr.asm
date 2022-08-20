global KeHalCr0Load
global KeHalCr0Read

global KeHalCr2Load
global KeHalCr2Read

global KeHalCr3Load
global KeHalCr3Read

global KeHalCr4Load
global KeHalCr4Read

global KeHalCr8Load
global KeHalCr8Read

section .data

section .text

; CR0 Access
KeHalCr0Load:
    mov cr0, rcx
    ret
KeHalCr0Read:
    mov rax, cr0
    ret

; CR2 Access
KeHalCr2Load:
    mov cr0, rcx
    ret
KeHalCr2Read:
    mov rax, cr0
    ret

; CR3 Access
KeHalCr3Load:
    mov cr0, rcx
    ret
KeHalCr3Read:
    mov rax, cr0
    ret

; CR4 Access
KeHalCr4Load:
    mov cr0, rcx
    ret
KeHalCr4Read:
    mov rax, cr0
    ret

; CR8 Access
KeHalCr8Load:
    mov cr0, rcx
    ret
KeHalCr8Read:
    mov rax, cr0
    ret
