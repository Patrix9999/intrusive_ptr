#!/usr/bin/env sh

mkdir -p build

rm -rf build/doxygen
doxygen ../Doxyfile

rm -rf build/html
sphinx-build -b html . build/html