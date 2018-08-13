#!/usr/bin/env bash

set -e

mkdir build
cd build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ../test
make
time ./benchmark
cd ..
rm -rf build
