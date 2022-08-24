#pragma once

#include "types.hpp"

namespace mmio
{

// static constexpr intptr_t PERIPHERAL_BASE   = 0x7E000000;
static constexpr intptr_t PERIPHERAL_BASE = 0xFE000000;

static constexpr intptr_t GPIO_BASE = PERIPHERAL_BASE + 0x200000;
static constexpr intptr_t GPFSEL0 = GPIO_BASE + 0x00;
static constexpr intptr_t GPFSEL1 = GPIO_BASE + 0x04;
static constexpr intptr_t GPFSEL2 = GPIO_BASE + 0x08;
static constexpr intptr_t GPFSEL3 = GPIO_BASE + 0x0C;
static constexpr intptr_t GPFSEL4 = GPIO_BASE + 0x10;
static constexpr intptr_t GPFSEL5 = GPIO_BASE + 0x14;
static constexpr intptr_t GPSET0 = GPIO_BASE + 0x1C;
static constexpr intptr_t GPSET1 = GPIO_BASE + 0x20;
static constexpr intptr_t GPCLR0 = GPIO_BASE + 0x28;
static constexpr intptr_t GPCLR1 = GPIO_BASE + 0x2C;

static constexpr intptr_t GPUPD0 = GPIO_BASE + 0xE4;
static constexpr intptr_t GPUPD1 = GPIO_BASE + 0xE8;
static constexpr intptr_t GPUPD2 = GPIO_BASE + 0xEC;
static constexpr intptr_t GPUPD3 = GPIO_BASE + 0xF0;

static constexpr intptr_t AUX_BASE = PERIPHERAL_BASE + 0x215000;
static constexpr intptr_t AUX_IRQ =
    AUX_BASE + 0x00; // Auxiliary Interrupt status
static constexpr intptr_t AUX_ENABLES = AUX_BASE + 0x04;   // Auxiliary enables
static constexpr intptr_t AUX_MU_IO_REG = AUX_BASE + 0x40; // Mini UART I/O Data
static constexpr intptr_t AUX_MU_IER_REG =
    AUX_BASE + 0x44; // Mini UART Interrupt Enable
static constexpr intptr_t AUX_MU_IIR_REG =
    AUX_BASE + 0x48; // Mini UART Interrupt Identify
static constexpr intptr_t AUX_MU_LCR_REG =
    AUX_BASE + 0x4C; // Mini UART Line Control
static constexpr intptr_t AUX_MU_MCR_REG =
    AUX_BASE + 0x50; // Mini UART Modem Control
static constexpr intptr_t AUX_MU_LSR_REG =
    AUX_BASE + 0x54; // Mini UART Line Status
static constexpr intptr_t AUX_MU_MSR_REG =
    AUX_BASE + 0x58; // Mini UART Modem Status
static constexpr intptr_t AUX_MU_SCRATCH = AUX_BASE + 0x5C; // Mini UART Scratch
static constexpr intptr_t AUX_MU_CNTL_REG =
    AUX_BASE + 0x60; // Mini UART Extra Control
static constexpr intptr_t AUX_MU_STAT_REG =
    AUX_BASE + 0x64; // Mini UART Extra Status
static constexpr intptr_t AUX_MU_BAUD_REG =
    AUX_BASE + 0x68; // Mini UART Baudrate

static constexpr intptr_t VIDEOCORE_MBOX = PERIPHERAL_BASE + 0x0000B880;
static constexpr intptr_t MBOX_READ = VIDEOCORE_MBOX + 0x00;
static constexpr intptr_t MBOX_POLL = VIDEOCORE_MBOX + 0x10;
static constexpr intptr_t MBOX_SENDER = VIDEOCORE_MBOX + 0x14;
static constexpr intptr_t MBOX_STATUS = VIDEOCORE_MBOX + 0x18;
static constexpr intptr_t MBOX_CONFIG = VIDEOCORE_MBOX + 0x1C;
static constexpr intptr_t MBOX_WRITE = VIDEOCORE_MBOX + 0x20;

static constexpr intptr_t V3D_BASE = PERIPHERAL_BASE + 0xC00000;

static constexpr intptr_t V3D_HUB = PERIPHERAL_BASE + 0xC00000;
static constexpr intptr_t V3D_HUB_AXICFG = V3D_HUB + 0x00;
static constexpr intptr_t V3D_HUB_UIFCFG = V3D_HUB + 0x04;
static constexpr intptr_t V3D_HUB_IDENT0 = V3D_HUB + 0x08;
static constexpr intptr_t V3D_HUB_IDENT1 = V3D_HUB + 0x0C;
static constexpr intptr_t V3D_HUB_IDENT2 = V3D_HUB + 0x10;
static constexpr intptr_t V3D_MMU_DEBUG_INFO = V3D_HUB + 0x01238;

static constexpr intptr_t V3D_CORE0 = PERIPHERAL_BASE + 0xC04000;
static constexpr intptr_t V3D_CTL_IDENT0 = V3D_CORE0 + 0x00;

static constexpr intptr_t V3D_CTL_SLCACTL = V3D_CORE0 + 0x24;
static constexpr intptr_t V3D_CTL_L2TCACTL = V3D_CORE0 + 0x30;
static constexpr intptr_t V3D_CTL_L2TFLSTA = V3D_CORE0 + 0x34;
static constexpr intptr_t V3D_CTL_L2TFLEND = V3D_CORE0 + 0x38;

// num completed / num active / have current dispatch / have queued dispatch
static constexpr intptr_t V3D_CSD_STATUS = V3D_CORE0 + 0x900;
// num workgroup x / workgroup x
static constexpr intptr_t V3D_CSD_QUEUED_CFG0 = V3D_CORE0 + 0x904;
// num workgroup y / workgroup y
static constexpr intptr_t V3D_CSD_QUEUED_CFG1 = V3D_CORE0 + 0x908;
// num workgroup z / workgroup z
static constexpr intptr_t V3D_CSD_QUEUED_CFG2 = V3D_CORE0 + 0x90C;
// overlap with prev / max sg id / batches per sg m1 / workgroups per sg / wg
// size
static constexpr intptr_t V3D_CSD_QUEUED_CFG3 = V3D_CORE0 + 0x910;
// num batch -1
static constexpr intptr_t V3D_CSD_QUEUED_CFG4 = V3D_CORE0 + 0x914;
// shader address
static constexpr intptr_t V3D_CSD_QUEUED_CFG5 = V3D_CORE0 + 0x918;
// uniform address
static constexpr intptr_t V3D_CSD_QUEUED_CFG6 = V3D_CORE0 + 0x91C;

static constexpr intptr_t V3D_TFU_CS = V3D_HUB + 0x400;
static constexpr intptr_t V3D_TFU_ICFG = V3D_HUB + 0x408;
/* Input Image Address */
static constexpr intptr_t V3D_TFU_IIA = V3D_HUB + 0x40c;
/* Input Chroma Address */
static constexpr intptr_t V3D_TFU_ICA = V3D_HUB + 0x410;
/* Input Image Stride */
static constexpr intptr_t V3D_TFU_IIS = V3D_HUB + 0x414;
/* Input Image U-Plane Address */
static constexpr intptr_t V3D_TFU_IUA = V3D_HUB + 0x418;
/* Output Image Address */
static constexpr intptr_t V3D_TFU_IOA = V3D_HUB + 0x41c;
/* Image Output Size */
static constexpr intptr_t V3D_TFU_IOS = V3D_HUB + 0x420;
/* TFU YUV Coefficient 0 */
static constexpr intptr_t V3D_TFU_COEF0 = V3D_HUB + 0x424;
/* TFU YUV Coefficient 1 */
static constexpr intptr_t V3D_TFU_COEF1 = V3D_HUB + 0x428;
/* TFU YUV Coefficient 2 */
static constexpr intptr_t V3D_TFU_COEF2 = V3D_HUB + 0x42c;
/* TFU YUV Coefficient 3 */
static constexpr intptr_t V3D_TFU_COEF3 = V3D_HUB + 0x430;

static constexpr intptr_t V3D_TFU_CRC = V3D_HUB + 0x434;

// tile mem address
static constexpr intptr_t V3D_CLE_CT0QMA = V3D_CORE0 + 0x170;
// tile size
static constexpr intptr_t V3D_CLE_CT0QMS = V3D_CORE0 + 0x174;
static constexpr intptr_t V3D_CLE_CT0QTS = V3D_CORE0 + 0x15c;
// bin command list start
static constexpr intptr_t V3D_CLE_CT0QBA = V3D_CORE0 + 0x160;
// render command list start
static constexpr intptr_t V3D_CLE_CT1QBA = V3D_CORE0 + 0x164;
// bin command list end
static constexpr intptr_t V3D_CLE_CT0QEA = V3D_CORE0 + 0x168;
// render command list end
static constexpr intptr_t V3D_CLE_CT1QEA = V3D_CORE0 + 0x16C;

static constexpr intptr_t V3D_CLE_BFC = V3D_CORE0 + 0x134;
static constexpr intptr_t V3D_CLE_RFC = V3D_CORE0 + 0x138;

static constexpr intptr_t V3D_PTB_BPCA = V3D_CORE0 + 0x00300;
static constexpr intptr_t V3D_PTB_BPCS = V3D_CORE0 + 0x00304;
static constexpr intptr_t V3D_PTB_BPOA = V3D_CORE0 + 0x00308;
static constexpr intptr_t V3D_PTB_BPOS = V3D_CORE0 + 0x0030c;

// PCM
static constexpr intptr_t PCM_BASE = PERIPHERAL_BASE + 0x203000;
static constexpr intptr_t PCM_CS_A = PCM_BASE + 0x00;
static constexpr intptr_t PCM_FIFO_A = PCM_BASE + 0x04;
static constexpr intptr_t PCM_MODE_A = PCM_BASE + 0x08;
static constexpr intptr_t PCM_RXC_A = PCM_BASE + 0x0C;
static constexpr intptr_t PCM_TXC_A = PCM_BASE + 0x10;
static constexpr intptr_t PCM_DREQ_A = PCM_BASE + 0x14;
static constexpr intptr_t PCM_INTEN_A = PCM_BASE + 0x18;
static constexpr intptr_t PCM_INTSTC_A = PCM_BASE + 0x1C;
static constexpr intptr_t PCM_GRAY = PCM_BASE + 0x20;

// PWM0
static constexpr intptr_t PWM0_BASE = PERIPHERAL_BASE + 0x20c000;
static constexpr intptr_t PWM0_CTL = PWM0_BASE + 0x00;
static constexpr intptr_t PWM0_STA = PWM0_BASE + 0x04;
static constexpr intptr_t PWM0_DMAC = PWM0_BASE + 0x08;
static constexpr intptr_t PWM0_RNG1 = PWM0_BASE + 0x10;
static constexpr intptr_t PWM0_DAT1 = PWM0_BASE + 0x14;
static constexpr intptr_t PWM0_FIF1 = PWM0_BASE + 0x18;
static constexpr intptr_t PWM0_RNG2 = PWM0_BASE + 0x20;
static constexpr intptr_t PWM0_DAT2 = PWM0_BASE + 0x24;
// PWM1
static constexpr intptr_t PWM1_BASE = PERIPHERAL_BASE + 0x20c800;
static constexpr intptr_t PWM1_CTL = PWM1_BASE + 0x00;
static constexpr intptr_t PWM1_STA = PWM1_BASE + 0x04;
static constexpr intptr_t PWM1_DMAC = PWM1_BASE + 0x08;
static constexpr intptr_t PWM1_RNG1 = PWM1_BASE + 0x10;
static constexpr intptr_t PWM1_DAT1 = PWM1_BASE + 0x14;
static constexpr intptr_t PWM1_FIF1 = PWM1_BASE + 0x18;
static constexpr intptr_t PWM1_RNG2 = PWM1_BASE + 0x20;
static constexpr intptr_t PWM1_DAT2 = PWM1_BASE + 0x24;

// PM
static constexpr intptr_t PM_BASE = PERIPHERAL_BASE + 0x100000;
static constexpr intptr_t PM_GRAFX = PM_BASE + 0x10C;

static constexpr intptr_t RPIVID_ASB_BASE = PERIPHERAL_BASE + 0xC11000;
static constexpr intptr_t ASB_V3D_S_CTRL = RPIVID_ASB_BASE + 0x8;
static constexpr intptr_t ASB_V3D_M_CTRL = RPIVID_ASB_BASE + 0xC;

static constexpr size_t GPIO_COUNT = 58;

namespace gpio
{

enum Function : u32
{
    INPUT = 0b000,
    OUTPUT = 0b001,
    FUNC0 = 0b100,
    FUNC1 = 0b101,
    FUNC2 = 0b110,
    FUNC3 = 0b111,
    FUNC4 = 0b011,
    FUNC5 = 0b010,
};

enum Resistor : u32
{
    PULL_NONE = 0b00,
    PULL_UP = 0b01,
    PULL_DOWN = 0b10,
};

void set(size_t idx, bool value);
void function(size_t idx, Function func);
void setPull(size_t idx, Resistor resistor);
Resistor getPull(size_t idx);
}

inline vu32& ref(intptr_t addr)
{
    return *reinterpret_cast<vu32*>(addr);
}

inline u32 read(intptr_t addr)
{
    return ref(addr);
}

inline void write(intptr_t addr, u32 value)
{
    ref(addr) = value;
}

}
