include ../util.mk

HOST_ENV := $(shell uname 2>/dev/null || echo Unknown)
HOST_ENV := $(patsubst MINGW%,MinGW,$(HOST_ENV))

CC           := gcc
CXX          := g++
CFLAGS       := -I. -O2 -s
LDFLAGS      := -lm
ALL_PROGRAMS := armips filesizer rncpack n64graphics n64graphics_ci mio0 slienc n64cksum textconv aifc_decode aiff_extract_codebook vadpcm_enc tabledesign extract_data_for_mio skyconv
LIBAUDIOFILE := audiofile/libaudiofile.a

OS := $(shell uname)

ifneq ($(OS), Darwin)
  ALL_PROGRAMS += flips
endif

ifeq ($(OS),Windows_NT)
  EXT := .exe
  OUTPUTS := $(ALL_PROGRAMS:=.exe)
else
  EXT :=
  OUTPUTS := $(ALL_PROGRAMS)
endif

# Only build armips from tools if it is not found on the system
ifeq ($(call find-command,armips),)
  BUILD_PROGRAMS := $(ALL_PROGRAMS)
else
  BUILD_PROGRAMS := $(filter-out armips,$(ALL_PROGRAMS))
endif

default: all

filesizer_SOURCES	:= filesizer.c

rncpack_SOURCES	:= rncpack.c

n64graphics_SOURCES := n64graphics.c utils.c
n64graphics_CFLAGS  := -DN64GRAPHICS_STANDALONE

n64graphics_ci_SOURCES := n64graphics_ci_dir/n64graphics_ci.c n64graphics_ci_dir/exoquant/exoquant.c n64graphics_ci_dir/utils.c

mio0_SOURCES := libmio0.c
mio0_CFLAGS  := -DMIO0_STANDALONE

slienc_SOURCES := slienc.c
slienc_CFLAGS :=

n64cksum_SOURCES := n64cksum.c utils.c
n64cksum_CFLAGS  := -DN64CKSUM_STANDALONE

textconv_SOURCES := textconv.c utf8.c hashtable.c

aifc_decode_SOURCES := aifc_decode.c

aiff_extract_codebook: $(LIBAUDIOFILE)
aiff_extract_codebook_SOURCES := aiff_extract_codebook.c sdk-tools/tabledesign/codebook.c sdk-tools/tabledesign/estimate.c sdk-tools/tabledesign/print.c sdk-tools/tabledesign/tabledesign.c
aiff_extract_codebook_CFLAGS  := -DEXTRACT_CODEBOOK -Iaudiofile -Wno-uninitialized
aiff_extract_codebook_LDFLAGS := -Laudiofile -laudiofile -lstdc++

tabledesign$(EXT): $(LIBAUDIOFILE)
tabledesign_SOURCES := sdk-tools/tabledesign/codebook.c sdk-tools/tabledesign/estimate.c sdk-tools/tabledesign/print.c sdk-tools/tabledesign/tabledesign.c
tabledesign_CFLAGS  := -Iaudiofile -Wno-uninitialized
tabledesign_LDFLAGS := -Laudiofile -laudiofile -lstdc++

vadpcm_enc_SOURCES := sdk-tools/adpcm/vadpcm_enc.c sdk-tools/adpcm/vpredictor.c sdk-tools/adpcm/quant.c sdk-tools/adpcm/util.c sdk-tools/adpcm/vencode.c
vadpcm_enc_CFLAGS  := -Wno-unused-result -Wno-uninitialized -Wno-sign-compare -Wno-absolute-value

extract_data_for_mio_SOURCES := extract_data_for_mio.c

skyconv_SOURCES := skyconv.c n64graphics.c utils.c
skyconv_CFLAGS := -g -I../include

armips$(EXT): CC := $(CXX)
armips_SOURCES := armips.cpp
armips_CFLAGS  := -std=gnu++11 -fno-exceptions -fno-rtti -pipe
armips_LDFLAGS := -pthread
ifeq ($(HOST_ENV),MinGW)
  armips_LDFLAGS += -municode
endif

flips$(EXT): CC := $(CXX)
flips_SOURCES := FlipsSrc/Flips.cpp
ifeq ($(OS),Windows_NT)
flips_CFLAGS := -Wall -Wextra -fopenmp -Os -flto -fuse-linker-plugin -fomit-frame-pointer -fmerge-all-constants \
-fvisibility=hidden -fno-exceptions -fno-rtti -fno-unwind-tables -fno-asynchronous-unwind-tables \
-ffunction-sections -fdata-sections -Wl,--gc-sections
else
flips_CFLAGS := -Wall -Wextra -fopenmp -Os -flto -fuse-linker-plugin -fomit-frame-pointer -fmerge-all-constants \
-fvisibility=hidden -fno-exceptions -fno-unwind-tables -fno-asynchronous-unwind-tables \
-ffunction-sections -fdata-sections -Wl,--gc-sections \
-fvisibility=hidden -fno-exceptions -fno-rtti -fno-unwind-tables -fno-asynchronous-unwind-tables \
-ffunction-sections -fdata-sections -Wl,--gc-sections  \
-Wl,-z,relro,-z,now,--as-needed,--hash-style=gnu,--relax
endif

all-except-recomp: $(LIBAUDIOFILE) $(OUTPUTS)

all: all-except-recomp

clean:
	$(RM) $(ALL_PROGRAMS)
	$(RM) UNFLoader*
	$(MAKE) -C audiofile clean

distclean: clean

define COMPILE
$(1)$$(EXT): $($1_SOURCES)
	$$(CC) $(CFLAGS) $($1_CFLAGS) $$^ -o $(1) $($1_LDFLAGS) $(LDFLAGS)
endef

$(foreach p,$(BUILD_PROGRAMS),$(eval $(call COMPILE,$(p))))

$(LIBAUDIOFILE):
	@$(MAKE) -C audiofile

.PHONY: all all-except-recomp clean distclean default
