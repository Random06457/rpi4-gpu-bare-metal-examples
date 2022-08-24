#pragma once

#include "types.hpp"

namespace gfx
{

class Font
{
public:
    Font(size_t gly_count, size_t w, size_t h, size_t bpg, size_t bpl,
         const void* data) :
        m_glyph_count(gly_count),
        m_glyph_width(w),
        m_glyph_height(h),
        m_byte_per_glyph(bpg),
        m_byte_per_line(bpl),
        m_glyph_data(reinterpret_cast<const u8*>(data))
    {
    }

public:
    size_t getGlyphCount() const { return m_glyph_count; }
    size_t getGlyphWidth() const { return m_glyph_width; }
    size_t getGlyphHeight() const { return m_glyph_height; }
    size_t getBytePerGlyph() const { return m_byte_per_glyph; }
    size_t getBytePerLine() const { return m_byte_per_line; }

    const u8* getGlyph(size_t idx) const
    {
        return m_glyph_data + m_byte_per_glyph * idx;
    }

private:
    size_t m_glyph_count;
    size_t m_glyph_width;
    size_t m_glyph_height;
    size_t m_byte_per_glyph;
    size_t m_byte_per_line;
    const u8* m_glyph_data;

public:
    static Font g_default_font;
};

}