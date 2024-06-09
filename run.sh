#!/usr/bin/env bash
set -e

# Navigate to this script's directory
cd "$(dirname "$(realpath "${BASH_SOURCE[0]}")")"

usage() {
    echo "Usage: $0 COMMAND INPUT"
    echo "  COMMAND  clean | test | <program_name>"
    echo "           Example: $0 day01a inputs/01a-example.txt"
    echo "  INPUT    input text file"
}

build() {
    mkdir -p $BUILD_DIR
    cd $BUILD_DIR

    cmake ..
    cmake --build .
}

CMD="$1"
INPUT="$2"

if [[ -z "$CMD" ]]; then
    usage
    exit 1
fi

BUILD_DIR=build

if [[ $CMD == clean ]]; then
    if [[ -d "$BUILD_DIR" ]]; then
        rm -rf "$BUILD_DIR"
    fi
    exit
fi

if [[ $CMD == test ]]; then
    build
    ctest -T memcheck
    exit
fi

if [[ -z "$INPUT" ]]; then
    usage
    exit 1
fi

build
"./$CMD" "$INPUT"

