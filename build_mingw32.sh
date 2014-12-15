#!/bin/sh

set -e

mkdir -p build.debug
cd build.debug
cmake ..
make -j4 syn_to_c gc_to_bin packer
cd ..

mkdir -p build.mingw32
cd build.mingw32
cmake \
	-DCMAKE_TOOLCHAIN_FILE=engine/cmake/toolchains/i686-w64-mingw32.cmake \
	-DPACKER="$(pwd)/../build.debug/engine/tools/packer/packer" \
	-DGC_TO_BIN="$(pwd)/../build.debug/engine/3rdparty/glsl_compiler/gc_to_bin" \
	-DSYN_TO_C="$(pwd)/../build.debug/engine/3rdparty/glsl_compiler/syn_to_c" \
	..
make -j4
cd ..
