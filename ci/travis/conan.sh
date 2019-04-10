#!/usr/bin/env bash

set -e

sudo pip install -U pip
sudo pip install conan

pip --version
conan --version
conan user
