.section ".text.boot"

.global _start

_start:
    // check core number
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbnz    x1, hang

    b hypervisor_init

hypervisor_init:

    // switch to EL1
    msr     sctlr_el1, xzr
    mrs     x1, hcr_el2
    orr     x1, x1, #(1 << 31) // set aarch64 for EL1
    orr     x1, x1, #(1 << 1) // SWIO
    msr     hcr_el2, x1
    mov     x1, #0b0101 // EL1h
    msr     spsr_el2, x1
    adr     x1, supervisor_init
    msr     elr_el2, x1

    mov     x1, #(3 << 20)    // Disable floating-point, SVE
    msr     cpacr_el1, x1

    eret

supervisor_init:
    // setup stack
    ldr     x1, =_start
    mov     sp, x1

    // setup init_array and jump to kmain
    bl      __clean_bss
    bl      __init_func
    bl      _ZN6Kernel5kmainEv

    // in case kmain returns
hang:
    wfe
    b       hang