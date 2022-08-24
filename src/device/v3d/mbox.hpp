#pragma once

#include <cassert>
#include <type_traits>
#include <utility>
#include "types.hpp"
#include "device/mmio.hpp"
#include "kernel/logging.hpp"
#include "mbox_message.hpp"

namespace videocore
{
class MailBox
{
public:
    static constexpr u32 MBOX_FULL = 0x80000000;
    static constexpr u32 MBOX_EMPTY = 0x40000000;

public:
    enum Channel
    {
        CHANNEL_POWER = 0,
        CHANNEL_FB = 1,
        CHANNEL_VUART = 2,
        CHANNEL_VCHIQ = 3,
        CHANNEL_LEDS = 4,
        CHANNEL_BTNS = 5,
        CHANNEL_TOUCH = 6,
        CHANNEL_COUNT = 7,
        CHANNEL_PROP = 8 // Request from ARM for response by VideoCore
    };

    static inline bool isEmpty()
    {
        return mmio::read(mmio::MBOX_STATUS) & MBOX_EMPTY;
    }
    static inline bool isFull()
    {
        return mmio::read(mmio::MBOX_STATUS) & MBOX_FULL;
    }

    static void doCall(Channel channel, const void* box);

    template<typename T>
    struct alignas(16) SingleRequest
    {
        static constexpr size_t SIZE = offsetof(SingleRequest<T>, endTag) + 4;

        template<typename... TArgs>
        SingleRequest(TArgs... args) :
            buff{
                .size = SIZE,
                .code = MBoxBuffer::MBOX_REQUEST,
            },
            msgHdr{ .tag = T::TAG, .size = sizeof(T), .size2 = sizeof(T) },
            msg(std::forward<TArgs>(args)...),
            endTag{ 0 }
        {
        }

        SingleRequest(const T& data) :
            buff{
                .size = SIZE + 4,
                .code = MBoxBuffer::MBOX_REQUEST,
            },
            msgHdr{ .tag = T::TAG, .size = sizeof(T), .size2 = sizeof(T) },
            msg(data),
            endTag{ 0 }
        {
        }

        /* 0x00 */ MBoxBuffer buff;
        /* 0x08 */ MBoxMessage msgHdr;
        /* 0x14 */ T msg;
        /* 0xXX */ u32 endTag;
    } PACKED;

    template<typename T>
    static inline void call(SingleRequest<T>& data)
    {
        doCall(CHANNEL_PROP, &data);
    }

    template<typename T>
    static inline SingleRequest<T> call(const T& data)
    {
        SingleRequest<T> ret(data);
        call<T>(ret);
        return ret;
    }

    template<typename T, typename... TArgs>
    static inline SingleRequest<T> call(TArgs... args)
    {
        SingleRequest<T> ret(std::forward<TArgs>(args)...);
        call<T>(ret);
        return ret;
    }

public: // todo: make private and clean up
    static inline ALIGNED(16) vu32 s_mbox[36];
};
};
