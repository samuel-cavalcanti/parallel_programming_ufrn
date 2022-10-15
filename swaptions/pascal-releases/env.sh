#!/bin/sh

export PATH=$PATH:$(pwd)/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/lib
export LIBRARY_PATH=$LIBRARY_PATH:$(pwd)/lib
export C_INCLUDE_PATH=$C_INCLUDE_PATH:$(pwd)/include
export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:$(pwd)/include
export CPATH=$CPATH:$(pwd)/include
