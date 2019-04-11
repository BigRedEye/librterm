#!/usr/bin/env bash

set -e

RTERM_SANITIZERS=OFF
if [ $BUILD_TYPE = Sanitizers ]; then RTERM_SANITIZERS=ON; fi

# Build
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DRTERM_BUILD_TESTS=ON -DRTERM_SANITIZERS=ON -DRTERM_EXTERNAL_LIBS=$RTERM_LIBS
cmake --build . --parallel $(nproc)

# Run
cd bin

# Check deps
readelf -d benchmark | grep NEEDED

# Run becnhmark
LSAN_OPTIONS=suppressions=./../../asan.supp ./benchmark ./resources 200 200
