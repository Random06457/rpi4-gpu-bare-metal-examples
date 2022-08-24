#pragma once

#include "types.hpp"
#include "drawer.hpp"

namespace gfx
{

class TextWriter
{
public:
    constexpr TextWriter(Drawer* drawer = nullptr)
    {
        setCursor(0, 0);
        setColor(Color::WHITE, Color::BLACK);
        m_drawer = drawer;
    }

    constexpr void setCursor(u32 x, u32 y)
    {
        m_origin_x = x;
        m_origin_y = y;
        m_x = x;
        m_y = y;
    }
    constexpr void setColor(Color fg, Color bg)
    {
        m_bg = bg;
        m_fg = fg;
    }

    void printChar(char c);
    void print(const char* fmt, ...);

private:
    u32 m_origin_x;
    u32 m_origin_y;
    u32 m_x;
    u32 m_y;
    Color m_bg;
    Color m_fg;
    Drawer* m_drawer;
};

}