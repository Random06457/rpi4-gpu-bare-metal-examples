#include "exception_handler.hpp"
#include <cassert>
#include "bit_utils.hpp"
#include "kernel/logging.hpp"

static void dumpExceptionInfo(ExceptionFrameEL1* ex)
{
    for (size_t i = 0; i < 10; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            size_t k = j * 10 + i;
            LOG("x%02zd : 0x%016llX   ", k, ex->x[k]);
        }
        LOG("\n");
    }
    LOG("x30 : 0x%016llX\n", ex->x[30]);

    LOG("xzr      : 0x%016llX\n", ex->xzr);
    LOG("spsr_el1 : 0x%016llX\n", ex->spsr_el1);
    LOG("elr_el1  : 0x%016llX\n", ex->elr_el1);
    LOG("far_el1  : 0x%016llX\n", ex->far_el1);
    LOG("sp_el0   : 0x%016llX\n", ex->sp_el0);
    LOG("esr_el1  : 0x%016llX (EC=%s(%d), ISS=%d, ISS2=%d, IL=%d)\n",
        ex->esr_el1.raw, aarch64::getExceptionClassName(ex->esr_el1.EC),
        static_cast<u32>(ex->esr_el1.EC), ex->esr_el1.ISS, ex->esr_el1.ISS2,
        ex->esr_el1.IL);
    LOG("current_el : EL%lld\n", aarch64::getCurrEL());
}

extern "C" void el1_curr_el_sync_sp0(ExceptionFrameEL1* ex)
{
    (void)ex;
    assert(0 && "el1_curr_el_sync_sp0");
}

extern "C" void el1_curr_el_irq_sp0(ExceptionFrameEL1* ex)
{
    (void)ex;
    assert(0 && "el1_curr_el_irq_sp0");
}

extern "C" void el1_curr_el_fiq_sp0(ExceptionFrameEL1* ex)
{
    (void)ex;
    assert(0 && "el1_curr_el_fiq_sp0");
}

extern "C" void el1_curr_el_serror_sp0(ExceptionFrameEL1* ex)
{
    (void)ex;
    assert(0 && "el1_curr_el_serror_sp0");
}

static const char* dataAbortCodeMessage(u64 code)
{
    switch (code)
    {
        case 0b000000:
            return "Address size fault, level 0 of translation or translation "
                   "table base register.";
        case 0b000001: return "Address size fault, level 1.";
        case 0b000010: return "Address size fault, level 2.";
        case 0b000011: return "Address size fault, level 3.";
        case 0b000100: return "Translation fault, level 0.";
        case 0b000101: return "Translation fault, level 1.";
        case 0b000110: return "Translation fault, level 2.";
        case 0b000111: return "Translation fault, level 3.";
        case 0b001001: return "Access flag fault, level 1.";
        case 0b001010: return "Access flag fault, level 2.";
        case 0b001011: return "Access flag fault, level 3.";
        case 0b001000: return "Access flag fault, level 0.";
        case 0b001100: return "Permission fault, level 0.";
        case 0b001101: return "Permission fault, level 1.";
        case 0b001110: return "Permission fault, level 2.";
        case 0b001111: return "Permission fault, level 3.";
        case 0b010000:
            return "Synchronous External abort, not on translation table walk "
                   "or hardware update of translation table.";
        case 0b010001:
            return "Synchronous Tag Check Fault. When FEAT_MTE2 is implemented";
        case 0b010011:
            return "Synchronous External abort on translation table walk or "
                   "hardware update of translation table, level -1.";
        case 0b010100:
            return "Synchronous External abort on translation table walk or "
                   "hardware update of translation table, level 0.";
        case 0b010101:
            return "Synchronous External abort on translation table walk or "
                   "hardware update of translation table, level 1.";
        case 0b010110:
            return "Synchronous External abort on translation table walk or "
                   "hardware update of translation table, level 2.";
        case 0b010111:
            return "Synchronous External abort on translation table walk or "
                   "hardware update of translation table, level 3.";
        case 0b011000:
            return "Synchronous parity or ECC error on memory access, not on "
                   "translation table walk.";
        case 0b011011:
            return "Synchronous parity or ECC error on memory access on "
                   "translation table walk or hardware update of translation "
                   "table, level -1.";
        case 0b011100:
            return "Synchronous parity or ECC error on memory access on "
                   "translation table walk or hardware update of translation "
                   "table, level 0.";
        case 0b011101:
            return "Synchronous parity or ECC error on memory access on "
                   "translation table walk or hardware update of translation "
                   "table, level 1.";
        case 0b011110:
            return "Synchronous parity or ECC error on memory access on "
                   "translation table walk or hardware update of translation "
                   "table, level 2.";
        case 0b011111:
            return "Synchronous parity or ECC error on memory access on "
                   "translation table walk or hardware update of translation "
                   "table, level 3.";
        case 0b100001: return "Alignment fault.";
        case 0b101001: return "Address size fault, level -1.";
        case 0b101011: return "Translation fault, level -1.";
        case 0b110000: return "TLB conflict abort.";
        case 0b110001:
            return "Unsupported atomic hardware update fault. When FEAT_HAFDBS "
                   "is implemented";
        case 0b110100: return "IMPLEMENTATION DEFINED fault (Lockdown).";
        case 0b110101:
            return "IMPLEMENTATION DEFINED fault (Unsupported Exclusive or "
                   "Atomic access).";
        default: return "Invalid";
    };
}

