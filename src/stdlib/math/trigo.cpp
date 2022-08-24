#include <cmath>
#include "types.hpp"

static consteval size_t facto(size_t x)
{
    if (x == 0)
        return 1;
    else
        return facto(x - 1) * x;
}

f64 trunc(f64 x)
{
    return (x > 0) ? floor(x) : ceil(x);
}

f64 fmod(f64 a, f64 b)
{
    return a - (trunc(a / b) * b);
}

static f64 sin0(f64 x)
{
    f64 x2 = x * x;
    f64 x3 = x2 * x;
    f64 x5 = x3 * x2;
    f64 x7 = x5 * x2;
    f64 x9 = x7 * x2;

    return x - x3 / facto(3) + x5 / facto(5) - x7 / facto(7) + x9 / facto(9);
}

static f64 cos0(f64 x)
{
    f64 x2 = x * x;
    f64 x4 = x2 * x2;
    f64 x6 = x4 * x2;
    f64 x8 = x4 * x4;

    return 1 - x2 / facto(2) + x4 / facto(4) - x6 / facto(6) + x8 / facto(8);
}

f64 sin(f64 x)
{
    x = fmod(x, M_PI * 2);
    if (x <= M_PI_2)
        return sin0(x);
    if (x <= M_PI)
        return cos0(x - M_PI_2);
    if (x <= 3 * M_PI_2)
        return -sin0(x - M_PI);
    else
        return -cos0(x - 3 * M_PI_2);
}

f64 cos(f64 x)
{
    return sin(x + M_PI_2);
}

f64 tan(f64 x)
{
    return sin(x) / cos(x);
}
