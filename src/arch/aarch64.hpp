#pragma once

#include <arm_sve.h>
#include "types.hpp"

#define MRS(x, name) __asm__ volatile("mrs %0, " #name : "=r"(x))
#define MSR(name, x) __asm__ volatile("msr " #name ", %0" : : "r"(x))

namespace aarch64
{
struct TCREL1
{
    union
    {
        u64 raw;
        struct
        {
            u64 T0SZ : 6;
            u64 : 1;
            u64 EPD0 : 1;
            u64 IRGN0 : 2;
            u64 ORGN0 : 2;
            u64 SH0 : 2;
            u64 TG0 : 1;
            u64 : 1;
            u64 T1SZ : 6;
            u64 A1 : 1;
            u64 EPD1 : 1;
            u64 IRGN1 : 2;
            u64 ORGN1 : 2;
            u64 SH1 : 2;
            u64 TG1 : 1;
            u64 : 1;
            u64 IPS : 3;
            u64 : 1;
            u64 AS : 1;
            u64 TBI0 : 1;
            u64 TBI1 : 1;
            u64 : 25;
        };
    };
};
static_assert(sizeof(TCREL1) == sizeof(u64));

struct TCREL2
{
    union
    {
        u64 raw;
        struct
        {
            u64 T0SZ : 6;
            u64 : 2;
            u64 IRGN0 : 2;
            u64 ORGN0 : 2;
            u64 SH0 : 2;
            u64 TG0 : 1;
            u64 : 1;
            u64 PS : 3;
            u64 : 1;
            u64 TBI : 1;
            u64 : 2;
            u64 : 1;
            u64 : 7;
            u64 : 1;
            u64 : 32;
        };
    };
};
static_assert(sizeof(TCREL2) == sizeof(u64));

struct TTBR
{
    union
    {
        struct
        {
            u64 CnP : 1;
            u64 BADDR : 47;
            u64 ASID : 16;
        };
        u64 raw;
    };
};
static_assert(sizeof(TTBR) == sizeof(u64));

enum class ExceptionClass : u64
{
    Uncategorized = 0x0,
    WFxTrap = 0x1,
    CP15RTTrap = 0x3,
    CP15RRTTrap = 0x4,
    CP14RTTrap = 0x5,
    CP14DTTrap = 0x6,
    AdvSIMDFPAccessTrap = 0x7,
    FPIDTrap = 0x8,
    PACTrap = 0x9,
    CP14RRTTrap = 0xC,
    BTITrap = 0xD,
    IllegalState = 0xE,
    SupervisorCallA32 = 0x11,
    HypervisorCallA32 = 0x12,
    MonitorCallA32 = 0x13,
    SupervisorCallA64 = 0x15,
    HypervisorCallA64 = 0x16,
    MonitorCallA64 = 0x17,
    SystemRegisterTrap = 0x18,
    SVEAccessTrap = 0x19,
    ERetTrap = 0x1A,
    El3_ImplementationDefined = 0x1F,
    InstructionAbortLowerEl = 0x20,
    InstructionAbortSameEl = 0x21,
    PCAlignment = 0x22,
    DataAbortLowerEl = 0x24,
    DataAbortSameEl = 0x25,
    SPAlignment = 0x26,
    FPTrappedExceptionA32 = 0x28,
    FPTrappedExceptionA64 = 0x2C,
    SError = 0x2F,
    BreakpointLowerEl = 0x30,
    BreakpointSameEl = 0x31,
    SoftwareStepLowerEl = 0x32,
    SoftwareStepSameEl = 0x33,
    WatchpointLowerEl = 0x34,
    WatchpointSameEl = 0x35,
    SoftwareBreakpointA32 = 0x38,
    VectorCatchA32 = 0x3A,
    SoftwareBreakpointA64 = 0x3C,
};

// Exception Syndrom Register
struct ESR
{
    union
    {
        u64 raw;
        struct
        {
            u64 ISS : 25;
            u64 IL : 1;
            ExceptionClass EC : 6;
            u64 ISS2 : 5;
            u64 : 27;
        };
    };
};
static_assert(sizeof(ESR) == sizeof(u64));

struct IDAA64MMFR0EL1
{
    union
    {
        u64 raw;
        struct
        {
            u64 PARange : 4;
            u64 ASIDBits : 4;
            u64 BigEnd : 4;
            u64 SNSMem : 4;
            u64 BigEndEL0 : 4;
            u64 TGran16 : 4;
            u64 TGran64 : 4;
            u64 TGran4 : 4;
            u64 TGran16_2 : 4;
            u64 TGran64_2 : 4;
            u64 TGran4_2 : 4;
            u64 ExS : 4;
            u64 : 8;
            u64 FGT : 4;
            u64 ECV : 4;
        };
    };
};
static_assert(sizeof(IDAA64MMFR0EL1) == sizeof(u64));

struct PAR
{
    union
    {
        u64 raw;
        struct
        {
            u64 F : 1;
        };
        struct
        {
            u64 F : 1;
            u64 : 6;
            u64 SH : 2;
            u64 NS : 1;
            u64 : 1; // impl defined
            u64 : 1;
            u64 PA : 40;
            u64 : 4;
            u64 ATTR : 8;
        } success;
        struct
        {
            u64 F : 1;
            u64 FST : 6; // Fault Status Code
            u64 : 1;
            u64 PTW : 1; // Stage 2 fault during stage 1 translation table walk
            u64 S : 1;   // Stage
            u64 : 1;
            u64 : 1;
            u64 : 36;
            u64 : 4;
            u64 : 4;
            u64 : 8;
        } fail;
    };
};
static_assert(sizeof(PAR) == sizeof(u64));

template<int N, int M>
struct PageEntry
{
    union
    {
        u64 raw;
        struct
        {
            u64 isValid : 1;
            u64 isTable : 1;
        };
        struct
        {
            u64 : 2;
            u64 attrLo : 10;
            u64 : N - 12;
            u64 outAddr : 48 - N;
            u64 : 4;
            u64 attrHi : 12;
        } block;
        struct
        {
            u64 : M;
            u64 tableAddr : 48 - M;
            u64 : 4;
            u64 : 5;
            u64 PXNTable : 1;
            u64 XNTable : 1;
            u64 APTable : 2;
            u64 NSTable : 1;
        } table;
    };
};
typedef PageEntry<30, 12> PageEntry4KB_L1;
typedef PageEntry<21, 12> PageEntry4KB_L2;
typedef PageEntry<25, 14> PageEntry16KB;
typedef PageEntry<29, 16> PageEntry64KB;

static_assert(sizeof(PageEntry4KB_L1) == sizeof(u64));
static_assert(sizeof(PageEntry4KB_L2) == sizeof(u64));
static_assert(sizeof(PageEntry16KB) == sizeof(u64));
static_assert(sizeof(PageEntry64KB) == sizeof(u64));

u64 getCurrEL();
void halt();
u64 getCoreID();
u64 getCBAR_EL1();

TTBR getTTBR0_EL1();
TTBR getTTBR1_EL1();
TTBR getTTBR0_EL2();
TTBR getTTBR1_EL2();
TTBR getTTBR0_EL3();

TCREL1 getTCR_EL1();
TCREL2 getTCR_EL2();

PAR getPAR_EL1();

// todo:
IDAA64MMFR0EL1 getID_AA64MMFR0_EL1();

void* getVBAR_EL1();
void* getVBAR_EL2();
void* getVBAR_EL3();
void setVBAR_EL1(const void* addr);
void setVBAR_EL2(const void* addr);
void setVBAR_EL3(const void* addr);

PAR atS1E2R(u64 addr);
PAR atS1E2W(u64 addr);

u32 getCNTFRQ_EL0();
u64 getCNTPCT_EL0();

void insSyncBarrier();
void dataSyncBarrier();

const char* getExceptionClassName(ExceptionClass EC);
}