extern "C" void el1_curr_el_sync_spx(ExceptionFrameEL1* ex)
{
    LOG("el1_curr_el_sync_spx\n");
    dumpExceptionInfo(ex);

    switch (ex->esr_el1.EC)
    {
        case aarch64::ExceptionClass::SupervisorCallA64:
            LOG("Handling SVC %d...\n", ex->esr_el1.ISS);
            break;
        case aarch64::ExceptionClass::DataAbortSameEl:
        {
            // https://developer.arm.com/documentation/ddi0595/2021-06/AArch64-Registers/ESR-EL1--Exception-Syndrome-Register--EL1-?lang=en#fieldset_0-24_0_15
            LOG("PC : 0x%016llx\n", ex->elr_el1);
            LOG("Data Abort cause by %s at address 0x%016llx\n",
                (ex->esr_el1.ISS & (1 << 5)) ? "Write" : "Read", ex->far_el1);

            u64 code = bits::shiftR<u64>(ex->esr_el1.ISS, 6, 0);
            const char* code_msg = dataAbortCodeMessage(code);
            LOG("Data Fault Status Code : %llx(\"%s\")\n", code, code_msg);
            assert(0);
            break;
        }

        default: assert(0 && "el1_curr_el_sync_spx"); break;
    }
}

extern "C" void el1_curr_el_irq_spx(ExceptionFrameEL1* ex)
{
    (void)ex;
    assert(0 && "el1_curr_el_irq_spx");
}

extern "C" void el1_curr_el_fiq_spx(ExceptionFrameEL1* ex)
{
    (void)ex;
    assert(0 && "el1_curr_el_fiq_spx");
}

extern "C" void el1_curr_el_serror_spx(ExceptionFrameEL1* ex)
{
    (void)ex;
    assert(0 && "el1_curr_el_serror_spx");
}

extern "C" void el1_lower_el_a64_sync(ExceptionFrameEL1* ex)
{
    (void)ex;
    assert(0 && "el1_lower_el_a64_sync");
}

extern "C" void el1_lower_el_a64_irq(ExceptionFrameEL1* ex)
{
    (void)ex;
    assert(0 && "el1_lower_el_a64_irq");
}

extern "C" void el1_lower_el_a64_fiq(ExceptionFrameEL1* ex)
{
    (void)ex;
    assert(0 && "el1_lower_el_a64_fiq");
}

extern "C" void el1_lower_el_a64_serror(ExceptionFrameEL1* ex)
{
    (void)ex;
    assert(0 && "el1_lower_el_a64_serror");
}
