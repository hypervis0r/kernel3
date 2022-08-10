global KeHalPortRead
global KeHalPortWrite

section .data

section .text

KeHalPortRead:
    mov rdx, rdi
    in ax, dx
    ret

KeHalPortWrite:
    mov rdx, rdi
    mov rax, rsi
    out dx, ax
    ret