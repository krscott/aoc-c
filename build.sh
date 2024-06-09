#!/usr/bin/env bash
set -e

# Navigate to this script's directory
cd "$(dirname "$(realpath "${BASH_SOURCE[0]}")")"

BUILD_DIR=out/Debug

usage() {
	echo "Usage: $0 [COMMAND [...]]"
	echo "  COMMAND  clean | test | run PROG INPUT"
	echo "           Example: $0 day01a inputs/01a-example.txt"
	echo "  PROG     program binary name"
	echo "  INPUT    input text file"
}

cd-build-dir() {
	mkdir -p "$BUILD_DIR"
	cd "$BUILD_DIR"
}

build() {
	(
		cd-build-dir
		cmake ../..
		cmake --build .
	)
}

case "$1" in
	"")
		build
		;;
	clean)
		if [[ -d $BUILD_DIR ]]; then
			rm -rf "$BUILD_DIR"
		fi
		;;
	test)
		build
		(
			cd-build-dir
			ctest -T memcheck
		)
		;;
	run)
		shift
		CMD="$1"
		shift
		if [[ -z $CMD ]]; then
			usage
			exit 1
		fi
		build
		"$BUILD_DIR/$CMD" "$@"
		;;
	*)
		usage
		exit 1
		;;
esac
