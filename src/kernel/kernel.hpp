#pragma once

#include "gfx/context.hpp"

class Kernel
{
public:
    [[noreturn]] static void kmain();

    Kernel() = default;

    void setupGfx(size_t w, size_t h);

    auto& gfx() { return m_gfx_ctx; }
    auto& gfx() const { return m_gfx_ctx; }

private:
    std::unique_ptr<gfx::Context> m_gfx_ctx = nullptr;
};
