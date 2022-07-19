#!/bin/bash


mkdir -p build/
rm -rf build/*

cd build
cmake ".."
make -j8
cd ..

NTHREADS=4;

simmedium="-ns 32 -sm 20000 -nt ${NTHREADS}";
simsmall="-ns 16 -sm 10000 -nt ${NTHREADS}";

./build/swaptions-pthreads $simsmall;

./build/swaptions-openmp $simsmall;   
