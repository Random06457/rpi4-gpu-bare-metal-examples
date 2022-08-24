#include <cstring>
#include "types.hpp"

extern "C"
{

    void* memset(void* s, int c, size_t n)
    {
        u8* ptr = reinterpret_cast<u8*>(s);

        for (size_t i = 0; i < n; i++)
            *ptr++ = static_cast<u8>(c);

        return s;
    }

    int memcmp(const void* buf1, const void* buf2, size_t n)
    {
        if (n == 0)
            return 0;

        const u8* ptr1 = reinterpret_cast<const u8*>(buf1);
        const u8* ptr2 = reinterpret_cast<const u8*>(buf2);

        while (n-- && *ptr1 == *ptr2)
        {
            ptr1++;
            ptr2++;
        }

        return *ptr1 - *ptr2;
    }

    int strcmp(const char* str1, const char* str2)
    {
        while (*str1 && *str2 && *str1 == *str2)
        {
            str1++;
            str2++;
        }

        return *str1 - *str2;
    }

    void* memmove(void* dst0, const void* src0, size_t size)
    {
        auto dst = reinterpret_cast<u8*>(dst0);
        auto src = reinterpret_cast<const u8*>(src0);

        if (dst > src)
        {
            // |A B C D E F|1 2 3 4 5
            //  A B C|A B C D E F|4 5

            for (size_t i = size - 1, j = 0; j < size; i--, j++)
                dst[i] = src[i];
        }
        else if (dst < src)
        {
            // A B C D E F|1 2 3 4 5|
            // A B C|1 2 3 4 5|3 4 5

            for (size_t i = 0; i < size; i++)
                dst[i] = src[i];
        }

        return dst0;
    }

    void* memcpy(void* dst0, const void* src0, size_t size)
    {
        auto dst = reinterpret_cast<u8*>(dst0);
        auto src = reinterpret_cast<const u8*>(src0);

        for (size_t i = 0; i < size; i++)
            dst[i] = src[i];

        return dst0;
    }

    size_t strlen(const char* str)
    {
        size_t ret = 0;
        while (*str++)
            ret++;
        return ret;
    }
}