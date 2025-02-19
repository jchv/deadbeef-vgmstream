CC ?= gcc
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
VGMSTREAM_OBJECTS := $(VGMSTREAM_SOURCES:.c=.o)
PKGCONFIG_DEPS := libmpg123 vorbis vorbisfile libavcodec libavformat libavutil
CFLAGS ?= \
	-g -O2 \
	-fvisibility=hidden \
	$(shell pkg-config $(PKGCONFIG_DEPS) --cflags)
INCLUDES ?= \
	-I$(DEADBEEF_ROOT)/include \
	-I$(VGMSTREAM_ROOT)/src \
	-I$(VGMSTREAM_ROOT)/ext_includes
DEFINES ?= \
	-DVGM_USE_FFMPEG \
	-DVGM_USE_VORBIS \
	-DVGM_USE_MPEG
LDFLAGS ?= \
	$(shell pkg-config $(PKGCONFIG_DEPS) --libs)

all: compile_flags.txt vgm.so

clean:
	find . -name "*.o" -delete
	rm vgm.so

compile_flags.txt: Makefile
	(echo $(CFLAGS) $(INCLUDES) $(DEFINES) | xargs -n1 echo) > $@

extensions.h: $(VGMSTREAM_ROOT)/src/formats.c
	awk '/ extension_list\[/,/}/{print}' $(VGMSTREAM_ROOT)/src/formats.c > $@

%.o: %.c
	$(CC) -c -o $@ $^ $(INCLUDES) $(DEFINES) $(CFLAGS) $(EXTRA_CFLAGS)

vgm.o: vgm.c extensions.h
	$(CC) -c -o $@ vgm.c $(INCLUDES) $(DEFINES) $(CFLAGS) $(EXTRA_CFLAGS)

vgm.so: $(VGMSTREAM_OBJECTS) vgm.o
	$(CC) -shared -o $@ $^ $(CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS)

install: vgm.so
	install -D vgm.so $(DEADBEEF_ROOT)/lib/deadbeef/vgm.so

install-local: vgm.so
	install -D vgm.so $(HOME)/.local/lib/deadbeef/vgm.so

.PHONY: all install install-local
