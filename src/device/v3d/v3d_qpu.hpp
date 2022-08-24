#pragma once

#include "types.hpp"

namespace v3d
{

namespace qpu
{

// CSD : compute shader dispatch
void executeCSD(u32 code, u32 uniform);

// TFU : texture formatting unit
void executeTFU(u32 tex, u32 src_stride);

void executeBin(u32 start, u32 end, u32 tile_addr, u32 tile_size,
                u32 tile_state);

void executeRender(u32 start, u32 end);

}

}
