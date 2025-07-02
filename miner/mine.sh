#!/bin/bash

set -e

VCPKG_ROOT="/c/vcpkg"
TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"

mkdir -p build
cd build

cmake .. -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" -DCMAKE_BUILD_TYPE=Debug


cmake --build . --config Debug

./Debug/miner.exe
