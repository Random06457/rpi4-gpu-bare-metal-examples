#pragma once

#include "types.hpp"

class uart
{
public:
    static void init(size_t baud);

#ifdef __GNUC__
    [[gnu::format(printf, 1, 2)]]
#endif
    static void
    print(const char* fmt, ...);
    static void write(const void* buff, size_t size);
    static void hexdump(const void* buff, size_t size);
    static void writeChar(char c);

private:
    static bool canWrite();
    static bool canRead();
    static void writeByte(u8 b);
    static u8 readByte();
};
