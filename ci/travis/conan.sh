#!/usr/bin/env bash

set -e

python3 -m pip install --user --upgrade pip
python3 -m pip install --user conan

conan --version
conan user
