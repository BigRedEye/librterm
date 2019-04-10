#!/usr/bin/env bash

set -e

pip install -U pip
pip install conan

pip --version
conan --version
conan user
