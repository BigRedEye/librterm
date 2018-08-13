#!/usr/bin/env bash

set -e

mkdir build
cd build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
make -j4
sudo make install
sudo ldconfig /usr/local/lib
cd ..
rm -rf build
