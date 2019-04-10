#!/usr/bin/env bash

set -e

pip install --user --upgrade pip
pip install --user conan

pip --version
conan --version
conan user
