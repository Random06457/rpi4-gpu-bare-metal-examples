#pragma once

#include "types.hpp"

#define MAX_SIZE(a, b) (sizeof(a) > sizeof(b) ? sizeof(a) : sizeof(b))

#define CHECK_REQ_SIZE(x) static_assert(sizeof(req) == (x))
#define CHECK_RESP_SIZE(x) static_assert(sizeof(resp) == (x))
#define CHECK_REQ_RESP_SIZE(x, y)                                              \
    static_assert(sizeof(req) == x && sizeof(resp) == y)

namespace videocore
{
enum class ClockID : u32
{
    RESERVED = 0x000000000,
    EMMC = 0x000000001,
    UART = 0x000000002,
    ARM = 0x000000003,
    CORE = 0x000000004,
    V3D = 0x000000005,
    H264 = 0x000000006,
    ISP = 0x000000007,
    SDRAM = 0x000000008,
    PIXEL = 0x000000009,
    PWM = 0x00000000a,
    HEVC = 0x00000000b,
    EMMC2 = 0x00000000c,
    M2MC = 0x00000000d,
    PIXEL_BVB = 0x00000000e,
};

struct MBoxBuffer
{
    static constexpr u32 MBOX_REQUEST = 0x00000000;
    static constexpr u32 MBOX_RESP_SUCCESS = 0x80000000;
    static constexpr u32 MBOX_RESP_ERROR = 0x80000001;

    u32 size;
    u32 code;
} PACKED;

struct MBoxMessage
{
    u32 tag;
    u32 size;
    u32 size2;
} PACKED;
static_assert(sizeof(MBoxMessage) == 0xC);

// VideoCore

struct GetFirmwareVersion
{
    static constexpr u32 TAG = 0x00000001;

    struct
    {
        u32 revision;
    } resp;
    CHECK_RESP_SIZE(4);
} PACKED;

// Hardware

struct GetBoardModel
{
    static constexpr u32 TAG = 0x00010001;

    struct
    {
        u32 model;
    } resp;

    CHECK_RESP_SIZE(4);
} PACKED;

struct GetBoardRevision
{
    static constexpr u32 TAG = 0x00010002;

    struct
    {
        u32 revision;
    } resp;

    CHECK_RESP_SIZE(4);
} PACKED;

struct GetBoardMAC
{
    static constexpr u32 TAG = 0x00010003;

    struct
    {
        u8 address[6];
    } resp;

    CHECK_RESP_SIZE(6);
} PACKED;

struct GetBoardSerial
{
    static constexpr u32 TAG = 0x00010004;

    struct
    {
        u64 serial;
    } resp;

    CHECK_RESP_SIZE(8);
} PACKED;

struct GetArmMemory
{
    static constexpr u32 TAG = 0x00010005;

    struct
    {
        u32 address;
        u32 size;
    } resp;

    CHECK_RESP_SIZE(8);
} PACKED;

struct GetVcMemory
{
    static constexpr u32 TAG = 0x00010006;

    struct
    {
        u32 address;
        u32 size;
    } resp;

    CHECK_RESP_SIZE(8);
} PACKED;

// todo: GetClocks

// Config

// todo: GetCommandLine

// Shared resource mangement

struct GetDmaChannels
{
    static constexpr u32 TAG = 0x00060001;

    struct
    {
        u32 mask;
    } resp;
    CHECK_RESP_SIZE(4);
} PACKED;

// Power

struct GetPowerState
{
    static constexpr u32 TAG = 0x00020001;

    GetPowerState(u32 device_id) : req{ device_id } {}

    union
    {
        struct
        {
            u32 device_id;
        } req;
        struct
        {
            u32 device_id;
            u32 state;
        } resp;
    };
    CHECK_REQ_RESP_SIZE(4, 8);
} PACKED;

struct SetDomainState
{
    static constexpr u32 TAG = 0x00038030;

    SetDomainState(u32 device_id, u32 state) : req{ device_id, state } {}

    union
    {
        struct
        {
            u32 device_id;
            u32 state;
        } req, resp;
    };
    CHECK_REQ_RESP_SIZE(8, 8);
} PACKED;

struct GetDomainState
{
    static constexpr u32 TAG = 0x00030030;

    GetDomainState(u32 device_id) : req{ device_id } {}

    union
    {
        struct
        {
            u32 device_id;
        } req;
        struct
        {
            u32 device_id;
            u32 state;
        } resp;
    };
    CHECK_REQ_RESP_SIZE(4, 8);
} PACKED;

struct GetTiming
{
    static constexpr u32 TAG = 0x00020002;

    GetTiming(u32 device_id) : req{ device_id } {}

    union
    {
        struct
        {
            u32 device_id;
        } req;
        struct
        {
            u32 device_id;
            u32 enable_wait;
        } resp;
    };
    CHECK_REQ_RESP_SIZE(4, 8);
} PACKED;

struct SetPowerState
{
    static constexpr u32 TAG = 0x00028001;

