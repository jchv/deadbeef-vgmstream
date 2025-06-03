// vgmstream plugin for DeaDBeeF
// Broken in two parts: need a SF implementation for VGMStream, and a decoder
// implementation for Deadbeef.

#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include <base/plugins.h>
#include <deadbeef/deadbeef.h>
#include <libvgmstream.h>

#include "extensions.h"

#define trace(...)                                                             \
  {                                                                            \
    fprintf(stderr, __VA_ARGS__);                                              \
  }

static ddb_decoder2_t plugin;
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
static int conf_force_looping = 0;

static const char settings_dlg[] =
    "property \"Loop count\" entry vgm.loopcount " DEFAULT_LOOP_COUNT_STR ";\n"
    "property \"Fade duration (seconds)\" entry "
    "vgm.fadeduration " DEFAULT_FADE_DURATION_STR ";\n"
    "property \"Fade delay (seconds)\" entry "
    "vgm.fadedelay " DEFAULT_FADE_DELAY_STR ";\n"
    "property \"Force looping\" checkbox vgm.forcelooping 0;\n";

/*
 * VGMStream Streamfile implementation for Deadbeef
 */

typedef struct {
  DB_FILE *file;
  off_t offset;
  char *name;
} db_priv_t;

static void dbsf_seek(void *user_data, off_t offset) {
  db_priv_t *priv = (db_priv_t *)user_data;
  if (!priv || !priv->file) {
    return;
  }
  if (deadbeef->fseek(priv->file, offset, SEEK_SET) == 0) {
    priv->offset = offset;
  } else {
    priv->offset = deadbeef->ftell(priv->file);
  }
}

static int64_t dbsf_get_size(void *user_data) {
  db_priv_t *priv = (db_priv_t *)user_data;
  if (!priv || !priv->file) {
    return 0;
  }
  return deadbeef->fgetlength(priv->file);
}

static off_t dbsf_get_offset(void *user_data) {
  db_priv_t *priv = (db_priv_t *)user_data;
  if (!priv) {
    return 0;
  }
  return priv->offset;
}

static const char *dbsf_get_name(void *user_data) {
  db_priv_t *priv = (db_priv_t *)user_data;
  if (!priv) {
    return NULL;
  }
  return priv->name;
}

static int dbsf_read(void *user_data, uint8_t *dst, int64_t offset,
                     int length) {
  db_priv_t *priv = (db_priv_t *)user_data;
  size_t read;
  if (!priv || !priv->file) {
    return 0;
  }
  if (priv->offset != offset) {
    dbsf_seek(priv, offset);
  }
  read = deadbeef->fread(dst, 1, length, priv->file);
  if (read > 0) {
    priv->offset += read;
  }
  assert(read <= INT_MAX);
  return (int)read;
}

static void dbsf_close(struct libstreamfile_t *libsf) {
  if (!libsf) {
    return;
  }

  db_priv_t *priv = (db_priv_t *)libsf->user_data;
  if (priv && priv->file) {
    deadbeef->fclose(priv->file);
    priv->file = NULL;
  }

  free(priv);
  free(libsf);
}

static libstreamfile_t *dbsf_create_from_path(const char *path);
static libstreamfile_t *dbsf_open(void *user_data, const char *const filename) {
  if (!filename) {
    return NULL;
  }
  return dbsf_create_from_path(filename);
}

static libstreamfile_t *dbsf_create(DB_FILE *file, const char *path) {
  libstreamfile_t *libsf = NULL;
  db_priv_t *priv = NULL;

  libsf = (libstreamfile_t *)calloc(1, sizeof(libstreamfile_t));
  if (!libsf) {
    goto err1;
  }

  libsf->read = dbsf_read;
  libsf->get_size = dbsf_get_size;
  libsf->get_name = dbsf_get_name;
  libsf->open = dbsf_open;
  libsf->close = dbsf_close;

  priv = (db_priv_t *)calloc(1, sizeof(db_priv_t));
  if (!priv) {
    goto err1;
  }

  priv->file = file;
  priv->offset = 0;
  priv->name = strdup(path);
  libsf->user_data = priv;

  return libsf;

err1:
  free(libsf);
  free(priv);
  return NULL;
}

static libstreamfile_t *dbsf_create_from_path(const char *path) {
  DB_FILE *file = deadbeef->fopen(path);

  if (!file) {
    /* Allow vgmstream's virtual files. */
    if (!libvgmstream_is_virtual_filename(path)) {
      return NULL;
    }
  }

  return dbsf_create(file, path);
}

