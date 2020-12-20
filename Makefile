VGMSTREAM_SOURCES = $(wildcard vgmstream/src/*.c) $(wildcard vgmstream/src/coding/*.c) \
                    $(wildcard vgmstream/src/layout/*.c) $(wildcard vgmstream/src/meta/*.c) \
		    vgmstream/ext_libs/clHCA.c
DEADBEEF_ROOT = /opt/deadbeef

PKGCONFIG_DEPS = libmpg123 vorbis vorbisfile libavcodec libavformat libavutil

CFLAGS = -fvisibility=hidden `pkg-config $(PKGCONFIG_DEPS) --cflags` -I$(DEADBEEF_ROOT)/include -Ivgmstream/ext_includes -g -O2 -DVGM_USE_FFMPEG -DVGM_USE_VORBIS
LIBS = `pkg-config $(PKGCONFIG_DEPS) --libs` -I$(DEADBEEF_ROOT)/lib -fPIC

all: vgm.so

extensions.h: vgmstream/src/formats.c
	awk '/\sextension_list\[/,/}/{print}' vgmstream/src/formats.c

vgm.so: $(VGMSTREAM_SOURCES) vgm.c
	gcc -shared -o $@ $^ $(CFLAGS) $(LIBS)

install: vgm.so
	cp vgm.so $(DEADBEEF_ROOT)/lib/deadbeef

.PHONY: all install
