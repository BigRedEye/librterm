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

# dlclose suppression for <unknown module> from https://github.com/google/sanitizers/issues/89#issuecomment-406316683
mkdir dlclose && cd dlclose
cmake ../../../ci/travis/fake_dlclose/
cmake --build .
cd ..

# Run becnhmark
LDPRELOAD="/usr/lib/libasan.so ./dlclose/lib/libfake-dlclose.so" LSAN_OPTIONS=suppressions=./../../asan.supp ./benchmark ./resources 200 200
