#pragma once

#include <cstddef>
#include <cstdint>

using s8 = std::int8_t;
using u8 = std::uint8_t;
using s16 = std::int16_t;
using u16 = std::uint16_t;
using s32 = std::int32_t;
using u32 = std::uint32_t;
using s64 = signed long long int;
using u64 = unsigned long long int;

using f32 = float;
using f64 = double;

using vs8 = volatile s8;
using vu8 = volatile u8;
using vs16 = volatile s16;
using vu16 = volatile u16;
using vs32 = volatile s32;
using vu32 = volatile u32;
using vs64 = volatile s64;
using vu64 = volatile u64;

#define PACKED __attribute__((packed))
#define ALIGNED(n) __attribute__((aligned(n)))
