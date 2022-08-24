#include "device/uart.hpp"
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include "device/mmio.hpp"
#include "stdlib/printf.h"

static constexpr size_t AUX_UART_CLOCK = 500000000;

static constexpr size_t AUX_MU_BAUD(size_t baud)
{
    return ((AUX_UART_CLOCK / (baud * 8)) - 1);
}

bool uart::canWrite()
{
    return !!(mmio::read(mmio::AUX_MU_LSR_REG) & 0x20);
}

bool uart::canRead()
{
    return !!(mmio::read(mmio::AUX_MU_LSR_REG) & 1);
}

void uart::writeByte(u8 b)
{
    while (!uart::canWrite())
        ;

    mmio::write(mmio::AUX_MU_IO_REG, b);
}

void uart::writeChar(char c)
{
    if (c == '\n')
        writeByte('\r');
    writeByte(c);
}

u8 uart::readByte()
{
    while (!uart::canRead())
        ;

    return mmio::read(mmio::AUX_MU_IO_REG);
}

void uart::init(size_t baud)
{
    mmio::write(mmio::AUX_ENABLES, 1); // enable UART1
    mmio::write(mmio::AUX_MU_IER_REG, 0);
    mmio::write(mmio::AUX_MU_CNTL_REG, 0);
    mmio::write(mmio::AUX_MU_LCR_REG, 3); // 8 bits
    mmio::write(mmio::AUX_MU_MCR_REG, 0);
    mmio::write(mmio::AUX_MU_IER_REG, 0);
    mmio::write(mmio::AUX_MU_IIR_REG, 0b11000110); // disable interrupts
    mmio::write(mmio::AUX_MU_BAUD_REG, AUX_MU_BAUD(baud));

    // set pin 14 as TX
    mmio::gpio::setPull(14, mmio::gpio::PULL_NONE);
    mmio::gpio::function(14, mmio::gpio::FUNC5); // TXD1

    // set pin 15 as RX
    mmio::gpio::setPull(15, mmio::gpio::PULL_NONE);
    mmio::gpio::function(15, mmio::gpio::FUNC5); // RXD1

    mmio::write(mmio::AUX_MU_CNTL_REG, 3); // enable RX/TX
}

void uart::print(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    fctvsnprintf([](char c, void*) { writeChar(c); }, nullptr, fmt, args);

    va_end(args);
}

void uart::write(const void* buff, size_t size)
{
    auto ptr = reinterpret_cast<const u8*>(buff);
    for (size_t i = 0; i < size; i++)
    {
        uart::writeByte(ptr[i]);
    }
}
