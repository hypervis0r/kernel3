#include "kernel/arch/amd64/exception.h"

EXCEPTION_HANDLER(KeHalReservedException, "-", "Reserved Exception")
EXCEPTION_HANDLER(KeHalDivideByZeroException, "#DE", "Divide-By-Zero Exception")
EXCEPTION_HANDLER(KeHalDebugException, "#DB", "Debug Exception")
EXCEPTION_HANDLER(KeHalNmiException, "-", "Non-Maskable-Interrupt Exception")
EXCEPTION_HANDLER(KeHalBreakpointException, "#BP", "Breakpoint Exception")
EXCEPTION_HANDLER(KeHalOverflowException, "#OF", "Overflow Exception")
EXCEPTION_HANDLER(KeHalBoundRangeException, "#BR", "Bound-Range Exceeded Exception")
EXCEPTION_HANDLER(KeHalInvalidOpcodeException, "#UD", "Invalid Opcode Exception")
EXCEPTION_HANDLER(KeHalDeviceNotAvailableException, "#NM", "Device-Not-Available Exception")
EXCEPTION_HANDLER_WITH_ERROR_CODE(KeHalDoubleFaultException, "#DF", "Double Fault")
EXCEPTION_HANDLER_WITH_ERROR_CODE(KeHalInvalidTssException, "#TS", "Invalid TSS Exception")
EXCEPTION_HANDLER_WITH_ERROR_CODE(KeHalSegmentNotPresentException, "#NP", "Segment Not Present Exception")
EXCEPTION_HANDLER_WITH_ERROR_CODE(KeHalStackException, "#SS", "Stack Segment Fault")
EXCEPTION_HANDLER_WITH_ERROR_CODE(KeHalGeneralProtectionException, "#GP", "General Protection Fault")
EXCEPTION_HANDLER_WITH_ERROR_CODE(KeHalPageFaultException, "#PF", "Page Fault")
EXCEPTION_HANDLER(KeHalx87FloatingPointException, "#MF", "x87 Floating Point Exception")
EXCEPTION_HANDLER_WITH_ERROR_CODE(KeHalAlignmentCheckException, "#AC", "Alignment Check Exception")
EXCEPTION_HANDLER(KeHalMachineCheckException, "#MC", "Machine Check Exception")
EXCEPTION_HANDLER(KeHalSimdFloatingPointException, "#XM/#XF", "SIMD Floating Point Exception")
EXCEPTION_HANDLER_WITH_ERROR_CODE(KeHalControlProtectionException, "#CP", "Control Protection Exception")
EXCEPTION_HANDLER(KeHalHypervisorInjectionException, "#HV", "Hypervisor Injection Exception")
EXCEPTION_HANDLER(KeHalVmmCommunicationException, "#VC", "VMM Communication Exception")
EXCEPTION_HANDLER(KeHalSecurityException, "#SX", "Security Exception")

ULONG_PTR ExcIsrTable[EXC_COUNT] =
{
    KeHalDivideByZeroException,
    KeHalDebugException,
    KeHalNmiException,
    KeHalBreakpointException,
    KeHalOverflowException,
    KeHalBoundRangeException,
    KeHalInvalidOpcodeException,
    KeHalDeviceNotAvailableException,
    KeHalDoubleFaultException,
    KeHalReservedException,
    KeHalInvalidTssException,
    KeHalSegmentNotPresentException,
    KeHalStackException,
    KeHalGeneralProtectionException,
    KeHalPageFaultException,
    KeHalReservedException,
    KeHalx87FloatingPointException,
    KeHalAlignmentCheckException,
    KeHalMachineCheckException,
    KeHalSimdFloatingPointException,
    KeHalReservedException,
    KeHalControlProtectionException,
    KeHalReservedException,
    KeHalReservedException,
    KeHalReservedException,
    KeHalReservedException,
    KeHalReservedException,
    KeHalReservedException,
    KeHalHypervisorInjectionException,
    KeHalVmmCommunicationException,
    KeHalSecurityException,
    KeHalReservedException,
};