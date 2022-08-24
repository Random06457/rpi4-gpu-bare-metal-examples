#include <cassert>

namespace std
{
[[__noreturn__]] void __throw_length_error(const char*)
{
    assert(0 && "std::__throw_length_error");
}
[[__noreturn__]] void __throw_out_of_range_fmt(const char*, ...)
{
    assert(0 && "std::__throw_out_of_range_fmt");
}
[[__noreturn__]] void __throw_bad_function_call()
{
    assert(0 && "std::__throw_bad_function_call");
}
[[__noreturn__]] void __throw_logic_error(const char*)
{
    assert(0 && "std::__throw_logic_error");
}
[[__noreturn__]] void __throw_bad_alloc()
{
    assert(0 && "std::__throw_bad_alloc");
}
[[__noreturn__]] void __throw_bad_array_new_length()
{
    assert(0 && "std::__throw_bad_array_new_length");
}

[[noreturn]] void terminate() noexcept
{
    assert(0 && "std::terminate");
}
}
