#include "device/mmio.hpp"
#include <cassert>
#include "bit_utils.hpp"

void mmio::gpio::function(size_t idx, Function func)
{
    assert(func < 8);
    assert(idx < mmio::GPIO_COUNT);

    intptr_t gpio_addr = mmio::GPFSEL0 + (idx / 10) * 4;
    size_t shift = (idx % 10) * 3;

    u32 x = mmio::read(gpio_addr);
    x = bits::setBits<u32>(x, func, 3, shift);
    mmio::write(gpio_addr, x);
}

void mmio::gpio::set(size_t idx, bool value)
{
    assert(idx < mmio::GPIO_COUNT);

    intptr_t base_addr = value ? mmio::GPSET0 : mmio::GPCLR0;

    intptr_t gpio_addr = base_addr + (idx / 32) * 4;

    mmio::write(gpio_addr, 1 << (idx % 32));
}

void mmio::gpio::setPull(size_t idx, Resistor resistor)
{
    assert(resistor < 3);
    assert(idx < mmio::GPIO_COUNT);

    intptr_t gpio_addr = mmio::GPUPD0 + (idx / 16) * 4;
    size_t shift = (idx % 16) * 2;

    u32 x = mmio::read(gpio_addr);
    x = bits::setBits<u32>(x, resistor, 2, shift);
    mmio::write(gpio_addr, x);
}

mmio::gpio::Resistor mmio::gpio::getPull(size_t idx)
{
    assert(idx < mmio::GPIO_COUNT);

    intptr_t gpio_addr = mmio::GPUPD0 + (idx / 16) * 4;
    size_t shift = (idx % 16) * 2;

    u32 x = mmio::read(gpio_addr);
    return static_cast<Resistor>(bits::shiftR(x, 2, shift));
}
