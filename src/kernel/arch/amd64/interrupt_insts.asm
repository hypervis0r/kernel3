global KeHalIdtLoad
global KeHalDisableInterrupts
global KeHalEnableInterrupts
global KeHalTriggerSoftwareInterrupt

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

KeHalTriggerSoftwareInterrupt:
    int3
    ret