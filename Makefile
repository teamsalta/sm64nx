# Makefile to rebuild SM64 split image

### Default target ###

default: all

### Build Options ###

# These options can either be changed by modifying the makefile, or
# by building with 'make SETTING=value'. 'make clean' may be required.

# Version of the game to build
VERSION ?= us
# Graphics microcode used
GRUCODE ?= f3d_old

# If NON_MATCHING is 1, define the NON_MATCHING and AVOID_UB macros when building (recommended)
NON_MATCHING ?= 0

# Build for Switch
TARGET_SWITCH ?= 0

## Debug Build

DEBUG_BUILD ?= 0

## Build for RPI

TARGET_RPI ?= 0




NON_MATCHING := 1
GRUCODE := f3dex2e
WINDOWS_BUILD := 0

ifeq ($(OS),Windows_NT)
  WINDOWS_BUILD := 1
endif



# Release

ifeq ($(VERSION),jp)
  VERSION_CFLAGS := -DVERSION_JP
  VERSION_ASFLAGS := --defsym VERSION_JP=1
  GRUCODE_CFLAGS := -DF3D_OLD
  GRUCODE_ASFLAGS := --defsym F3D_OLD=1
  TARGET := sm64.jp
else
ifeq ($(VERSION),us)
  VERSION_CFLAGS := -DVERSION_US
  VERSION_ASFLAGS := --defsym VERSION_US=1
  GRUCODE_CFLAGS := -DF3D_OLD
  GRUCODE_ASFLAGS := --defsym F3D_OLD=1
  TARGET := sm64.us
else
ifeq ($(VERSION),eu)
  $(warning Building EU is experimental and is prone to breaking. Try at your own risk.)
  VERSION_CFLAGS := -DVERSION_EU
  VERSION_ASFLAGS := --defsym VERSION_EU=1
  GRUCODE_CFLAGS := -DF3D_NEW
  GRUCODE_ASFLAGS := --defsym F3D_NEW=1
  TARGET := sm64.eu
else
  $(error unknown version "$(VERSION)")
endif
endif
endif

# Microcode

ifeq ($(GRUCODE),f3dex) # Fast3DEX
  GRUCODE_CFLAGS := -DF3DEX_GBI
  GRUCODE_ASFLAGS := --defsym F3DEX_GBI_SHARED=1 --defsym F3DEX_GBI=1
  TARGET := $(TARGET).f3dex
  COMPARE := 0
else
ifeq ($(GRUCODE), f3dex2) # Fast3DEX2
  GRUCODE_CFLAGS := -DF3DEX_GBI_2
  GRUCODE_ASFLAGS := --defsym F3DEX_GBI_SHARED=1 --defsym F3DEX_GBI_2=1
  TARGET := $(TARGET).f3dex2
  COMPARE := 0
else
ifeq ($(GRUCODE), f3dex2e) # Fast3DEX2 Extended (for PC)
  GRUCODE_CFLAGS := -DF3DEX_GBI_2E
  TARGET := $(TARGET).f3dex2e
  COMPARE := 0
else
ifeq ($(GRUCODE),f3d_new) # Fast3D 2.0H (Shindou)
  GRUCODE_CFLAGS := -DF3D_NEW
  GRUCODE_ASFLAGS := --defsym F3D_NEW=1
  TARGET := $(TARGET).f3d_new
  COMPARE := 0
else
ifeq ($(GRUCODE),f3dzex) # Fast3DZEX (2.0J / Animal Forest - Dōbutsu no Mori)
  $(warning Fast3DZEX is experimental. Try at your own risk.)
  GRUCODE_CFLAGS := -DF3DEX_GBI_2
  GRUCODE_ASFLAGS := --defsym F3DEX_GBI_SHARED=1 --defsym F3DZEX_GBI=1
  TARGET := $(TARGET).f3dzex
  COMPARE := 0
endif
endif
endif
endif
endif

ifeq ($(NON_MATCHING),1)
  VERSION_CFLAGS := $(VERSION_CFLAGS) -DNON_MATCHING -DAVOID_UB
  COMPARE := 0
endif


################ Target Executable and Sources ###############

# BUILD_DIR is location where all build artifacts are placed
BUILD_DIR_BASE := build

