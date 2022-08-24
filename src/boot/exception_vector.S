save_regs:

    sub sp, sp, #0x130

    stp x0,  x1,  [sp, #0x000]
    stp x2,  x3,  [sp, #0x010]
    stp x4,  x5,  [sp, #0x020]
    stp x6,  x7,  [sp, #0x030]
    stp x8,  x9,  [sp, #0x040]
    stp x10, x11, [sp, #0x050]
    stp x12, x13, [sp, #0x060]
    stp x14, x15, [sp, #0x070]
    stp x16, x17, [sp, #0x080]
    stp x18, x19, [sp, #0x090]
    stp x20, x21, [sp, #0x0A0]
    stp x22, x23, [sp, #0x0B0]
    stp x24, x25, [sp, #0x0C0]
    stp x26, x27, [sp, #0x0D0]

    ldp x0, x1,   [sp, #0x130-0x10] // x29, x30

    stp x28, x0, [sp, #0x0E0]
    stp x1, xzr, [sp, #0x0F0]



    mrs x0, spsr_el1 // Saved Program Status Register
    mrs x1, elr_el1 // Exception Link Register
    mrs x2, esr_el1 // Exception Syndrom Register
    mrs x3, far_el1 // Fault Address Register
    mrs x4, sp_el0 // Stack Pointer EL0

    stp x0, x1, [sp, #0x100]
    stp x2, x3, [sp, #0x110]
    stp x4, x5, [sp, #0x120]

    ret


restore_regs:

    ldp x0, x1, [sp, #0x100]
    ldp x2, x3, [sp, #0x110]
    ldp x4, x5, [sp, #0x120]

    msr spsr_el1, x0 // Saved Program Status Register
    msr elr_el1, x1 // Exception Link Register
    msr esr_el1, x2 // Exception Syndrom Register
    msr far_el1, x3 // Fault Address Register
    msr sp_el0, x4 // Stack Pointer EL0

    ldp x0,  x1,  [sp, #0x000]
    ldp x2,  x3,  [sp, #0x010]
    ldp x4,  x5,  [sp, #0x020]
    ldp x6,  x7,  [sp, #0x030]
    ldp x8,  x9,  [sp, #0x040]
    ldp x10, x11, [sp, #0x050]
    ldp x12, x13, [sp, #0x060]
    ldp x14, x15, [sp, #0x070]
    ldp x16, x17, [sp, #0x080]
    ldp x18, x19, [sp, #0x090]
    ldp x20, x21, [sp, #0x0A0]
    ldp x22, x23, [sp, #0x0B0]
    ldp x24, x25, [sp, #0x0C0]
    ldp x26, x27, [sp, #0x0D0]
    ldp x28, x29, [sp, #0x0E0]
    ldp x30, xzr, [sp, #0x0F0]

    add sp, sp, #0x130

    eret


.macro HANDLE_EXCEPTION name

    stp x29, x30, [sp, #-0x10] // save LR / FP here
    bl save_regs

    mov x0, sp
    bl \name

    b restore_regs

    .balign 0x80
.endm


.balign 0x800

.global exception_vector_el1
exception_vector_el1:

    // current EL, SP0
    // sync
    HANDLE_EXCEPTION el1_curr_el_sync_sp0

    // IRQ
    HANDLE_EXCEPTION el1_curr_el_irq_sp0

    // FIQ
    HANDLE_EXCEPTION el1_curr_el_fiq_sp0

    // SError
    HANDLE_EXCEPTION el1_curr_el_serror_sp0


    // current EL, current SP
    // sync
    HANDLE_EXCEPTION el1_curr_el_sync_spx

    // IRQ
    HANDLE_EXCEPTION el1_curr_el_irq_spx

    // FIQ
    HANDLE_EXCEPTION el1_curr_el_fiq_spx

    // SError
    HANDLE_EXCEPTION el1_curr_el_serror_spx


    // lower EL, current SP
    // sync
    HANDLE_EXCEPTION el1_lower_el_a64_sync

    // IRQ
    HANDLE_EXCEPTION el1_lower_el_a64_irq

    // FIQ
    HANDLE_EXCEPTION el1_lower_el_a64_fiq

    // SError
    HANDLE_EXCEPTION el1_lower_el_a64_serror
