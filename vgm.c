// vgmstream plugin for DeaDBeeF
// Broken in two parts: need a SF implementation for VGMStream, and a decoder implementation for Deadbeef.

#include <stdlib.h>
#include <string.h>
#include <deadbeef/deadbeef.h>
#include "vgmstream/src/vgmstream.h"
#include "vgmstream/src/streamfile.h"
#include "extensions.h"

#define trace(...) { fprintf(stderr, __VA_ARGS__); }

static DB_decoder_t plugin;
static DB_functions_t *deadbeef;

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

static off_t dbsf_get_offset(DBSTREAMFILE *this) {
	return this->offset;
}

static void dbsf_get_name(DBSTREAMFILE *this, char *buffer, size_t length) {
	strncpy(buffer, this->name, length);
	buffer[length-1]='\0';
}

static size_t dbsf_read(DBSTREAMFILE *this, uint8_t *dest, off_t offset, size_t length) {
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
static STREAMFILE *dbsf_open(DBSTREAMFILE *this, const char *const filename,size_t buffersize) {
	if (!filename) return NULL;
	return dbsf_create_from_path(filename);
}

static STREAMFILE *dbsf_create(DB_FILE *file, const char *path) {
	DBSTREAMFILE *streamfile = malloc(sizeof(DBSTREAMFILE));

	if (!streamfile) return NULL;

	memset(streamfile,0,sizeof(DBSTREAMFILE));
	streamfile->sf.read = (void*)dbsf_read;
	streamfile->sf.get_size = (void*)dbsf_get_size;
	streamfile->sf.get_offset = (void*)dbsf_get_offset;
	streamfile->sf.get_name = (void*)dbsf_get_name;
	streamfile->sf.open = (void*)dbsf_open;
	streamfile->sf.close = (void*)dbsf_close;
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

VGMSTREAM *init_vgmstream_from_dbfile(const char *path) {
	STREAMFILE *sf;
	VGMSTREAM *vgm;

	sf = dbsf_create_from_path(path);
	if (!sf) return NULL;

	vgm = init_vgmstream_from_STREAMFILE(sf);
	if (!vgm) goto err1;

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
	int position;
	int fadesamples;
	int totalsamples;
} vgm_info_t;

static double looptimes = 2.0;
static double fadeseconds = 10.0;
static double fadedelayseconds = 10.0;

#define COPYRIGHT_STR \
	"- deadbeef-vgmstream -\n" \
	"Copyright (c) 2014 John Chadwick <johnwchadwick@gmail.com>\n" \
	"\n" \
	"Licensed under the same terms as vgmstream (see below.)\n" \
	"\n" \
	"- vgmstream -\n" \
	"Copyright (c) 2008-2010 Adam Gashlin, Fastelbja, Ronny Elfert\n" \
	"Portions Copyright (c) 2004-2008, Marko Kreen\n" \
	"Portions Copyright 2001-2007  jagarl / Kazunori Ueno <jagarl@creator.club.ne.jp>\n" \
	"Portions Copyright (c) 1998, Justin Frankel/Nullsoft Inc.\n" \
	"Portions Copyright (C) 2006 Nullsoft, Inc.\n" \
	"Portions Copyright (c) 2005-2007 Paul Hsieh\n" \
	"Portions Public Domain originating with Sun Microsystems\n" \
	"\n" \
	"Permission to use, copy, modify, and distribute this software for any\n" \
	"purpose with or without fee is hereby granted, provided that the above\n" \
	"copyright notice and this permission notice appear in all copies.\n" \
	"\n" \
	"THE SOFTWARE IS PROVIDED \"AS IS\" AND THE AUTHOR DISCLAIMS ALL WARRANTIES\n" \
	"WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF\n" \
	"MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR\n" \
	"ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES\n" \
	"WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN\n" \
	"ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF\n" \
	"OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.\n"

static DB_fileinfo_t *vgm_open(uint32_t hints) {
	DB_fileinfo_t *_info = malloc(sizeof(vgm_info_t));
	vgm_info_t *info = (vgm_info_t *)_info;
	memset(info, 0, sizeof (vgm_info_t));
	return _info;
}

static int vgm_init(DB_fileinfo_t *_info, DB_playItem_t *it) {
	vgm_info_t *info = (vgm_info_t *)_info;
        const char *fname = deadbeef->pl_find_meta (it, ":URI");
	int i;

	info->s = init_vgmstream_from_dbfile(fname);
	info->fadesamples = fadeseconds * info->s->sample_rate;
	info->totalsamples = get_vgmstream_play_samples(looptimes, fadeseconds, fadedelayseconds, info->s);
	_info->fmt.bps = 16;
	_info->fmt.channels = info->s->channels;
	_info->fmt.samplerate = info->s->sample_rate;
	for (i = 0; i < _info->fmt.channels; i++) {
		_info->fmt.channelmask |= 1 << i;
	}
	_info->readpos = 0;
	_info->plugin = &plugin;
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
	int32_t sample_count = size / (info->s->channels * sizeof(int16_t)),
	        fade_start = info->totalsamples - info->fadesamples,
                fade_end = info->totalsamples,
                fade_samples = info->fadesamples;

	/* Past the end? Let deadbeef know. */
	if (info->position >= info->totalsamples) {
		return 0;
	}

	/* Do the actual rendering. */
	render_vgmstream((int16_t *)bytes, sample_count, info->s);

	/* If we are overlapping any piece after the fade starts, we must fade out here.
         * TODO: Desperately needs cleanup. It's _AWFUL_.
         */
	if (info->s->loop_flag && (info->position > fade_start || info->position + sample_count > fade_start)) {
		int16_t *buf = (int16_t *)bytes;
		for (i = 0; i < sample_count; ++i) {
			int pos = i + info->position;
			if (pos > fade_start) {
				int samples_into_fade = pos - fade_start;
				double fadedness = (double)(fade_samples-samples_into_fade)/fade_samples;
				for (j = 0; j < info->s->channels; j++) {
					buf[i * info->s->channels + j] = buf[i * info->s->channels + j] * fadedness;
				}
			} else if (pos > fade_end) {
				info->position = info->totalsamples;
				return i;
			}
		}
	}
	info->position += sample_count;
	return size;
}

static int vgm_seek_sample(DB_fileinfo_t *_info, int sample) {
	int i;
	vgm_info_t *info = (vgm_info_t *)_info;
	int16_t samples[0x2000];

	/* Reset stream to go backwards */
	if (info->s->current_sample > sample) {
		reset_vgmstream(info->s);
		info->position = 0;
	} else {
		sample -= info->s->current_sample;
	}

	/* Render out 0x1000 chunks and throw them away */
	while (sample > 0x1000) {
		for (i = 0; i < info->s->channels; ++i)
			vgm_read(_info, (char *)samples, 0x2000);
		sample -= 0x1000;
	}

	/* Read the final chunk of <0x2000 data (or not) */
	if (sample != 0)
		for (i = 0; i < info->s->channels; ++i)
			vgm_read(_info, (char *)samples, sample * 2);

	/* Set seek pointer */
	_info->readpos = (float)(info->position) / _info->fmt.samplerate;

	return 0;
}

static int vgm_seek(DB_fileinfo_t *_info, float time) {
	return vgm_seek_sample(_info, time * _info->fmt.samplerate);
}

static DB_playItem_t *vgm_insert(ddb_playlist_t *plt, DB_playItem_t *after, const char *fname) {
	VGMSTREAM *vgm = init_vgmstream_from_dbfile(fname);
	if (vgm == NULL) {
		return after;
	}
	size_t num_samples = get_vgmstream_play_samples(looptimes, fadeseconds, fadedelayseconds, vgm);
	DB_playItem_t *it = deadbeef->pl_item_alloc_init(fname, plugin.plugin.id);

	deadbeef->pl_replace_meta(it, ":FILETYPE", "vgm");
	deadbeef->plt_set_item_duration(plt, it, (float)num_samples/vgm->sample_rate);

	after = deadbeef->plt_insert_item(plt, after, it);
	deadbeef->pl_item_unref (it);

	close_vgmstream(vgm);

	return after;
}

static int vgm_start(void) {
	return 0;
}

static int vgm_stop(void) {
	return 0;
}


// define plugin interface
static DB_decoder_t plugin = {
	DB_PLUGIN_SET_API_VERSION
	.plugin.version_major = 0,
	.plugin.version_minor = 1,
	.plugin.type = DB_PLUGIN_DECODER,
	.plugin.name = "vgmstream",
	.plugin.id = "vgm",
	.plugin.descr = "Decodes a variety of streaming video game music formats.",
	.plugin.copyright = COPYRIGHT_STR,
	.plugin.start = vgm_start,
	.plugin.stop = vgm_stop,
	.open = vgm_open,
	.init = vgm_init,
	.free = vgm_free,
	.read = vgm_read,
	.seek = vgm_seek,
	.seek_sample = vgm_seek_sample,
	.insert = vgm_insert,
	.exts = extension_list,
};

__attribute__ ((visibility ("default")))
DB_plugin_t *vgm_load(DB_functions_t *api) {
	deadbeef = api;
	return DB_PLUGIN (&plugin);
}

