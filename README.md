# deadbeef-vgmstream
deadbeef-vgmstream is a DeaDBeeF decoder plugin which connects vgmstream to DeaDBeeF, adding support for hundreds of streaming video game and middleware audio formats.

## Developing
A Nix flake is included, as well as a corresponding `.envrc`.

- Run a copy of Deadbeef with this plugin using `nix run`.
- When using Nix to build, the NixOS version of VgmStream is used and the submodule is ignored.
- When using `make` to build, the submodule version of VgmStream is used.

To develop using Nix, ensure you have Nix installed and direnv configured, then type `direnv allow` to allow the environment to be applied. From there, you can run `make` like normal, and `nix run` to test inside of Deadbeef.

Nix is not required to build. When not using Nix, you must ensure that mpg123, libvorbis, and FFmpeg are available via pkg-config, as these are requirements of VgmStream.

When building using `make`, you can point to another VgmStream source path by setting the `VGMSTREAM_ROOT`. This will override the submodule.

## Installing
By default, the Makefile is configured to install the plugin to a DeaDBeeF installation at the prefix, `/opt/deadbeef`. If you are using the official DeaDBeeF packages for Debian or Ubuntu, this should be fine. Otherwise, you'll need to adjust `DEADBEEF_ROOT` depending on where the program is installed to. Mind you that plugins are not installed directly to `DEADBEEF_ROOT` but under the `lib/deadbeef` subdirectory.
