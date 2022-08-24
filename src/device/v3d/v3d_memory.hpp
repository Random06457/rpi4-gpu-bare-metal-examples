#pragma once

#include "types.hpp"
#include "mbox_message.hpp"

template<typename T>
static constexpr u32 ARM_TO_BUS(T* ptr)
{
    // We use linear mapping so no need to do any fancy translation.
    // Truncating a pointer is dangerous but in our case it works because
    // all the addresses we use which are currently guaranteed to be < 2^32.
    // Though, for a bigger project you probably want to setup the MMU and do
    // something more robust.
    return static_cast<u32>(reinterpret_cast<uintptr_t>(ptr));
}

class V3DBuffer
{
public:
    V3DBuffer(u32 size, u32 align)
    {
        m_addr = new (std::align_val_t(align)) u8[size];
        m_size = size;
        m_align = align;
    }
    ~V3DBuffer() { delete[] m_addr; }

    template<typename T = void>
    inline T* ptr(size_t off = 0)
    {
        return reinterpret_cast<T*>(paddr() + off);
    }

    u32 paddr() const { return ARM_TO_BUS(m_addr); }

    size_t size() const { return m_size; }

public:
    u32 m_size;
    u32 m_align;
    u8* m_addr;
};
