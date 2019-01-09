#!/usr/bin/env bash

set -e

mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DRTERM_ADD_TEST_TARGET=ON
cmake --build .
cd test
time ./benchmark
