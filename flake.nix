{
  description = "Deadbeef plugin for playing streaming video game music";
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs =
    { nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs { inherit system; };
        deadbeef-vgmstream = pkgs.callPackage ./package.nix { };
        deadbeef-with-vgmstream = (pkgs.deadbeef-with-plugins.override {
          plugins = [ deadbeef-vgmstream ];
        }).overrideAttrs {
          meta.mainProgram = "deadbeef";
        };
      in
      {
        packages = {
          inherit deadbeef-vgmstream deadbeef-with-vgmstream;
          default = deadbeef-with-vgmstream;
        };
        devShell = pkgs.mkShell {
          inputsFrom = [ deadbeef-vgmstream ];
        };
      }
    );
}
