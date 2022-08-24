#include <cstring>
#include "types.hpp"

extern "C"
{
    extern void (*__init_array_start[])();
    extern void (*__init_array_end[])();
    extern u8 __bss_start[];
    extern u8 __bss_end[];

    void __init_func()
    {
        for (auto iter = __init_array_start; iter < __init_array_end; iter++)
            (*iter)();
    }

    void __fini_func()
    {
    }

    void __clean_bss()
    {
        std::memset(__bss_start, 0, __bss_end - __bss_start);
    }
}