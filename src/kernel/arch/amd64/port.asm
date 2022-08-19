global KeHalPortRead
global KeHalPortWrite

section .data

section .text

KeHalPortRead:
    mov edx, ecx
    in al, dx
    ret

KeHalPortWrite:
    mov eax, edx
    mov edx, ecx
    out dx, al
    ret