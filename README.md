# aoc-c

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
