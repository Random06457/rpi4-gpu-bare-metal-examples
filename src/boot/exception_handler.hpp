#pragma once

#include "types.hpp"
#include "arch/aarch64.hpp"

struct ExceptionFrameEL1
{
    u64 x[31];
    u64 xzr;
    u64 spsr_el1;
    u64 elr_el1;
    aarch64::ESR esr_el1;
    u64 far_el1;
    u64 sp_el0;
} PACKED;