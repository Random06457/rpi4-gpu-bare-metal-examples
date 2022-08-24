#pragma once

#include "font.hpp"
#include "framebuffer.hpp"

namespace gfx
{

class Drawer
{
public:
    void fillScreen(Color color);
    void fillRect(size_t x, size_t y, size_t w, size_t h, Color color);
    void drawGlyph(size_t x, size_t y, u8 glyIdx, Color fg, Color bg);
    void drawTex(void* tex, size_t x, size_t y, size_t w, size_t h);

    void setFont(Font* font) { m_font = font; }
    Font* getFont() const { return m_font; }

    void setFb(FrameBuffer* fb) { m_fb = fb; }
    FrameBuffer* getFb() const { return m_fb; }

private:
    FrameBuffer* m_fb = nullptr;
    Font* m_font = nullptr;
};

}