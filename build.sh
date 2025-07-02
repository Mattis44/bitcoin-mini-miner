#!/bin/bash

set -e

VCPKG_ROOT="/c/vcpkg"
TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
ACCESS_TOKEN="$1"

if [ -z "$ACCESS_TOKEN" ]; then
  echo "[🔴] Missing access token."
  echo "Usage: ./build.sh YOUR_ACCESS_TOKEN"
  exit 1
fi

mkdir -p build
cd build

cmake .. -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" -DCMAKE_BUILD_TYPE=Debug


cmake --build . --config Debug

./Debug/miner.exe "$ACCESS_TOKEN"
