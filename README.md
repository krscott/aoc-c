# aoc-c

[![build status](https://github.com/krscott/aoc-c/actions/workflows/cmake-single-platform.yml/badge.svg)](https://github.com/krscott/aoc-c/actions)

Advent of Code 2023 in C.

Rules
- stdlib only
- all error conditions must be passed back to main (including malloc failures)
- must pass memcheck

## Building and Testing

Manually
```
mkdir -p out/Debug
cd out/Debug
cmake ../..
cmake --build .
ctest
```

Build script
```
./build.sh test
```

Nix flake
```
nix build .
```
