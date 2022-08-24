#pragma once

#include "drawer.hpp"
#include "framebuffer.hpp"
#include "text_writer.hpp"

namespace gfx
{

class Context
{
public:
    Context(size_t w, size_t h) :
        m_fb(std::move(FrameBuffer::createFromVC(w, h))),
        m_drawer(std::make_unique<Drawer>()),
        m_text_writer(std::make_unique<TextWriter>(m_drawer.get()))
    {
        m_drawer->setFb(m_fb.get());
        m_drawer->setFont(&gfx::Font::g_default_font);
    }

    auto fb() { return m_fb.get(); }
    auto fb() const { return m_fb.get(); }
    auto drawer() { return m_drawer.get(); }
    auto drawer() const { return m_drawer.get(); }
    auto writer() { return m_text_writer.get(); }
    auto writer() const { return m_text_writer.get(); }

private:
    std::unique_ptr<FrameBuffer> m_fb;
    std::unique_ptr<Drawer> m_drawer;
    std::unique_ptr<TextWriter> m_text_writer;
};

}