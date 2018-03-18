#!/usr/bin/env bash

set -e

cmake .
make
sudo make install
sudo ldconfig /usr/local/lib
