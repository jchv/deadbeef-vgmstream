// vgmstream plugin for DeaDBeeF
// Broken in two parts: need a SF implementation for VGMStream, and a decoder
// implementation for Deadbeef.

#include <stdlib.h>
#include <string.h>

#include <base/plugins.h>
#include <deadbeef/deadbeef.h>
#include <streamfile.h>
#include <vgmstream.h>

#include "extensions.h"

#define trace(...)                                                             \
  {                                                                            \
    fprintf(stderr, __VA_ARGS__);                                              \
  }

static DB_decoder_t plugin;
static DB_functions_t *deadbeef;

#define DEFAULT_LOOP_COUNT 2.0
#define DEFAULT_FADE_DURATION 10.0
#define DEFAULT_FADE_DELAY 10.0

#define DEFAULT_LOOP_COUNT_STR "2.0"
#define DEFAULT_FADE_DURATION_STR "10.0"
#define DEFAULT_FADE_DELAY_STR "10.0"

static int conf_loop_single = 0;
static double conf_loop_count = DEFAULT_LOOP_COUNT;
static double conf_fade_duration = DEFAULT_FADE_DURATION;
static double conf_fade_delay = DEFAULT_FADE_DELAY;

static const char settings_dlg[] =
    "property \"Loop count\" entry vgm.loopcount " DEFAULT_LOOP_COUNT_STR ";\n"
    "property \"Fade duration (seconds)\" entry "
    "vgm.fadeduration " DEFAULT_FADE_DURATION_STR ";\n"
    "property \"Fade delay (seconds)\" entry "
    "vgm.fadedelay " DEFAULT_FADE_DELAY_STR ";\n";

/*
 * VGMStream Streamfile implementation for Deadbeef
 */

typedef struct _DBSTREAMFILE {
  STREAMFILE sf;
  DB_FILE *file;
  off_t offset;
  char name[260];
} DBSTREAMFILE;

static void dbsf_seek(DBSTREAMFILE *this, off_t offset) {
  if (!this->file) {
    return;
  }
  if (deadbeef->fseek(this->file, offset, SEEK_SET) == 0) {
    this->offset = offset;
  } else {
    this->offset = deadbeef->ftell(this->file);
  }
}

static off_t dbsf_get_size(DBSTREAMFILE *this) {
  if (!this->file) {
    return 0;
  }
  return deadbeef->fgetlength(this->file);
}

static off_t dbsf_get_offset(DBSTREAMFILE *this) { return this->offset; }

static void dbsf_get_name(DBSTREAMFILE *this, char *buffer, size_t length) {
  strncpy(buffer, this->name, length);
  buffer[length - 1] = '\0';
}

static size_t dbsf_read(DBSTREAMFILE *this, uint8_t *dest, off_t offset,
                        size_t length) {
  size_t read;
  if (!this->file) {
    return 0;
  }
  if (this->offset != offset) {
    dbsf_seek(this, offset);
  }
  read = deadbeef->fread(dest, 1, length, this->file);
  if (read > 0) {
    this->offset += read;
  }
  return read;
}

static void dbsf_close(DBSTREAMFILE *this) {
  if (this->file) {
    deadbeef->fclose(this->file);
    this->file = NULL;
  }
  free(this);
}

static STREAMFILE *dbsf_create_from_path(const char *path);
static STREAMFILE *dbsf_open(DBSTREAMFILE *this, const char *const filename,
                             size_t buffersize) {
  if (!filename)
    return NULL;
  return dbsf_create_from_path(filename);
}

static STREAMFILE *dbsf_create(DB_FILE *file, const char *path) {
  DBSTREAMFILE *streamfile = malloc(sizeof(DBSTREAMFILE));

  if (!streamfile)
    return NULL;

  memset(streamfile, 0, sizeof(DBSTREAMFILE));
  streamfile->sf.read = (void *)dbsf_read;
  streamfile->sf.get_size = (void *)dbsf_get_size;
  streamfile->sf.get_offset = (void *)dbsf_get_offset;
  streamfile->sf.get_name = (void *)dbsf_get_name;
  streamfile->sf.open = (void *)dbsf_open;
  streamfile->sf.close = (void *)dbsf_close;
  streamfile->file = file;
  streamfile->offset = 0;
  strncpy(streamfile->name, path, sizeof(streamfile->name));

  return &streamfile->sf;
}