static libvgmstream_t *init_vgmstream_from_dbfile(const char *path,
                                                  int subsong) {
  libstreamfile_t *sf = NULL;
  libvgmstream_t *vgmstream = NULL;

  sf = dbsf_create_from_path(path);
  if (!sf) {
    return NULL;
  }

  libvgmstream_config_t vcfg;
  memset(&vcfg, 0, sizeof(libvgmstream_config_t));
  vcfg.allow_play_forever = conf_loop_single;
  vcfg.play_forever = conf_loop_single;
  vcfg.loop_count = conf_loop_count;
  vcfg.fade_time = conf_fade_duration;
  vcfg.fade_delay = conf_fade_delay;
  vcfg.force_loop = conf_force_looping;

  // Believe it or not, this is correct: we don't need to do anything different
  // in the error case versus the success case.
  vgmstream = libvgmstream_create(sf, subsong, &vcfg);
  libstreamfile_close(sf);
  return vgmstream;
}

/*
 * VGMStream decoder plugin for Deadbeef
 */

typedef struct {
  DB_fileinfo_t info;
  libvgmstream_t *s;
  int can_loop;
  int position;
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
  vgm_info_t *info = malloc(sizeof(vgm_info_t));
  memset(info, 0, sizeof(vgm_info_t));
  info->can_loop = (hints & DDB_DECODER_HINT_CAN_LOOP) != 0;
  return &info->info;
}

static int vgm_init(DB_fileinfo_t *_info, DB_playItem_t *it) {
  vgm_info_t *info = (vgm_info_t *)_info;
  deadbeef->pl_lock();
  char *fname = strdup(deadbeef->pl_find_meta(it, ":URI"));
  int subsong = deadbeef->pl_find_meta_int(it, ":TRACKNUM", 0);
  deadbeef->pl_unlock();

  info->s = init_vgmstream_from_dbfile(fname, subsong != 0 ? subsong : 1);
  free(fname);

  if (!info->s) {
    return -1;
  }

  info->totalsamples = info->s->format->play_samples;

  _info->readpos = 0;
  _info->plugin = &plugin.decoder;
  _info->fmt.bps = 16;
  _info->fmt.channels = info->s->format->channels;
  _info->fmt.samplerate = info->s->format->sample_rate;

  deadbeef->pl_lock();
  int i, num_playlists = deadbeef->plt_get_count();
  // Unfortunately, there's no way to get the playlist directly from the
  // DB_playItem_t. Loop through all playlists and find the right one.
  for (i = 0; i < num_playlists; i++) {
    ddb_playlist_t *plt = deadbeef->plt_get_for_idx(i);
    if (!plt || deadbeef->plt_get_item_idx(plt, it, 0) == -1) {
      deadbeef->plt_unref(plt);
      continue;
    }
    deadbeef->plt_set_item_duration(
        plt, it, (float)info->totalsamples / _info->fmt.samplerate);
    deadbeef->plt_unref(plt);
    break;
  }
  deadbeef->pl_unlock();

  for (i = 0; i < _info->fmt.channels; i++) {
    _info->fmt.channelmask |= 1 << i;
  }

  return 0;
}

static void vgm_free(DB_fileinfo_t *_info) {
  vgm_info_t *info = (vgm_info_t *)_info;
  libvgmstream_free(info->s);
  if (info) {
    free(info);
  }
}

static int vgm_read(DB_fileinfo_t *_info, char *bytes, int size) {
  vgm_info_t *info = (vgm_info_t *)_info;
  int sample_size = _info->fmt.channels * sizeof(int16_t);
  int32_t sample_count = size / sample_size;

  if (info->s->decoder->done) {
    /* At the end? Let deadbeef know. */
    return 0;
  }

  /* Do the actual rendering. */
  int result = libvgmstream_fill(info->s, bytes, sample_count);
  if (result < 0) {
    /* An error occurred. Signal EOF. */
    return 0;
  }
  sample_count = info->s->decoder->buf_samples;

  /* Update position. */
  info->position += sample_count;
  _info->readpos = (float)info->position / (float)_info->fmt.samplerate;

  return sample_count * sample_size;
}

static int vgm_seek_sample64(DB_fileinfo_t *_info, int64_t sample) {
  vgm_info_t *info = (vgm_info_t *)_info;
  libvgmstream_seek(info->s, sample);
  info->position = sample;
  _info->readpos = (float)info->position / (float)_info->fmt.samplerate;
  return 0;
}

static int vgm_seek_sample(DB_fileinfo_t *_info, int sample) {
  return vgm_seek_sample64(_info, sample);
}

