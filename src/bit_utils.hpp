#pragma once

#include "types.hpp"

namespace bits
{
template<typename T>
inline constexpr T bit(size_t n)
{
    return (1u << n);
}

template<typename T>
inline constexpr T mask(size_t n)
{
    return (1u << n) - 1u;
}

template<typename T>
inline constexpr T shiftMask(size_t n, size_t shift)
{
    return mask<T>(n) << shift;
}

template<typename T>
inline constexpr T shiftR(T x, size_t n, size_t shift)
{
    return (x >> shift) & mask<T>(n);
}

template<typename T>
inline constexpr T shiftL(T x, size_t n, size_t shift)
{
    return (x & mask<T>(n)) << shift;
}

template<typename T>
inline constexpr T setBits(T x, T val, size_t n, size_t shift)
{
    x &= ~shiftMask<T>(n, shift);
    x |= shiftL(val, n, shift);
    return x;
}

template<typename T>
inline constexpr T align(T x, size_t align)
{
    return (x + align - 1) & ~(align - 1);
}
}