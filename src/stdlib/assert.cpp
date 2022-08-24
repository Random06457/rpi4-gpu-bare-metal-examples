#include <cassert>
#include "kernel/logging.hpp"
#include "kernel/panic.h"

extern "C"
{

    void __assert_fail(const char* expr, const char* file, unsigned int line,
                       const char* function)
    {
        LOG("\nAssertion failed in function \"%s\" at %s:%u :\n\"%s\"",
            function, file, line, expr);

        panic("Assertion failed");
    }
}