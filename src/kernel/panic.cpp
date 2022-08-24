#include "panic.h"
#include "logging.hpp"

extern "C"
{

    void panic(const char* msg)
    {
        LOG("\n");
        LOG("\nPANIC!!\n\n");
        LOG("%s", msg);

        while (true)
            ;
    }
}