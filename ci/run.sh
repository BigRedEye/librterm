#!/usr/bin/env bash

set -e

cmake .
make
sudo install librterm.so /usr/local/lib
cd test
cmake . -G "Unix Makefiles"
make
time ./benchmark