STREAMFILE *dbsf_create_from_path(const char *path) {
  DB_FILE *file = deadbeef->fopen(path);

  if (!file) {
    /* Allow vgmstream's virtual files. */
    if (!vgmstream_is_virtual_filename(path)) {
      return NULL;
    }
  }

  return dbsf_create(file, path);
}

VGMSTREAM *init_vgmstream_from_dbfile(const char *path, int subsong) {
  STREAMFILE *sf;
  VGMSTREAM *vgm;

  sf = dbsf_create_from_path(path);
  if (!sf)
    return NULL;

  sf->stream_index = subsong;

  vgm = init_vgmstream_from_STREAMFILE(sf);
  if (!vgm)
    goto err1;

  return vgm;
err1:
  dbsf_close((DBSTREAMFILE *)sf);
  return NULL;
}

/*
 * VGMStream decoder plugin for Deadbeef
 */

typedef struct {
  DB_fileinfo_t info;
  VGMSTREAM *s;
  int can_loop;
  int position;
  int fadesamples;
  int totalsamples;
} vgm_info_t;

#define COPYRIGHT_STR                                                          \
  "deadbeef-vgmstream\n"                                                       \
  "Copyright (c) 2014-2025 John Chadwick <johnwchadwick@gmail.com>\n"          \
  "\n"                                                                         \
  "Licensed under the same terms as vgmstream (see below.)\n"                  \
  "\n"                                                                         \
  "vgmstream\n"                                                                \
  "Copyright (c) 2008-2010 Adam Gashlin, Fastelbja, Ronny Elfert\n"            \
  "Portions Copyright (c) 2004-2008, Marko Kreen\n"                            \
  "Portions Copyright 2001-2007  jagarl / Kazunori Ueno "                      \
  "<jagarl@creator.club.ne.jp>\n"                                              \
  "Portions Copyright (c) 1998, Justin Frankel/Nullsoft Inc.\n"                \
  "Portions Copyright (C) 2006 Nullsoft, Inc.\n"                               \
  "Portions Copyright (c) 2005-2007 Paul Hsieh\n"                              \
  "Portions Public Domain originating with Sun Microsystems\n"                 \
  "\n"                                                                         \
  "Permission to use, copy, modify, and distribute this software for any\n"    \
  "purpose with or without fee is hereby granted, provided that the above\n"   \
  "copyright notice and this permission notice appear in all copies.\n"        \
  "\n"                                                                         \
  "THE SOFTWARE IS PROVIDED \"AS IS\" AND THE AUTHOR DISCLAIMS ALL "           \
  "WARRANTIES\n"                                                               \
  "WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF\n"         \
  "MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR\n"  \
  "ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES\n"   \
  "WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN\n"    \
  "ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF\n"  \
  "OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.\n"

static DB_fileinfo_t *vgm_open(uint32_t hints) {
  DB_fileinfo_t *_info = malloc(sizeof(vgm_info_t));
  vgm_info_t *info = (vgm_info_t *)_info;
  memset(info, 0, sizeof(vgm_info_t));
  info->can_loop = (hints & DDB_DECODER_HINT_CAN_LOOP) != 0;
  return _info;
}

static int vgm_init(DB_fileinfo_t *_info, DB_playItem_t *it) {
  vgm_info_t *info = (vgm_info_t *)_info;
  deadbeef->pl_lock();
  char *fname = strdup(deadbeef->pl_find_meta(it, ":URI"));
  int subsong = deadbeef->pl_find_meta_int(it, ":TRACKNUM", 0);
  deadbeef->pl_unlock();

  info->s = init_vgmstream_from_dbfile(fname, subsong != 0 ? subsong : 1);
  free(fname);

  info->fadesamples = conf_fade_duration * info->s->sample_rate;
  info->totalsamples = get_vgmstream_play_samples(
      conf_loop_count, conf_fade_duration, conf_fade_delay, info->s);

  _info->readpos = 0;
  _info->plugin = &plugin;
  _info->fmt.bps = 16;
  _info->fmt.channels = info->s->channels;
  _info->fmt.samplerate = info->s->sample_rate;

  int i;
  for (i = 0; i < _info->fmt.channels; i++) {
    _info->fmt.channelmask |= 1 << i;
  }

  return 0;
}

