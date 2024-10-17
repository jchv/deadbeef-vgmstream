DEADBEEF_ROOT ?= /opt/deadbeef
VGMSTREAM_ROOT ?= ./vgmstream
VGMSTREAM_SOURCES := \
	$(wildcard $(VGMSTREAM_ROOT)/src/*.c) \
	$(wildcard $(VGMSTREAM_ROOT)/src/base/*.c) \
	$(wildcard $(VGMSTREAM_ROOT)/src/coding/*.c) \
	$(wildcard $(VGMSTREAM_ROOT)/src/coding/libs/*.c) \
	$(wildcard $(VGMSTREAM_ROOT)/src/layout/*.c) \
	$(wildcard $(VGMSTREAM_ROOT)/src/meta/*.c) \
	$(wildcard $(VGMSTREAM_ROOT)/src/util/*.c)
PKGCONFIG_DEPS := libmpg123 vorbis vorbisfile libavcodec libavformat libavutil
CFLAGS = \
	-g -O2 \
	-fvisibility=hidden \
	$(shell pkg-config $(PKGCONFIG_DEPS) --cflags) \
	-I$(DEADBEEF_ROOT)/include \
	-I$(VGMSTREAM_ROOT)/src \
	-I$(VGMSTREAM_ROOT)/ext_includes \
	-DVGMSTREAM_ROOT=$(VGMSTREAM_ROOT) \
	-DVGM_USE_FFMPEG \
	-DVGM_USE_VORBIS \
	-DVGM_USE_MPEG
LIBS = \
       -fPIC \
       $(shell pkg-config $(PKGCONFIG_DEPS) --libs) \
       -I$(DEADBEEF_ROOT)/lib

all: compile_flags.txt vgm.so

compile_flags.txt: Makefile
	(echo $(CFLAGS) | xargs -n1 echo) > $@

extensions.h: $(VGMSTREAM_ROOT)/src/formats.c
	awk '/\sextension_list\[/,/}/{print}' $(VGMSTREAM_ROOT)/src/formats.c > $@

vgm.so: $(VGMSTREAM_SOURCES) vgm.c extensions.h
	gcc -shared -o $@ $^ $(CFLAGS) $(LIBS)

install: vgm.so
	install -D vgm.so $(DEADBEEF_ROOT)/lib/deadbeef/vgm.so

install-local: vgm.so
	install -D vgm.so $(HOME)/.local/lib/deadbeef/vgm.so

.PHONY: all install install-local
