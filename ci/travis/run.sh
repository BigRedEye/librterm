#!/usr/bin/env bash

set -e

# Build
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DRTERM_BUILD_TESTS=ON -DRTERM_EXTERNAL_LIBS=$RTERM_LIBS
cmake --build . --parallel $(nproc)

# Run
cd bin

# Check deps
readelf -d benchmark | grep NEEDED

# Run becnhmark
LSAN_OPTIONS=suppressions=./../../asan.supp ./benchmark ./resources 200 200
