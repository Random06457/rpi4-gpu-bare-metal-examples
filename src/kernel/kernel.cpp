#include "kernel.hpp"
#include <cassert>
#include <cstring>
#include "arch/aarch64.hpp"
#include "device/pm.hpp"
#include "logging.hpp"

void Kernel::setupGfx(size_t w, size_t h)
{
    assert(m_gfx_ctx.get() == nullptr);

    m_gfx_ctx = std::make_unique<gfx::Context>(w, h);
}

void Kernel::kmain()
{
    Kernel kern;

    /* Setup Gfx */
    kern.setupGfx(1920, 1080);

    /* Setup Logger */
    Logging::addCallback([&kern](char c)
                         { kern.gfx()->writer()->printChar(c); });

    /* Check EL */
    u64 el = aarch64::getCurrEL();
    assert(el == 1);

    /* Setup Exceptions */
    extern u8 exception_vector_el1[];
    aarch64::setVBAR_EL1(exception_vector_el1);

    /* Power on V3D */
    pm::powerOnV3D();

    LOG("Hello World!\n");
    LOG("Currently running in EL%lld\n", el);

    /* Run our example code here */
    void rainbowTriangleGL(Kernel&);
    rainbowTriangleGL(kern);

    while (true)
        ;
}
