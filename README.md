# deadbeef-vgmstream
deadbeef-vgmstream is a DeaDBeeF decoder plugin which connects vgmstream to DeaDBeeF, adding support for hundreds of streaming video game and middleware audio formats.

## Installing
By default, the Makefile is configured to install the plugin to a DeaDBeeF installation at the prefix, `/opt/deadbeef`. If you are using the official DeaDBeeF packages for Debian or Ubuntu, this should be fine. Otherwise, you'll need to adjust `DEADBEEF_ROOT` depending on where the program is installed to. Mind you that plugins are not installed directly to `DEADBEEF_ROOT` but under the `lib/deadbeef` subdirectory.

## Known issues
This plugin is far from perfect - it was rather rushed out. The current issues are:

 *  Seeking is broken.
     - Seeking is slow, especially seeking backwards. This is because vgmstream does not have true support for seeking. It may be possible to add support for seeking, but work has to be done to each decoder and some other code in vgmstream.
     - Sometimes, seeking near the end of a file mysteriously causes an end-of-file early. Seeking generally works well, so it's not really clear what's happening here.
 *  There is no support for meta-data.
     - Because we are dealing with files that are often internal to a video game, there will not always be interesting metadata.
     - If we want good support for meta-data, we should probably add a means by which it can be stored and retrieved out-of-band.
 *  Importantly, it needs *more testing.*
     - I have only tested with 44.1 KHz, 16-bit, stereo CRI ADX files. While these have worked incredibly well for me, there are *a lot* of ways different formats could cause problems.
     - The implementation of STREAMFILE to DeaDBeeF's VFS API is also much untested. It works well for local files, of course, but I have yet to test anything else.
     - Performance is mostly up to vgmstream, but the performance of e.g. adding files to the playlist probably should be assessed.