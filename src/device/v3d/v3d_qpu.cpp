#include "v3d_qpu.hpp"
#include "bit_utils.hpp"
#include "device/mmio.hpp"
#include "kernel/logging.hpp"

namespace v3d
{

namespace qpu
{

static void logV3DCSD()
{
    u32 status = mmio::read(mmio::V3D_CSD_STATUS);
    bool have_queue_dispatch = status & 1;
    bool have_current_dispatch = status & 2;
    bool num_active = bits::shiftR<u32>(status, 2, 2);
    bool num_completed = bits::shiftR<u32>(status, 8, 4);

    LOG("CSD Status: num active : %d; num completed : %d;\n", num_active,
        num_completed);
}

static size_t getCSDCompletedJob()
{
    return bits::shiftR<u32>(mmio::read(mmio::V3D_CSD_STATUS), 8, 4);
}

static void flushCache()
{
    static constexpr u32 V3D_L2TCACTL_TMUWCF = bits::bit<u32>(8);
    static constexpr u32 V3D_L2TCACTL_L2TFLS = bits::bit<u32>(0);
    static constexpr u32 V3D_L2TCACTL_FLM_CLEAN = bits::shiftL<u32>(2, 2, 1);

    // TMU write combiner flush
    mmio::write(mmio::V3D_CTL_L2TCACTL, V3D_L2TCACTL_TMUWCF);
    while (mmio::read(mmio::V3D_CTL_L2TCACTL) & V3D_L2TCACTL_TMUWCF)
        ;

    // clean l2t (write dirty bits + clear)
    mmio::write(mmio::V3D_CTL_L2TCACTL,
                V3D_L2TCACTL_L2TFLS | V3D_L2TCACTL_FLM_CLEAN);

    while (mmio::read(mmio::V3D_CTL_L2TCACTL) & V3D_L2TCACTL_L2TFLS)
        ;
}

void executeCSD(u32 code, u32 uniform)
{
    u32 num_wg_x = 16;
    u32 num_wg_y = 1;
    u32 num_wg_z = 1;
    u32 wg_size = num_wg_x * num_wg_y * num_wg_z;
    u32 wg_per_sg = 16;

    LOG("code address: 0x%08x\n", code);
    LOG("uniform address: 0x%08x\n", uniform);

    u32 num_batch = 1;
    mmio::write(mmio::V3D_CSD_QUEUED_CFG1, num_wg_y << 16);
    mmio::write(mmio::V3D_CSD_QUEUED_CFG2, num_wg_z << 16);
    mmio::write(mmio::V3D_CSD_QUEUED_CFG3,
                ((((wg_per_sg * wg_size + 16u - 1u) / 16u) - 1u) << 12) |
                        (wg_per_sg << 8) ||
                    (wg_size));
    mmio::write(mmio::V3D_CSD_QUEUED_CFG4, num_batch - 1);
    mmio::write(mmio::V3D_CSD_QUEUED_CFG5, code);
    mmio::write(mmio::V3D_CSD_QUEUED_CFG6, uniform);

    LOG("Launching V3D CSD Task\n\n");

    size_t count = getCSDCompletedJob();

    mmio::write(mmio::V3D_CSD_QUEUED_CFG0, num_wg_x << 16);

    while (getCSDCompletedJob() <= count)
        ;

    LOG("Flushing cache...\n");

    flushCache();
}

void executeBin(u32 start, u32 end, u32 tile_addr, u32 tile_size,
                u32 tile_state)
{
    static constexpr u32 V3D_CLE_CT0QTS_ENABLE = bits::bit<u32>(1);
    if (tile_addr)
    {
        mmio::write(mmio::V3D_CLE_CT0QMA, tile_addr);
        mmio::write(mmio::V3D_CLE_CT0QMS, tile_size);
    }
    if (tile_state)
        mmio::write(mmio::V3D_CLE_CT0QTS, tile_state | V3D_CLE_CT0QTS_ENABLE);

    mmio::write(mmio::V3D_CLE_CT0QBA, start);
    mmio::write(mmio::V3D_CLE_CT0QEA, end);
}

void executeRender(u32 start, u32 end)
{
    mmio::write(mmio::V3D_CLE_CT1QBA, start);
    mmio::write(mmio::V3D_CLE_CT1QEA, end);
}

}

}