ifeq ($(TARGET_SWITCH),1)
  BUILD_DIR := $(BUILD_DIR_BASE)/$(VERSION)_nx
else
  BUILD_DIR := $(BUILD_DIR_BASE)/$(VERSION)_pc
endif

LIBULTRA := $(BUILD_DIR)/libultra.a
ifeq ($(TARGET_WEB),1)
EXE := $(BUILD_DIR)/$(TARGET).html
else ifeq ($(WINDOWS_BUILD),1)
EXE := $(BUILD_DIR)/$(TARGET).exe
else
EXE := $(BUILD_DIR)/$(TARGET)
endif
ROM := $(BUILD_DIR)/$(TARGET).z64
ELF := $(BUILD_DIR)/$(TARGET).elf
LD_SCRIPT := sm64.ld
MIO0_DIR := $(BUILD_DIR)/bin
TEXTURE_DIR := textures
ACTOR_DIR := actors
LEVEL_DIRS := $(patsubst levels/%,%,$(dir $(wildcard levels/*/header.h)))

# Directories containing source files
SRC_DIRS := src src/engine src/game src/audio src/menu src/buffers actors levels bin data assets src/controller src/player
ASM_DIRS := lib

SRC_DIRS := $(SRC_DIRS) src/pc src/pc/gfx src/pc/audio src/pc/controller
ASM_DIRS :=

BIN_DIRS := bin bin/$(VERSION)

ULTRA_SRC_DIRS := lib/src lib/src/math
ULTRA_ASM_DIRS := lib/asm lib/data
ULTRA_BIN_DIRS := lib/bin

MIPSISET := -mips2 -32

##Build Type

ifeq ($(TARGET_SWITCH), 1)
OPT_FLAGS := -O3
else
OPT_FLAGS := -O2
endif


ifeq ($(DEBUG_BUILD), 1)
OPT_FLAGS := -g
OPT_FLAGS := -DDEBUG
endif


ifeq ($(TARGET_RPI), 1)
OPT_FLAGS := -O3 
endif




# File dependencies and variables for specific files
#include Makefile.split

# Source code files
LEVEL_CXX_FILES := $(wildcard levels/*/leveldata.cpp) $(wildcard levels/*/script.cpp) $(wildcard levels/*/geo.cpp)
C_FILES		:= $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
CXX_FILES	:= $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp)) $(LEVEL_CXX_FILES)
S_FILES := $(foreach dir,$(ASM_DIRS),$(wildcard $(dir)/*.s))
ULTRA_C_FILES := $(foreach dir,$(ULTRA_SRC_DIRS),$(wildcard $(dir)/*.c))

#GENERATED_C_FILES :=  $(addprefix $(BUILD_DIR)/bin/,$(addsuffix _skybox.c,$(notdir $(basename $(wildcard textures/skyboxes/*.png)))))



ULTRA_C_FILES_SKIP := \
sqrtf.c \
string.c \
sprintf.c \
_Printf.c \
kdebugserver.c \
osInitialize.c \
osCreateThread.c \
osDestroyThread.c \
osStartThread.c \
osSetThreadPri.c \
osPiStartDma.c \
osPiRawStartDma.c \
osPiRawReadIo.c \
osPiGetCmdQueue.c \
osJamMesg.c \
osSendMesg.c \
osRecvMesg.c \
osSetEventMesg.c \
osTimer.c \
osSetTimer.c \
osSetTime.c \
osCreateViManager.c \
osViSetSpecialFeatures.c \
osVirtualToPhysical.c \
osViBlack.c \
osViSetEvent.c \
osViSetMode.c \
osViSwapBuffer.c \
osSpTaskLoadGo.c \
osCreatePiManager.c \
osGetTime.c \
osEepromProbe.c \
osEepromWrite.c \
osEepromLongWrite.c \
osEepromRead.c \
osEepromLongRead.c \
osContInit.c \
osContStartReadData.c \
osAiGetLength.c \
osAiSetFrequency.c \
osAiSetNextBuffer.c \
__osViInit.c \
__osSyncPutChars.c \
__osAtomicDec.c \
__osSiRawStartDma.c \
__osViSwapContext.c \
__osViGetCurrentContext.c

C_FILES := $(filter-out src/game/main.c,$(C_FILES))
ULTRA_C_FILES := $(filter-out $(addprefix lib/src/,$(ULTRA_C_FILES_SKIP)),$(ULTRA_C_FILES))

# Object files
O_FILES := $(foreach file,$(C_FILES),$(BUILD_DIR)/$(file:.c=.o)) \
           $(foreach file,$(CXX_FILES),$(BUILD_DIR)/$(file:.cpp=.o)) \
           $(foreach file,$(S_FILES),$(BUILD_DIR)/$(file:.s=.o))

ULTRA_O_FILES := $(foreach file,$(ULTRA_S_FILES),$(BUILD_DIR)/$(file:.s=.o)) \
                 $(foreach file,$(ULTRA_C_FILES),$(BUILD_DIR)/$(file:.c=.o))

# Automatic dependency files
DEP_FILES := $(O_FILES:.o=.d) $(ULTRA_O_FILES:.o=.d) $(BUILD_DIR)/$(LD_SCRIPT).d

# Files with GLOBAL_ASM blocks
ifneq ($(NON_MATCHING),1)
GLOBAL_ASM_C_FILES != grep -rl 'GLOBAL_ASM(' $(wildcard src/audio/*.c) $(wildcard src/game/*.c)
GLOBAL_ASM_O_FILES = $(foreach file,$(GLOBAL_ASM_C_FILES),$(BUILD_DIR)/$(file:.c=.o))
GLOBAL_ASM_DEP = $(BUILD_DIR)/src/audio/non_matching_dep
endif

# Segment elf files
SEG_FILES := $(SEGMENT_ELF_FILES) $(ACTOR_ELF_FILES) $(LEVEL_ELF_FILES)

##################### Compiler Options #######################
INCLUDE_CFLAGS := -I include -I $(BUILD_DIR) -I $(BUILD_DIR)/include -I src -I .
ENDIAN_BITWIDTH := $(BUILD_DIR)/endian-and-bitwidth

ifeq ($(TARGET_SWITCH), 1)

ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>/devkitpro")
endif

PATH := /opt/devkitpro/portlibs/switch/bin:/opt/devkitpro/tools/bin:/opt/devkitpro/devkitA64/bin:$(PATH)

$(info $(PATH))

PORTLIBS ?= $(DEVKITPRO)/portlibs/switch
LIBNX ?= $(DEVKITPRO)/libnx

APP_TITLE := Super Mario 64 NX
APP_AUTHOR := Some Autists
APP_VERSION := 1_$(VERSION)
APP_ICON := icon.jpg
ROMFS	:=	romfs

AS := aarch64-none-elf-as
CC := aarch64-none-elf-gcc
CXX := aarch64-none-elf-g++
LD := aarch64-none-elf-g++
CPP := aarch64-none-elf-cpp -P
OBJDUMP := aarch64-none-elf-objdump
OBJCOPY := aarch64-none-elf-objcopy
STRIP := aarch64-none-elf-strip

ifneq ($(ROMFS),)
	export NROFLAGS += --romfsdir=$(CURDIR)/$(ROMFS)
endif

ARCH := -march=armv8-a+crc+crypto+simd -mtune=cortex-a57 -mtp=soft -ftls-model=local-exec -fPIC
CC_CHECK := $(CC) -fsyntax-only -fsigned-char $(ARCH) $(INCLUDE_CFLAGS) -Wall -Wextra -Wno-format-security $(VERSION_CFLAGS) $(GRUCODE_CFLAGS) `sdl2-config --cflags` -DUSE_SDL=2 -D__SWITCH__=1 -Wno-narrowing -fpermissive -std=gnu++17 -DBUILD_NRO
CFLAGS := $(ARCH) $(OPT_FLAGS) $(INCLUDE_CFLAGS) $(VERSION_CFLAGS) $(GRUCODE_CFLAGS) -fno-strict-aliasing -fwrapv `sdl2-config --cflags` -DUSE_SDL=2 -D__SWITCH__=1 -Wno-narrowing -fpermissive -std=gnu++17 -DBUILD_NRO
ASFLAGS := -march=armv8-a+crc+crypto -I include -I $(BUILD_DIR) $(VERSION_ASFLAGS)
LDFLAGS := -specs=$(LIBNX)/switch.specs $(ARCH) -no-pie -L$(LIBNX)/lib -L$(PORTLIBS)/lib -lSDL2 -lEGL -lGLESv2 -lglapi -ldrm_nouveau -lnx -lm -lzstd -lz -lstdc++

else # TARGET_SWITCH

AS := as

CC := gcc
CXX := g++

ifeq ($(WINDOWS_BUILD),1)
  LD := $(CXX)
else
  LD := $(CC)
  
endif
CPP := cpp -P
OBJDUMP := objdump
OBJCOPY := objcopy

ifeq ($(WINDOWS_BUILD),1)
CC_CHECK := $(CC) -fsyntax-only -fsigned-char $(INCLUDE_CFLAGS) -Wall -Wextra -Wno-format-security $(VERSION_CFLAGS) $(GRUCODE_CFLAGS) `sdl2-config --cflags` -Wno-narrowing -fpermissive -std=gnu++17 -DGLEW_STATIC
CFLAGS := $(OPT_FLAGS) $(INCLUDE_CFLAGS) $(VERSION_CFLAGS) $(GRUCODE_CFLAGS) -fno-strict-aliasing -fwrapv `sdl2-config --cflags` -Wno-narrowing -fpermissive -std=gnu++17 -DGLEW_STATIC
else ifeq ($(OS),Windows_NT)
# mingw
CC_CHECK := $(CC) -fsyntax-only -fsigned-char $(INCLUDE_CFLAGS) -Wall -Wextra -Wno-format-security $(VERSION_CFLAGS) $(GRUCODE_CFLAGS) `sdl2-config --cflags` -DUSE_SDL=2 -Wno-narrowing -fpermissive -std=gnu++17 -DGLEW_STATIC
CFLAGS := $(OPT_FLAGS) $(INCLUDE_CFLAGS) $(VERSION_CFLAGS) $(GRUCODE_CFLAGS) -fno-strict-aliasing -fwrapv `sdl2-config --cflags` -DUSE_SDL=2 -Wno-narrowing -fpermissive -std=gnu++17 -DGLEW_STATIC
else
CC_CHECK := $(CC) -fsyntax-only -fsigned-char $(INCLUDE_CFLAGS) -Wall -Wextra -Wno-format-security $(VERSION_CFLAGS) $(GRUCODE_CFLAGS) `sdl2-config --cflags` -Wno-narrowing -fpermissive -std=gnu++17 -DGLEW_STATIC
CFLAGS := $(OPT_FLAGS) $(INCLUDE_CFLAGS) $(VERSION_CFLAGS) $(GRUCODE_CFLAGS) -fno-strict-aliasing -fwrapv `sdl2-config --cflags` -Wno-narrowing -fpermissive -std=gnu++17 -DGLEW_STATIC
endif

ASFLAGS := -I include -I $(BUILD_DIR) $(VERSION_ASFLAGS)


ifeq ($(WINDOWS_BUILD),1)
LDFLAGS := -static -lm -lglew32 -lopengl32 `sdl2-config --static-libs` -no-pie -lpthread -static-libgcc -lzstd -lz -lole32 -loleaut32 -limm32 -lversion -lwinmm -lsetupapi
else
ifeq ($(OS),Windows_NT)
# mingw
LDFLAGS := -static -lm -lglew32 -lopengl32 `sdl2-config --static-libs` -no-pie -lpthread -static-libgcc -lzstd -lole32 -loleaut32 -limm32 -lversion -lwinmm -lsetupapi
else
LDFLAGS := -static -lm -lGL `sdl2-config --static-libs` -no-pie -lpthread -lasound -lX11 -lXrandr -lpulse
endif
endif

endif

######################## Targets #############################

ifeq ($(TARGET_SWITCH),1)
all: $(EXE).nro
else
all: $(EXE)
endif

clean:
	$(RM) -r $(BUILD_DIR_BASE)

distclean:
	$(RM) -r $(BUILD_DIR_BASE)

libultra: $(BUILD_DIR)/libultra.a

asm/boot.s: $(BUILD_DIR)/lib/bin/ipl3_font.bin


ALL_DIRS := $(BUILD_DIR) $(addprefix $(BUILD_DIR)/,$(SRC_DIRS) $(ASM_DIRS) $(ULTRA_SRC_DIRS) $(ULTRA_ASM_DIRS) $(ULTRA_BIN_DIRS) $(BIN_DIRS) $(TEXTURE_DIRS) $(TEXT_DIRS) $(addprefix levels/,$(LEVEL_DIRS)) include) $(MIO0_DIR) $(addprefix $(MIO0_DIR)/,$(VERSION))

# Make sure build directory exists before compiling anything
DUMMY != mkdir -p $(ALL_DIRS)

################################################################


# Source code
$(BUILD_DIR)/src/audio/%.o: OPT_FLAGS := -O2 -Wo,-loopunroll,0
$(BUILD_DIR)/src/audio/load.o: OPT_FLAGS := -O2 -framepointer -Wo,-loopunroll,0
$(BUILD_DIR)/lib/src/%.o: OPT_FLAGS :=
$(BUILD_DIR)/lib/src/math/ll%.o: MIPSISET := -mips3 -32
$(BUILD_DIR)/lib/src/math/%.o: OPT_FLAGS := -O2
$(BUILD_DIR)/lib/src/math/ll%.o: OPT_FLAGS :=
$(BUILD_DIR)/lib/src/ldiv.o: OPT_FLAGS := -O2
$(BUILD_DIR)/lib/src/string.o: OPT_FLAGS := -O2
$(BUILD_DIR)/lib/src/gu%.o: OPT_FLAGS := -O3
$(BUILD_DIR)/lib/src/al%.o: OPT_FLAGS := -O3


# Rebuild files with 'GLOBAL_ASM' if the NON_MATCHING flag changes.
$(GLOBAL_ASM_O_FILES): $(GLOBAL_ASM_DEP).$(NON_MATCHING)
$(GLOBAL_ASM_DEP).$(NON_MATCHING):
	@rm -f $(GLOBAL_ASM_DEP).*
	touch $@

$(BUILD_DIR)/%.o: %.cpp
	@$(CXX) -fsyntax-only $(CFLAGS) -MMD -MP -MT $@ -MF $(BUILD_DIR)/$*.d $<
	$(CXX) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR)/%.o: %.c
	@$(CC_CHECK) -MMD -MP -MT $@ -MF $(BUILD_DIR)/$*.d $<
	$(CC) -c $(CFLAGS) -o $@ $<


$(BUILD_DIR)/%.o: $(BUILD_DIR)/%.c
	@$(CC_CHECK) -MMD -MP -MT $@ -MF $(BUILD_DIR)/$*.d $<
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR)/%.o: %.s
	$(AS) $(ASFLAGS) -MD $(BUILD_DIR)/$*.d -o $@ $<


$(EXE): $(O_FILES) $(MIO0_FILES:.mio0=.o) $(ULTRA_O_FILES)
	$(LD) -L $(BUILD_DIR) -o $@ $(O_FILES) $(ULTRA_O_FILES) $(LDFLAGS)

ifeq ($(TARGET_SWITCH), 1)


%.nro: %.stripped %.nacp
	@elf2nro $< $@ --nacp=$*.nacp --icon=$(APP_ICON) --romfsdir=$(ROMFS)
	@echo built ... $(notdir $@)

%.nacp:
	@nacptool --create "$(APP_TITLE)" "$(APP_AUTHOR)" "$(APP_VERSION)" $@ $(NACPFLAGS)
	@echo built ... $(notdir $@)

%.stripped: %
	@$(STRIP) -o $@ $<
	@echo stripped ... $(notdir $<)

endif



.PHONY: all clean distclean default diff test load libultra
.PRECIOUS: $(BUILD_DIR)/bin/%.elf $(BUILD_DIR)/%
.DELETE_ON_ERROR:

# Remove built-in rules, to improve performance
MAKEFLAGS += --no-builtin-rules

-include $(DEP_FILES)

print-% : ; $(info $* is a $(flavor $*) variable set to [$($*)]) @true
