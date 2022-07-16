#!/bin/bash


mkdir -p build/
rm -r build/*

cd build
cmake ".."
make -j8
cd ..

./build/swaptions-pthreads -ns 16 -sm 10000 -nt 4;

./build/swaptions-openmp -ns 16 -sm 10000 -nt 4;   
