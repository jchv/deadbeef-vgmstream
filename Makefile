VGMSTREAM_SOURCES = $(wildcard vgmstream/*.c) $(wildcard vgmstream/coding/*.c) \
                    $(wildcard vgmstream/layout/*.c) $(wildcard vgmstream/meta/*.c)
DEADBEEF_ROOT = /opt/deadbeef

PKGCONFIG_DEPS = libmpg123 vorbis vorbisfile

CFLAGS = -fvisibility=hidden `pkg-config $(PKGCONFIG_DEPS) --cflags` -I$(DEADBEEF_ROOT)/include -g -O2
LIBS = `pkg-config $(PKGCONFIG_DEPS) --libs` -I$(DEADBEEF_ROOT)/lib -fPIC

all: vgm.so

vgm.so: $(VGMSTREAM_SOURCES) vgm.c
	gcc -shared -o $@ $^ $(CFLAGS) $(LIBS)

install: vgm.so
	cp vgm.so $(DEADBEEF_ROOT)/lib/deadbeef

.PHONY: all install