static void vgm_free(DB_fileinfo_t *_info) {
  vgm_info_t *info = (vgm_info_t *)_info;
  close_vgmstream(info->s);
  if (info) {
    free(info);
  }
}

static int vgm_read(DB_fileinfo_t *_info, char *bytes, int size) {
  int i, j;
  vgm_info_t *info = (vgm_info_t *)_info;
  int sample_size = _info->fmt.channels * sizeof(int16_t);
  int32_t sample_count = size / sample_size,
          fade_start = info->totalsamples - info->fadesamples,
          fade_end = info->totalsamples, fade_samples = info->fadesamples;

  int terminate = !conf_loop_single || !info->can_loop || !info->s->loop_flag;

  if (terminate) {
    /* Past the end? Let deadbeef know. */
    if (info->position >= info->totalsamples) {
      return 0;
    }
  }

  /* Do the actual rendering. */
  sample_count = render_vgmstream((int16_t *)bytes, sample_count, info->s);

  /* Update position. */
  info->position += sample_count;
  _info->readpos = (float)info->position / (float)_info->fmt.samplerate;

  /* If we are overlapping any piece after the fade starts, we must fade out
   * here.
   * TODO: Code should be refactored to be more clear.
   */
  if (terminate && (info->position > fade_start ||
                    info->position + sample_count > fade_start)) {
    int16_t *buf = (int16_t *)bytes;
    for (i = 0; i < sample_count; ++i) {
      int pos = i + info->position;
      if (pos > fade_start) {
        int samples_into_fade = pos - fade_start;
        double fadedness =
            (double)(fade_samples - samples_into_fade) / fade_samples;
        for (j = 0; j < info->s->channels; j++) {
          buf[i * info->s->channels + j] =
              buf[i * info->s->channels + j] * fadedness;
        }
      } else if (pos > fade_end) {
        break;
      }
    }
  }

  return sample_count * sample_size;
}

static int vgm_seek_sample(DB_fileinfo_t *_info, int sample) {
  int i;
  vgm_info_t *info = (vgm_info_t *)_info;
  int16_t samples[0x1000 * _info->fmt.channels];

  /* Reset stream to go backwards */
  if (sample < info->position) {
    reset_vgmstream(info->s);
    info->position = 0;
    _info->readpos = 0;
  }

  int seek = sample - info->position;

  /* Render out 0x1000 sample chunks and discard them */
  while (seek > 0x1000) {
    vgm_read(_info, (char *)samples,
             0x1000 * _info->fmt.channels * sizeof(int16_t));
    seek -= 0x1000;
  }

  /* Read the final chunk of < 0x1000 samples (or not) */
  if (seek > 0) {
    vgm_read(_info, (char *)samples,
             seek * _info->fmt.channels * sizeof(int16_t));
  }

  return 0;
}

static int vgm_seek(DB_fileinfo_t *_info, float time) {
  return vgm_seek_sample(_info, time * _info->fmt.samplerate);
}

