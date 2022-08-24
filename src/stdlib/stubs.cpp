#include <cassert>
#include <stdio.h>
#include "kernel/logging.hpp"

extern "C"
{
    FILE* stderr = (FILE*)2;
    FILE* stdout = (FILE*)1;

    int fputc(int, FILE*)
    {
        assert(0);
    }
    int* __errno_location()
    {
        assert(0);
    }
    int __fprintf_chk(FILE*, int, const char*, ...)
    {
        assert(0);
    }
    int fprintf(FILE*, const char*, ...)
    {
        assert(0);
    }
    void __udivti3()
    {
        assert(0);
    }
    void memchr()
    {
        assert(0);
    }
    void __memcpy_chk()
    {
        assert(0);
    }
    void __unordtf2()
    {
        assert(0);
    }
    void __getf2()
    {
        assert(0);
    }
    void __gttf2()
    {
        assert(0);
    }
    void __letf2()
    {
        assert(0);
    }
    void __eqtf2()
    {
        assert(0);
    }
    void __trunctfsf2()
    {
        assert(0);
    }
    void __trunctfdf2()
    {
        assert(0);
    }
    void __umodti3()
    {
        assert(0);
    }

    size_t fwrite(const void* buff, size_t size, size_t count, FILE* f)
    {
        assert(f == stdout || f == stdout);

        const char* ptr = reinterpret_cast<const char*>(buff);

        for (size_t i = 0; i < size * count; i++)
            Logging::printChar(ptr[i]);

        return size * count;
    }
}
