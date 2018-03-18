#!/usr/bin/env bash

set -e

cd test
cmake . -G "Unix Makefiles"
make
time ./benchmark
