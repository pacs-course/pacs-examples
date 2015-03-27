#!/bin/bash
make distclean
./configure --enable-shared=yes --prefix=`pwd`/../../Examples/
make
make install
make distclean
./configure --enable-shared=no --prefix=`pwd`/../../Examples/
make
make install
