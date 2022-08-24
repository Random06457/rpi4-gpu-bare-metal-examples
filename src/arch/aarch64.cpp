#include "arch/aarch64.hpp"

namespace aarch64
{
#define MRS_FUNC2(type, funcName, reg)                                         \
    type get##funcName()                                                       \
    {                                                                          \
        type x;                                                                \
        MRS(x, reg);                                                           \
        return x;                                                              \
    }

#define MSR_FUNC2(type, funcName, reg)                                         \
    void set##funcName(type x)                                                 \
    {                                                                          \
        MSR(reg, x);                                                           \
    }

#define MRS_FUNC(type, reg) MRS_FUNC2(type, reg, reg);
#define MSR_FUNC(type, reg) MSR_FUNC2(type, reg, reg);

void halt()
{
    asm("hlt #0");
}

u64 getCurrEL()
{
    u64 curr_el;
    MRS(curr_el, CurrentEL);
    return curr_el >> 2;
}

u64 getCoreID()
{
    u64 mpidr_el1;
    MRS(mpidr_el1, mpidr_el1);
    return mpidr_el1 & 3;
}

MRS_FUNC(IDAA64MMFR0EL1, ID_AA64MMFR0_EL1)

MRS_FUNC2(u64, CBAR_EL1, s3_1_c15_c3_0)

MRS_FUNC(TTBR, TTBR0_EL1)
MRS_FUNC(TTBR, TTBR1_EL1)
MRS_FUNC(TTBR, TTBR0_EL2)
MRS_FUNC(TTBR, TTBR0_EL3)

MRS_FUNC(TCREL1, TCR_EL1)
MRS_FUNC(TCREL2, TCR_EL2)

MRS_FUNC(void*, VBAR_EL1)
MRS_FUNC(void*, VBAR_EL2)
MRS_FUNC(void*, VBAR_EL3)

MSR_FUNC(const void*, VBAR_EL1);
MSR_FUNC(const void*, VBAR_EL2);
MSR_FUNC(const void*, VBAR_EL3);

MRS_FUNC(u32, CNTFRQ_EL0);
MRS_FUNC(u64, CNTPCT_EL0);

MRS_FUNC(PAR, PAR_EL1);

void insSyncBarrier()
{
    __asm__ volatile("isb");
}

void dataSyncBarrier()
{
    __asm__ volatile("dsb ish");
}

PAR atS1E2R(u64 addr)
{
    PAR ret;
    asm volatile("at s1e2r, %0 \n"
                 "mrs %1, par_el1 \n"
                 : "=r"(ret)
                 : "r"(addr));

    return ret;
}
PAR atS1E2W(u64 addr)
{
    PAR ret;
    asm("at s1e2w, %0 \n"
        "mrs %1, par_el1 \n"
        : "=r"(ret)
        : "r"(addr));

    return ret;
}

const char* getExceptionClassName(ExceptionClass EC)
{
#define ENTRY(name)                                                            \
    case ExceptionClass::name: return #name;

    switch (EC)
    {
        ENTRY(Uncategorized)
        ENTRY(WFxTrap)
        ENTRY(CP15RTTrap)
        ENTRY(CP15RRTTrap)
        ENTRY(CP14RTTrap)
        ENTRY(CP14DTTrap)
        ENTRY(AdvSIMDFPAccessTrap)
        ENTRY(FPIDTrap)
        ENTRY(PACTrap)
        ENTRY(CP14RRTTrap)
        ENTRY(BTITrap)
        ENTRY(IllegalState)
        ENTRY(SupervisorCallA32)
        ENTRY(HypervisorCallA32)
        ENTRY(MonitorCallA32)
        ENTRY(SupervisorCallA64)
        ENTRY(HypervisorCallA64)
        ENTRY(MonitorCallA64)
        ENTRY(SystemRegisterTrap)
        ENTRY(SVEAccessTrap)
        ENTRY(ERetTrap)
        ENTRY(El3_ImplementationDefined)
        ENTRY(InstructionAbortLowerEl)
        ENTRY(InstructionAbortSameEl)
        ENTRY(PCAlignment)
        ENTRY(DataAbortLowerEl)
        ENTRY(DataAbortSameEl)
        ENTRY(SPAlignment)
        ENTRY(FPTrappedExceptionA32)
        ENTRY(FPTrappedExceptionA64)
        ENTRY(SError)
        ENTRY(BreakpointLowerEl)
        ENTRY(BreakpointSameEl)
        ENTRY(SoftwareStepLowerEl)
        ENTRY(SoftwareStepSameEl)
        ENTRY(WatchpointLowerEl)
        ENTRY(WatchpointSameEl)
        ENTRY(SoftwareBreakpointA32)
        ENTRY(VectorCatchA32)
        ENTRY(SoftwareBreakpointA64)
    }

    return "Invalid";

#undef ENTRY
}
};
