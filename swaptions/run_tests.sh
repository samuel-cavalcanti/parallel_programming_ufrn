#!/bin/bash


mkdir -p build/
rm -rf build/*

cd build
cmake ".."
make -j8
cd ..

echo "      running Pthreads tests"
./build/swaptions-pthreads-test $simsmall;
echo "      running OpenMP tests"
./build/swaptions-openmp-test $simsmall;   
