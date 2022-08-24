#include "text_writer.hpp"
#include "stdlib/printf.h"

namespace gfx
{

void TextWriter::printChar(char c)
{
    switch (c)
    {
        case '\r': m_x = m_origin_x; break;
        case '\n':
            m_x = m_origin_x;
            m_y += m_drawer->getFont()->getGlyphHeight() + 2;
            break;
        default:
            m_drawer->drawGlyph(m_x, m_y, c, m_fg, m_bg);
            m_x += m_drawer->getFont()->getGlyphWidth() + 1;
            break;
    }
}

void TextWriter::print(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    fctvsnprintf(
        [](char c, void* arg)
        {
            auto pos = static_cast<TextWriter*>(arg);
            pos->printChar(c);
        },
        this, fmt, args);

    va_end(args);
}

}