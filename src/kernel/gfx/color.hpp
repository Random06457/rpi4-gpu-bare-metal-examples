#pragma once

#include "types.hpp"

namespace gfx
{

struct Color
{
    Color() = default;
    Color(const Color&) = default;

    constexpr Color(u32 argb) : argb(argb) {}
    constexpr Color(u8 r, u8 g, u8 b, u8 a) : b(b), g(g), r(r), a(a) {}

    union
    {
        struct
        {
            u8 b;
            u8 g;
            u8 r;
            u8 a;
        };
        u32 argb;
    };

    static const Color RED;
    static const Color GREEN;
    static const Color BLUE;
    static const Color BLACK;
    static const Color WHITE;
};

inline constexpr Color Color::RED = { 0xFF, 0x00, 0x00, 0xFF };
inline constexpr Color Color::GREEN = { 0x00, 0xFF, 0x00, 0xFF };
inline constexpr Color Color::BLUE = { 0x00, 0x00, 0xFF, 0xFF };
inline constexpr Color Color::BLACK = { 0x00, 0x00, 0x00, 0xFF };
inline constexpr Color Color::WHITE = { 0xFF, 0xFF, 0xFF, 0xFF };

}