static DB_playItem_t *vgm_insert_subsong(ddb_playlist_t *plt,
                                         DB_playItem_t *after,
                                         const char *fname, int subsong) {
  VGMSTREAM *vgm = init_vgmstream_from_dbfile(fname, subsong);
  if (!vgm) {
    return after;
  }

  DB_playItem_t *it = deadbeef->pl_item_alloc_init(fname, plugin.plugin.id);

  vgmstream_title_t tcfg;
  memset(&tcfg, 0, sizeof(vgmstream_title_t));
  tcfg.remove_extension = 1;

  char title[1024];

  vgmstream_get_title(title, sizeof(title), fname, vgm, &tcfg);
  deadbeef->pl_add_meta(it, "title", title);
  deadbeef->pl_replace_meta(it, ":FILETYPE", "vgm");
  deadbeef->pl_set_meta_int(it, ":TRACKNUM", subsong);
  if (vgm->loop_flag) {
    deadbeef->pl_set_meta_int(it, ":loop_start", vgm->loop_start_sample);
    deadbeef->pl_set_meta_int(it, ":loop_end", vgm->loop_end_sample);
  }
  if (vgm->num_streams > 1) {
    deadbeef->pl_set_meta_int(it, ":stream_count", vgm->num_streams);
  }
  if (vgm->stream_index > 1) {
    deadbeef->pl_set_meta_int(it, ":stream_index", vgm->stream_index);
  }
  if (vgm->stream_name != NULL && *vgm->stream_name != '\0') {
    deadbeef->pl_add_meta(it, ":stream_name", vgm->stream_name);
  }
  deadbeef->pl_set_meta_int(it, ":SAMPLERATE", vgm->sample_rate);
  deadbeef->pl_set_meta_int(it, ":CHANNELS", vgm->channels);
  deadbeef->pl_set_meta_int(it, ":BPS", 16);
  deadbeef->pl_set_meta_int(it, ":BITRATE",
                            get_vgmstream_average_bitrate(vgm) / 1000);

  size_t num_samples = get_vgmstream_play_samples(
      conf_loop_count, conf_fade_duration, conf_fade_delay, vgm);
  deadbeef->plt_set_item_duration(plt, it,
                                  (float)num_samples / vgm->sample_rate);

  after = deadbeef->plt_insert_item(plt, after, it);
  deadbeef->pl_item_unref(it);

  close_vgmstream(vgm);

  return after;
}

static DB_playItem_t *vgm_insert(ddb_playlist_t *plt, DB_playItem_t *after,
                                 const char *fname) {
  VGMSTREAM *vgm = init_vgmstream_from_dbfile(fname, 0);
  if (vgm == NULL) {
    return after;
  }

  int i, num_subsongs = vgm->num_streams > 0 ? vgm->num_streams : 1;
  int add_subsong = (num_subsongs > 1 && vgm->stream_index == 0)
                        ? -1
                        : (vgm->stream_index || 1);

  close_vgmstream(vgm);

  if (add_subsong == -1) {
    for (i = 1; i <= num_subsongs; ++i) {
      after = vgm_insert_subsong(plt, after, fname, i);
    }
  } else {
    after = vgm_insert_subsong(plt, after, fname, add_subsong);
  }

  return after;
}

static void vgm_reload_config(void) {
  conf_loop_single =
      deadbeef->conf_get_int("playback.loop", PLAYBACK_MODE_LOOP_ALL) ==
      PLAYBACK_MODE_LOOP_SINGLE;
  conf_loop_count = (double)deadbeef->conf_get_float("vgm.loopcount",
                                                     (float)DEFAULT_LOOP_COUNT);
  conf_fade_duration = (double)deadbeef->conf_get_float(
      "vgm.fadeduration", (float)DEFAULT_FADE_DURATION);
  conf_fade_delay = (double)deadbeef->conf_get_float("vgm.fadedelay",
                                                     (float)DEFAULT_FADE_DELAY);
}

static int vgm_start(void) {
  vgm_reload_config();
  return 0;
}

static int vgm_stop(void) { return 0; }

static int vgm_message(uint32_t id, uintptr_t ctx, uint32_t p1, uint32_t p2) {
  switch (id) {
  case DB_EV_CONFIGCHANGED:
    vgm_reload_config();
    break;
  }
  return 0;
}

// define plugin interface
static DB_decoder_t plugin = {
    DB_PLUGIN_SET_API_VERSION.plugin.version_major = 0,
    .plugin.version_minor = 1,
    .plugin.type = DB_PLUGIN_DECODER,
    .plugin.name = "vgmstream",
    .plugin.id = "vgm",
    .plugin.descr = "Decodes a variety of streaming video game music formats.",
    .plugin.copyright = COPYRIGHT_STR,
    .plugin.start = vgm_start,
    .plugin.stop = vgm_stop,
    .plugin.configdialog = settings_dlg,
    .plugin.message = vgm_message,
    .open = vgm_open,
    .init = vgm_init,
    .free = vgm_free,
    .read = vgm_read,
    .seek = vgm_seek,
    .seek_sample = vgm_seek_sample,
    .insert = vgm_insert,
    .exts = extension_list,
};

__attribute__((visibility("default"))) DB_plugin_t *
vgm_load(DB_functions_t *api) {
  deadbeef = api;
  return DB_PLUGIN(&plugin);
}
