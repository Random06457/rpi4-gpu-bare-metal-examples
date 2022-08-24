#pragma once

#include <memory>
#include "types.hpp"
#include "color.hpp"

namespace gfx
{

class FrameBuffer
{
public:
    FrameBuffer(size_t width, size_t height, size_t pitch, void* ptr) :
        m_width(width),
        m_height(height),
        m_pitch(pitch),
        m_data(reinterpret_cast<Color*>(ptr))
    {
    }

    auto width() const { return m_width; }
    auto height() const { return m_height; }
    auto pitch() const { return m_pitch; }
    auto data() const { return m_data; }
    auto data(size_t x, size_t y) const
    {
        return reinterpret_cast<Color*>(reinterpret_cast<u8*>(m_data) +
                                        y * m_pitch) +
               x;
    }

    static std::unique_ptr<FrameBuffer> createFromVC(size_t w, size_t h);

private:
    size_t m_width;
    size_t m_height;
    size_t m_pitch;
    Color* m_data;
};

}