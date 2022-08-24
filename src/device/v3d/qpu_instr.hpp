#pragma once

#include "types.hpp"

namespace v3d
{

static constexpr u64 MUL_OP_NOP = 15;
static constexpr u64 ADD_OP_ADD = 56;
static constexpr u64 ADD_OP_OR = 182;
static constexpr u64 ADD_OP_NOP = 187;
static constexpr u64 ADD_OP_TMUWT = 187;
static constexpr u64 MUX_A = 6;
static constexpr u64 MUX_B = 7;
static constexpr u64 WADDR_NOP = 6;
static constexpr u64 WADDR_TMUD = 11;
static constexpr u64 WADDR_TMUA = 12;

enum QPUInstrSignal
{
    QPUInstrSignal_None,
    QPUInstrSignal_THRSW,
    QPUInstrSignal_LDUNIF,
    QPUInstrSignal_THRSW_LDUNIF,
    QPUInstrSignal_LDTMU,
    QPUInstrSignal_THRSW_LDTMU,
    QPUInstrSignal_LDTMU_LDUNIF,
    QPUInstrSignal_THRSW_LDTMU_LDUNIF,
    QPUInstrSignal_LDVARY,
    QPUInstrSignal_THRSW_LDVARY,
    QPUInstrSignal_LDVARY_LDUNIF,
    QPUInstrSignal_THRSW_LDVARY_LDUNIF,
    QPUInstrSignal_LDUNIFRF,
    QPUInstrSignal_THRSW_LDUNIFRF,
    QPUInstrSignal_SMIMM_LDVARY,
    QPUInstrSignal_SMIMM,
    QPUInstrSignal_LDTLB,
    QPUInstrSignal_LDTLBU,
    QPUInstrSignal_WRTMUC,
    QPUInstrSignal_THRSW_WRTMUC,
    QPUInstrSignal_LDVARY_WRTMUC,
    QPUInstrSignal_THRSW_LDVARY_WRTMUC,
    QPUInstrSignal_UCB,
    QPUInstrSignal_ROT,
    QPUInstrSignal_LDUNIFA,
    QPUInstrSignal_LDUNIFARF,
    QPUInstrSignal_SMIMM_LDTMU = 31,
};

struct QPUInstr
{
    QPUInstr(u64 raw) : raw(raw) {}

    union
    {
        u64 raw;
        struct
        {
            u64 raddr_b : 6; // read address B rf/imm
            u64 raddr_a : 6; // read address A rf
            u64 add_a : 3;   // ADD ra (register op a) r0..r5/a/b
            u64 add_b : 3;   // ADD rb (register op b) r0..r5/a/b
            u64 mul_a : 3;   // MUL ra (register op a) r0..r5/a/b
            u64 mul_b : 3;   // MUL rb (register op b) r0..r5/a/b
            u64 op_add : 8;  // ADD opcode
            u64 waddr_a : 6; // ADD write address rf/magic
            u64 waddr_m : 6; // MUL write address rf/magic
            u64 ma : 1;      // ADD magic write
            u64 mm : 1;      // MUL magic write
            u64 cond : 7;
            u64 sig : 5;    // signaling bits
            u64 op_mul : 6; // MUL opcode
        } alu;
        struct
        {
            u64 : 6;
            u64 : 6;
            u64 bdi : 2;
            u64 ub : 1;
            u64 bdu : 3;
            u64 : 3;
            u64 msfign : 2;
            u64 : 1;
            u64 addr_high : 8;
            u64 cond : 3;
            u64 addr_low : 21;
            u64 : 2;
            u64 : 6;
        } branch;
    };
};
static_assert(sizeof(QPUInstr) == 8);

}