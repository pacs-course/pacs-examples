#!/bin/bash
./configure --prefix=`pwd`/../../Examples/
make clean
make
make install
make clean
