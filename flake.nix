{
  description = "Advent of Code 2023 in C";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      supportedSystems = [ "x86_64-linux" "x86_64-darwin" "aarch64-linux" "aarch64-darwin" ];
      forAllSystems = nixpkgs.lib.genAttrs supportedSystems;
    in {
      packages = forAllSystems (system:
        let
          pkgs = import nixpkgs { inherit system; };
        in {
          default = pkgs.clangStdenv.mkDerivation {
            name = "advent-of-code-2023";
            src = ./.;

            buildInputs = with pkgs; [
              cmake
            ];

            configurePhase = ''
              cmake .
            '';

            buildPhase = ''
              make
            '';

            installPhase = ''
              mkdir -p $out/bin
              cp day* $out/bin
            '';
          };
        }
      );

      # apps = forAllSystems (system: {
      #   default = {
      #     type = "app";
      #     program = "${self.packages.${system}.default}/bin/aoc";
      #   };
      # });

      devShells = forAllSystems (system:
        let
          pkgs = import nixpkgs { inherit system; };
          llvm = pkgs.llvmPackages_latest;
        in {
          default = pkgs.mkShell.override { stdenv = pkgs.clangStdenv; } {
            packages = with pkgs; [
              # builder
              gnumake
              cmake
              # bear

              # debugger
              llvm.lldb
              gdb

              # fix headers not found
              clang-tools

              # LSP and compiler
              # llvm.libstdcxxClang

              # other tools
              # cppcheck
              llvm.libllvm
              valgrind

              # stdlib for cpp
              # llvm.libcxx
            ];
          };
        });
    };
}
