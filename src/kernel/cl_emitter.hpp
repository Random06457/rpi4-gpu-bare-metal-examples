#pragma once

#include <cstring>
#include <span>
#include "bit_utils.hpp"
#include "types.hpp"
#include "device/v3d/v3d_memory.hpp"

class CLEmitter
{
public:
    CLEmitter(size_t size = 0x1000, size_t align = 4096) :
        m_buff(new (std::align_val_t(align)) u8[size]),
        m_offset(0),
        m_capacity(size)
    {
    }

    ~CLEmitter() { delete[] m_buff; }

    void reset() { m_offset = 0; }

    u32 paddr(size_t off = 0) const
    {
        return ARM_TO_BUS(m_buff + m_offset + off);
    }
    template<typename T = void>
    T* ptr(size_t off = 0)
    {
        return reinterpret_cast<T*>(m_buff + m_offset + off);
    }
    template<typename T = void>
    T* alloc(size_t size)
    {
        auto ret = ptr<T>();
        m_offset += size;
        return ret;
    }

    void align(size_t align)
    {
        size_t size = bits::align<u32>(paddr(), align) - paddr();
        alloc(size);
    }

    template<typename T>
        CLEmitter& operator<<(T&& x) requires std::is_same_v < std::span<u8>,
    typename std::remove_reference<T>::type >
    {
        assert(m_offset + sizeof(x) <= m_capacity);
        std::memcpy(ptr(), x.data(), x.size_bytes());
        m_offset += x.size_bytes();
        return *this;
    }

    template<typename T>
    CLEmitter& operator<<(T&& x)
    {
        using Y = std::remove_reference<T>::type;
        static_assert(!std::is_same_v<std::span<u8>, Y>);

        assert(m_offset + sizeof(x) <= m_capacity);
        std::memcpy(ptr(), &x, sizeof(T));
        m_offset += sizeof(T);
        return *this;
    }

    size_t size() const { return m_offset; }
    size_t capacity() const { return m_capacity; }
    u32 startPaddr() { return ARM_TO_BUS(m_buff); }
    u32 endPaddr() { return ARM_TO_BUS(m_buff + size()); }
    template<typename T = void>
    T* startPtr()
    {
        return reinterpret_cast<T*>(m_buff);
    }

private:
    u8* m_buff;
    size_t m_offset;
    size_t m_capacity;
};
