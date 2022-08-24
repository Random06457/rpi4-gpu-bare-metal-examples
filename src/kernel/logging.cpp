#include "logging.hpp"
#include <cstdarg>
#include <vector>
#include "stdlib/printf.h"

static std::vector<Logging::LogCallback> s_callbacks;

void Logging::addCallback(LogCallback callback)
{
    s_callbacks.push_back(callback);
}

void Logging::log(const char* fmt, ...)
{
    va_list args;

    va_start(args, fmt);

    fctvsnprintf([](char c, void* arg) { Logging::printChar(c); }, nullptr, fmt,
                 args);

    va_end(args);
}

void Logging::printChar(char c)
{
    for (auto& callback : s_callbacks)
        callback(c);
}

void Logging::hexdump(const void* buff, size_t size)
{
    auto ptr = reinterpret_cast<const u8*>(buff);

    log("         | 00 01 02 03 O4 05 06 07 08 09 0A 0B 0C 0C 0E 0F\n");
    log("         |-----------------------------------------------\n");

    for (u32 off = 0; off < size;)
    {
        log("%08X | ", off);

        for (size_t i = 0; i < 16 && off < size; i++, off++)
            log("%02X ", ptr[off]);

        log("\n");
    }
}