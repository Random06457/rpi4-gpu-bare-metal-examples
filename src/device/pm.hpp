#pragma once

#include "bit_utils.hpp"
#include "mmio.hpp"

namespace pm
{
static constexpr u32 PM_V3DRSTN = bits::bit<u32>(6);
static constexpr u32 PM_PASSWORD = 0x5A000000;
static constexpr u32 ASB_REQ_STOP = bits::bit<u32>(0);
static constexpr u32 ASB_ACK = bits::bit<u32>(1);
static constexpr u32 ASB_EMPTY = bits::bit<u32>(2);
static constexpr u32 ASB_FULL = bits::bit<u32>(3);

inline void write(intptr_t addr, u32 value)
{
    mmio::write(addr, PM_PASSWORD | value);
}
inline u32 read(intptr_t addr)
{
    return mmio::read(addr);
}

void powerOnV3D();

};