    SetPowerState(u32 device_id, u32 state) : req{ device_id, state } {}

    union
    {
        struct
        {
            u32 device_id;
            u32 state;
        } req;
        struct
        {
            u32 device_id;
            u32 state;
        } resp;
    };
    CHECK_REQ_RESP_SIZE(8, 8);
} PACKED;

// Clocks

struct GetClockState
{
    static constexpr u32 TAG = 0x00030001;

    GetClockState(ClockID clock_id) : req{ clock_id } {}

    union
    {
        struct
        {
            ClockID clock_id;
        } req;
        struct
        {
            ClockID clock_id;
            u32 state;
        } resp;
    };
    CHECK_REQ_RESP_SIZE(4, 8);
} PACKED;

struct SetClockState
{
    static constexpr u32 TAG = 0x00038001;

    SetClockState(ClockID clock_id, u32 state) : req{ clock_id, state } {}

    union
    {
        struct
        {
            ClockID clock_id;
            u32 state;
        } req;
        struct
        {
            ClockID clock_id;
            u32 state;
        } resp;
    };
    CHECK_REQ_RESP_SIZE(8, 8);
} PACKED;

struct GetClockRate
{
    static constexpr u32 TAG = 0x00030002;

    GetClockRate(ClockID clock_id) : req{ clock_id } {}

    union
    {
        struct
        {
            ClockID clock_id;
        } req;
        struct
        {
            ClockID clock_id;
            u32 rate;
        } resp;
    };
    CHECK_REQ_RESP_SIZE(4, 8);
} PACKED;

struct GetClockRateMeasured
{
    static constexpr u32 TAG = 0x00030047;

    GetClockRateMeasured(ClockID clock_id) : req{ clock_id } {}

    union
    {
        struct
        {
            ClockID clock_id;
        } req;
        struct
        {
            ClockID clock_id;
            u32 rate;
        } resp;
    };
    CHECK_REQ_RESP_SIZE(4, 8);
} PACKED;

struct SetClockRate
{
    static constexpr u32 TAG = 0x00038002;

    SetClockRate(ClockID clock_id, u32 rate, u32 skip_setting_turbo) :
        req{ clock_id, rate, skip_setting_turbo }
    {
    }

    union
    {
        struct
        {
            ClockID clock_id;
            u32 rate;
            u32 skip_setting_turbo;
        } req;
        struct
        {
            ClockID clock_id;
            u32 rate;
        } resp;
    };
    CHECK_REQ_RESP_SIZE(12, 8);
} PACKED;

// todo: ?
struct SetClockRate2
{
    static constexpr u32 TAG = 0x00038002;

    SetClockRate2(ClockID clock_id, u32 rate) : req{ clock_id, rate } {}

    union
    {
        struct
        {
            ClockID clock_id;
            u32 rate;
        } req;
        struct
        {
            ClockID clock_id;
            u32 rate;
        } resp;
    };
    CHECK_REQ_RESP_SIZE(8, 8);
} PACKED;

struct GetMaxClockRate
{
    static constexpr u32 TAG = 0x00030004;

    GetMaxClockRate(ClockID clock_id) : req{ clock_id } {}

    union
    {
        struct
        {
            ClockID clock_id;
        } req;
        struct
        {
            ClockID clock_id;
            u32 rate;
        } resp;
    };
    CHECK_REQ_RESP_SIZE(4, 8);
} PACKED;

// memory
struct AllocMem
{
    static constexpr u32 MEM_FLAG_DISCARDABLE =
        1 << 0; /* can be resized to 0 at any time. Use for cached data */
    static constexpr u32 MEM_FLAG_NORMAL =
        0 << 2; /* normal allocating alias. Don't use from ARM */
    static constexpr u32 MEM_FLAG_DIRECT = 1 << 2; /* 0xC alias uncached */
    static constexpr u32 MEM_FLAG_COHERENT =
        2 << 2; /* 0x8 alias. Non-allocating in L2 but coherent */
    static constexpr u32 MEM_FLAG_L1_NONALLOCATING =
        (MEM_FLAG_DIRECT | MEM_FLAG_COHERENT); /* Allocating in L2 */
    static constexpr u32 MEM_FLAG_ZERO =
        1 << 4; /* initialise buffer to all zeros */
    static constexpr u32 MEM_FLAG_NO_INIT =
        1 << 5; /* don't initialise (default is initialise to all ones */
    static constexpr u32 MEM_FLAG_HINT_PERMALOCK =
        1 << 6; /* Likely to be locked for long periods of time. */

    static constexpr u32 TAG = 0x0003000c;

    AllocMem(u32 size, u32 alignment, u32 flags) : req{ size, alignment, flags }
    {
    }

