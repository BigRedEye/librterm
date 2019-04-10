#!/usr/bin/env bash

set -e

sudo apt-get install python3-pip

pip3 install --user --upgrade pip
pip3 install --user conan

conan --version
conan user
