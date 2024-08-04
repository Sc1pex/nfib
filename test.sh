#!/bin/sh

cd build
meson compile
cd ..
./build/fib_lib/libtest
./build/cli/clitest
