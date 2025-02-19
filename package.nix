{
  lib,
  stdenv,
  pkg-config,
  deadbeef,
  gtk3,
  vgmstream,
  mpg123,
  libvorbis,
  ffmpeg,
}:

stdenv.mkDerivation {
  pname = "deadbeef-vgmstream";
  version = "unstable";

  src = ./.;

  nativeBuildInputs = [ pkg-config ];

  buildInputs = [
    deadbeef
    gtk3
    mpg123
    libvorbis
    ffmpeg.dev
  ];

  preBuild = ''
    cp --no-preserve=mode,ownership -LR ${vgmstream.src} ./vgmstream
  '';

  enableParallelBuilding = true;

  installPhase = ''
    runHook preInstall

    mkdir -p $out/lib/deadbeef/
    cp *.so $out/lib/deadbeef/

    runHook postInstall
  '';

  buildFlags = [
    "DEADBEEF_ROOT=${deadbeef}"
  ];

  meta = with lib; {
    description = "Streaming video game music plugin";
    homepage = "https://github.com/jchv/deadbeef-vgmstream";
    license = licenses.mit;
    maintainers = [ maintainers.jchw ];
    platforms = platforms.linux;
  };
}