    union
    {
        struct
        {
            u32 size;
            u32 alignment;
            u32 flags;
        } req;
        struct
        {
            u32 handle;
        } resp;
    };
    CHECK_REQ_RESP_SIZE(12, 4);
} PACKED;

struct LockMemory
{
    static constexpr u32 TAG = 0x0003000d;

    LockMemory(u32 handle) : req{ handle } {}

    union
    {
        struct
        {
            u32 handle;
        } req;
        struct
        {
            u32 address;
        } resp;
    };

    CHECK_REQ_RESP_SIZE(4, 4);
} PACKED;

struct UnlockMemory
{
    static constexpr u32 TAG = 0x0003000e;

    UnlockMemory(u32 handle) : req{ handle } {}

    union
    {
        struct
        {
            u32 handle;
        } req;
        struct
        {
            u32 status;
        } resp;
    };
    CHECK_REQ_RESP_SIZE(4, 4);
} PACKED;

struct ReleaseMemory
{
    static constexpr u32 TAG = 0x0003000f;

    ReleaseMemory(u32 handle) : req{ handle } {}

    union
    {
        struct
        {
            u32 handle;
        } req;
        struct
        {
            u32 status;
        } resp;
    };
    CHECK_REQ_RESP_SIZE(4, 4);
} PACKED;

struct ExecuteCode
{
    static constexpr u32 TAG = 0x00030010;

    ExecuteCode(u32 entrypoint, const u32* r) : req{ entrypoint, {} }
    {
        req.entrypoint = entrypoint;
        for (size_t i = 0; i < 6; i++)
            req.r[i] = r[i];
    }

    ExecuteCode(u32 entrypoint, u32 r0, u32 r1, u32 r2, u32 r3, u32 r4,
                u32 r5) :
        req{ entrypoint,
             {
                 r0,
                 r1,
                 r2,
                 r3,
                 r4,
                 r5,
             } }
    {
    }

    union
    {
        struct
        {
            u32 entrypoint;
            u32 r[6];
        } req;
        struct
        {
            u32 r0;
        } resp;
    };

    CHECK_REQ_RESP_SIZE(28, 4);
} PACKED;

struct QPUEnable
{
    static constexpr u32 TAG = 0x00030012;

    QPUEnable(u32 enable) : req{ enable } {}

    struct
    {
        u32 enable;
    } req;
    CHECK_REQ_SIZE(4);
} PACKED;

// Framebuffer

struct AllocFB
{
    static constexpr u32 TAG = 0x00040001;

    AllocFB(u32 align) : req{ align } {}

    union
    {
        struct
        {
            u32 align;
        } req;
        struct
        {
            u32 addr;
            u32 size;
        } resp;
    };
    CHECK_REQ_RESP_SIZE(4, 8);
} PACKED;

struct SetPhysicalWH
{
    static constexpr u32 TAG = 0x00048003;

    SetPhysicalWH(u32 width, u32 height) : req{ width, height } {}

    union
    {
        struct
        {
            u32 width;
            u32 height;
        } req, resp;
    };
    CHECK_REQ_RESP_SIZE(8, 8);
} PACKED;

struct SetVirtualWH
{
    static constexpr u32 TAG = 0x00048004;

    SetVirtualWH(u32 width, u32 height) : req{ width, height } {}

    union
    {
        struct
        {
            u32 width;
            u32 height;
        } req, resp;
    };
    CHECK_REQ_RESP_SIZE(8, 8);
} PACKED;

struct SetDepth
{
    static constexpr u32 TAG = 0x00048005;

    SetDepth(u32 depth) : req{ depth } {}

    union
    {
        struct
        {
            u32 depth;
        } req, resp;
    };
    CHECK_REQ_RESP_SIZE(4, 4);
} PACKED;

struct SetPixelOrder
{
    static constexpr u32 TAG = 0x00048006;

    SetPixelOrder(u32 state) : req{ state } {}

    union
    {
        struct
        {
            u32 state;
        } req, resp;
    };
    CHECK_REQ_RESP_SIZE(4, 4);
} PACKED;

struct GetPitch
{
    static constexpr u32 TAG = 0x00048008;

    GetPitch() : resp{ 0 } {}

    union
    {
        struct
        {
            u32 pitch;
        } resp;
    };
    CHECK_RESP_SIZE(4);
} PACKED;

struct SetVirtualOffset
{
    static constexpr u32 TAG = 0x00048009;

    SetVirtualOffset(u32 x, u32 y) : req{ x, y } {}

    union
    {
        struct
        {
            u32 x;
            u32 y;
        } req, resp;
    };
    CHECK_REQ_RESP_SIZE(8, 8);
} PACKED;

struct NotifyXhciReset
{
    static constexpr u32 TAG = 0x00030058;

    NotifyXhciReset(u32 addr) : addr(addr) {}

    u32 addr;
} PACKED;

struct VchiqInit
{
    static constexpr u32 TAG = 0x00048010;

    VchiqInit(u32 channel) : channel(channel) {}

    u32 channel;
} PACKED;
};
