global KeHalIdtLoad
global KeHalDisableInterrupts
global KeHalEnableInterrupts

section .data

section .text

KeHalIdtLoad:
    lidt [rdi]
    ret

KeHalDisableInterrupts:
    cli
    ret

KeHalEnableInterrupts:
    sti
    ret