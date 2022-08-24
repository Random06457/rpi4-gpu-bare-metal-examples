#include "pm.hpp"

namespace pm
{

void powerOnV3D()
{
    // Deasserting reset
    write(mmio::PM_GRAFX, read(mmio::PM_GRAFX) | PM_V3DRSTN);

    // Enabling ASB master
    write(mmio::ASB_V3D_M_CTRL, read(mmio::ASB_V3D_M_CTRL) & ~ASB_REQ_STOP);
    // Waiting for acknowledgement
    while (read(mmio::ASB_V3D_M_CTRL) & ASB_ACK)
        ;

    // Enabling ASB slave
    write(mmio::ASB_V3D_S_CTRL, read(mmio::ASB_V3D_S_CTRL) & ~ASB_REQ_STOP);
    // Waiting for acknowledgement
    while (read(mmio::ASB_V3D_S_CTRL) & ASB_ACK)
        ;
}

}
