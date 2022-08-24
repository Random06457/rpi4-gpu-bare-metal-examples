include prettyprint.mk

BOOT_MNT 	?=
ROOTFS_MNT 	?=

ifeq ($(BOOT_MNT),)
$(error "Please set BOOT_MNT to your boot parition. e.g. /media/user/boot")
endif

ifeq ($(ROOTFS_MNT),)
$(error "Please set ROOTFS_MNT to your rootfs parition. e.g. /media/user/rootfs")
endif


BIN_PREFIX	:= aarch64-linux-gnu-
CXX			:= $(BIN_PREFIX)g++
CC			:= $(BIN_PREFIX)gcc
AS			:= $(BIN_PREFIX)as
OBJCOPY		:= $(BIN_PREFIX)objcopy
LD			:= $(BIN_PREFIX)ld
FORMAT 		:= clang-format-14

USE_LD		:= gold

TARGET		:= rpi4-gpu-bare-metal
TARGET_IMG	:= $(TARGET).img
TARGET_ELF	:= $(TARGET).elf

BUILD := build

WARN =	-Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function
OPT = -O0

CPU := -mcpu=cortex-a72+fp+simd+crypto+crc

STD = c++20

CXXFLAGS = \
	-std=$(STD) \
	$(OPT) \
	$(WARN) \
	$(CPU) \
	-ffreestanding \
	-fno-pie \
	-fno-unwind-tables \
	-fno-exceptions \
	-mstrict-align \
	-fno-use-cxa-atexit \
	-fno-rtti

CFLAGS = \
	-std=c11 \
	$(OPTFLAGS) \
	$(WARN) \
	$(CPU) \
	-ffreestanding \
	-fno-pie

LDFLAGS = \
	$(CPU) \
	-Wl,-Map=$(BUILD)/$(TARGET).map \
	-z max-page-size=0x1000 \
	-fno-unwind-tables \
	-nostartfiles \
	-nostdlib

ASFLAGS :=

CPPFLAGS := -MMD

DEBUG ?= 1
ifneq ($(DEBUG),0)
	CXXFLAGS += -g
	CFLAGS += -g
	ASFLAGS += -g
endif

LTO ?= 0
ifneq ($(LTO),0)
	CXXFLAGS += -flto
	CFLAGS += -flto
	LDFLAGS += -flto
endif

LDSCRIPT := $(addprefix -T, \
	linker.ld \
)

INC_DIRS := $(addprefix -I, \
	src \
	src/3rd-party \
)

LIBS := $(addprefix -l,	\
)

LIB_DIRS := $(addprefix -L, \
)

CXX_FILES := \
	src/arch/aarch64.cpp \
	src/boot/init.cpp \
	src/boot/exception_handler.cpp \
	src/device/v3d/mbox.cpp \
	src/device/v3d/v3d_qpu.cpp \
	src/device/mmio.cpp \
	src/device/pm.cpp \
	src/device/uart.cpp \
	src/stdlib/math/trigo.cpp \
	src/kernel/examples/gl_rainbow_triangle.cpp \
	src/kernel/gfx/drawer.cpp \
	src/kernel/gfx/font.cpp \
	src/kernel/gfx/framebuffer.cpp \
	src/kernel/gfx/text_writer.cpp \
	src/kernel/heap.cpp \
	src/kernel/kernel.cpp \
	src/kernel/logging.cpp \
	src/kernel/math_util.cpp \
	src/kernel/panic.cpp \
	src/stdlib/assert.cpp \
	src/stdlib/cstring.cpp \
	src/stdlib/exception.cpp \
	src/stdlib/memory.cpp \
	src/stdlib/stubs.cpp

C_FILES := \
	src/stdlib/printf.c


S_FILES := \
	src/boot/boot.S \
	src/boot/exception_vector.S \
	src/stdlib/math/math_aarch64.S

O_FILES := \
	$(CXX_FILES:%.cpp=$(BUILD)/%.o) \
	$(S_FILES:%.S=$(BUILD)/%.o) \
	$(C_FILES:%.c=$(BUILD)/%.o)

D_FILES := $(O_FILES:%.o=%.d)


SRC_DIRS := $(shell find src -type d)
$(shell mkdir -p $(BUILD) $(SRC_DIRS:%=$(BUILD)/%))

all: $(BUILD)/$(TARGET_IMG)

clean:
	rm -rf $(BUILD)

install: all
	cp $(BUILD)/$(TARGET_IMG) $(BOOT_MNT)/kernel8.img
	eject $(BOOT_MNT)
	eject $(ROOTFS_MNT)


FMT_FILES := $(shell find src -type f -name "*.[c\|h]pp")

format:
	$(FORMAT) -i $(FMT_FILES)

format-check:
	$(FORMAT) --dry-run -Werror $(FMT_FILES)

$(BUILD)/$(TARGET_ELF): $(O_FILES)

$(BUILD)/%.o : %.cpp
	$(call printtask,Compiling CXX,$<)
	$(V)$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(INC_DIRS) -c $< -o $@

$(BUILD)/%.o : %.c
	$(call printtask,Compiling C,$<)
	$(V)$(CC) $(CFLAGS) $(CPPFLAGS) $(INC_DIRS) -c $< -o $@

$(BUILD)/%.o : %.S
	$(call printtask,Assembling,$<)
	$(V)$(CC) $(ASFLAGS) -c $< -o $@

%.elf:
	$(call printtask,Linking,$@)
	$(V)$(CXX) -fuse-ld=$(USE_LD) $(LDFLAGS) $(LDSCRIPT) $^ -o $@

%.img: %.elf
	$(call printtask,Objcopy IMG,$@)
	$(V)$(OBJCOPY) -O binary $< $@

.PHONY: all clean install format format-check

-include $(D_FILES)