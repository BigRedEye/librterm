#!/usr/bin/env bash

set -e

pip3 install --user --upgrade pip3
pip3 install --user conan

pip3 --version
conan --version
conan user
