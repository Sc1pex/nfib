#!/bin/sh

cd build
meson compile
cd ..
./build/nfib $@
