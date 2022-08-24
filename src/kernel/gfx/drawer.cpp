#include "drawer.hpp"
#include <cassert>

namespace gfx
{

void Drawer::fillScreen(Color color)
{
    assert(m_fb);

    fillRect(0, 0, m_fb->width(), m_fb->height(), color);
}

void Drawer::fillRect(size_t x, size_t y, size_t w, size_t h, Color color)
{
    assert(m_fb);

    for (size_t y2 = y; y2 < y + h && y2 < m_fb->height(); y2++)
        for (size_t x2 = x; x2 < x + w && x2 < m_fb->width(); x2++)
            *m_fb->data(x2, y2) = color;
}

void Drawer::drawGlyph(size_t x, size_t y, u8 gly_idx, Color fg, Color bg)
{
    assert(m_font && m_fb);

    auto glyph = m_font->getGlyph(gly_idx);

    for (size_t yOff = 0;
         yOff < m_font->getGlyphWidth() && y + yOff < m_fb->height(); yOff++)
        for (size_t xOff = 0;
             xOff < m_font->getGlyphHeight() && x + xOff < m_fb->width();
             xOff++)
        {
            bool filled = glyph[yOff] & (1 << xOff);
            *m_fb->data(x + xOff, y + yOff) = filled ? fg : bg;
        }
}

void Drawer::drawTex(void* tex, size_t x, size_t y, size_t w, size_t h)
{
    assert(m_fb);

    Color* col = reinterpret_cast<Color*>(tex);

    for (size_t yOff = 0; yOff < h && y + yOff < m_fb->height(); yOff++)
        for (size_t xOff = 0; xOff < w && x + xOff < m_fb->width(); xOff++)
            *m_fb->data(x + xOff, y + yOff) = col[yOff * w + xOff];
}

}