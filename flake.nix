{
  description = "Deadbeef plugin for playing streaming video game music";
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
      ...
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs { inherit system; };
        deadbeef-vgmstream = pkgs.callPackage ./package.nix { };
        deadbeef-with-vgmstream =
          (pkgs.deadbeef-with-plugins.override {
            plugins = [ deadbeef-vgmstream ];
          }).overrideAttrs
            {
              meta.mainProgram = "deadbeef";
            };
      in
      {
        packages = {
          inherit deadbeef-vgmstream deadbeef-with-vgmstream;
          default = deadbeef-with-vgmstream;
        };
        checks = {
          inherit deadbeef-vgmstream;
          basic = pkgs.callPackage ./tests/basic.nix { inherit self; };
        };
        devShells.default = pkgs.mkShell {
          inputsFrom = [ deadbeef-vgmstream ];
        };
      }
    )
    // {
      overlays.default = final: prev: {
        deadbeefPlugins = prev.deadbeefPlugins // {
          vgmstream = final.callPackage ./package.nix { };
        };
      };
      nixosModules.deadbeef-vgmstream = {
        config = {
          nixpkgs.overlays = [ self.overlays.default ];
        };
      };
    };
}
