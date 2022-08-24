#pragma once

#include <functional>
#include "types.hpp"

#define LOG(fmt, ...) Logging::log(fmt, ##__VA_ARGS__)
#define HEXDUMP(ptr, size) Logging::hexdump(ptr, size);

class Logging
{
public:
    using LogCallback = std::function<void(char)>;

#ifdef __GNUC__
    [[gnu::format(printf, 1, 2)]]
#endif
    static void
    log(const char* fmt, ...);
    static void printChar(char c);

    static void addCallback(LogCallback callback);

    static void hexdump(const void* buff, size_t size);
};