static int vgm_seek(DB_fileinfo_t *_info, float time) {
  return vgm_seek_sample64(
      _info, (int64_t)((double)time * (double)_info->fmt.samplerate));
}

static DB_playItem_t *vgm_insert_subsong(ddb_playlist_t *plt,
                                         DB_playItem_t *after,
                                         const char *fname, int subsong) {
  libvgmstream_t *vgm = init_vgmstream_from_dbfile(fname, subsong);
  if (!vgm) {
    return after;
  }

  DB_playItem_t *it = deadbeef->pl_item_alloc_init(fname, plugin.decoder.plugin.id);

  libvgmstream_title_t tcfg;
  memset(&tcfg, 0, sizeof(libvgmstream_title_t));
  tcfg.remove_extension = 1;
  tcfg.remove_archive = 1;
  tcfg.filename = fname;

  char title[1024];
  libvgmstream_get_title(vgm, &tcfg, title, sizeof(title));
  deadbeef->pl_add_meta(it, "title", title);
  deadbeef->pl_replace_meta(it, ":FILETYPE", "vgm");
  deadbeef->pl_set_meta_int(it, ":TRACKNUM", subsong);
  if (vgm->format->loop_flag) {
    deadbeef->pl_set_meta_int(it, ":loop_start", vgm->format->loop_start);
    deadbeef->pl_set_meta_int(it, ":loop_end", vgm->format->loop_end);
  }
  if (vgm->format->subsong_count > 1) {
    deadbeef->pl_set_meta_int(it, ":stream_count", vgm->format->subsong_count);
  }
  if (vgm->format->subsong_index > 1) {
    deadbeef->pl_set_meta_int(it, ":stream_index", vgm->format->subsong_index);
  }
  if (vgm->format->stream_name[0] != 0) {
    deadbeef->pl_add_meta(it, ":stream_name", vgm->format->stream_name);
  }
  deadbeef->pl_set_meta_int(it, ":SAMPLERATE", vgm->format->sample_rate);
  deadbeef->pl_set_meta_int(it, ":CHANNELS", vgm->format->channels);
  deadbeef->pl_set_meta_int(it, ":BPS", 16);
  deadbeef->pl_set_meta_int(it, ":BITRATE", vgm->format->stream_bitrate / 1000);

  size_t num_samples = vgm->format->play_samples;
  deadbeef->plt_set_item_duration(
      plt, it, (float)num_samples / vgm->format->sample_rate);

  after = deadbeef->plt_insert_item(plt, after, it);
  deadbeef->pl_item_unref(it);

  libvgmstream_free(vgm);

  return after;
}

static DB_playItem_t *vgm_insert(ddb_playlist_t *plt, DB_playItem_t *after,
                                 const char *fname) {
  libvgmstream_t *vgm = init_vgmstream_from_dbfile(fname, 0);
  if (vgm == NULL) {
    return after;
  }

  int i, num_subsongs =
             vgm->format->subsong_count > 0 ? vgm->format->subsong_count : 1;
  int add_subsong = (num_subsongs > 1 && vgm->format->subsong_index == 0)
                        ? -1
                        : (vgm->format->subsong_index || 1);

  libvgmstream_free(vgm);

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
  conf_loop_single = deadbeef->conf_get_int("playback.loop", DDB_REPEAT_ALL) ==
                     DDB_REPEAT_SINGLE;
  conf_loop_count = (double)deadbeef->conf_get_float("vgm.loopcount",
                                                     (float)DEFAULT_LOOP_COUNT);
  conf_fade_duration = (double)deadbeef->conf_get_float(
      "vgm.fadeduration", (float)DEFAULT_FADE_DURATION);
  conf_fade_delay = (double)deadbeef->conf_get_float("vgm.fadedelay",
                                                     (float)DEFAULT_FADE_DELAY);
  conf_force_looping = deadbeef->conf_get_int("vgm.forcelooping", 0);
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
static ddb_decoder2_t plugin = {
    .decoder =
        {
            DB_PLUGIN_SET_API_VERSION.plugin.version_major = 0,
            .plugin.version_minor = 1,
            .plugin.flags = DDB_PLUGIN_FLAG_IMPLEMENTS_DECODER2,
            .plugin.type = DB_PLUGIN_DECODER,
            .plugin.name = "vgmstream",
            .plugin.id = "vgm",
            .plugin.descr =
                "Decodes a variety of streaming video game music formats.",
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
        },
    .seek_sample64 = vgm_seek_sample64,
};

__attribute__((visibility("default"))) DB_plugin_t *
vgm_load(DB_functions_t *api) {
  deadbeef = api;
  return DB_PLUGIN(&plugin);
}
