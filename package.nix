{
  lib,
  stdenv,
  pkg-config,
  deadbeef,
  vgmstream,
  mpg123,
  libvorbis,
  ffmpeg,
}:

stdenv.mkDerivation {
  pname = "deadbeef-vgmstream-plugin";
  version = "0-unstable";

  src = ./.;

  nativeBuildInputs = [ pkg-config ];

  buildInputs = [
    deadbeef
    mpg123
    libvorbis
    ffmpeg.dev
  ];

  enableParallelBuilding = true;

  makeFlags = [ "DEADBEEF_ROOT=${deadbeef}" ];
  installFlags = [ "DEADBEEF_ROOT=$(out)" ];

  postUnpack = ''
    cp --no-preserve=mode,ownership -LR ${vgmstream.src} $sourceRoot/vgmstream
  '';

  meta = {
    description = "Streaming video game music plugin";
    homepage = "https://github.com/jchv/deadbeef-vgmstream";
    license = lib.licenses.isc;
    maintainers = [ lib.maintainers.jchw ];
    platforms = lib.platforms.linux;
  };
}
