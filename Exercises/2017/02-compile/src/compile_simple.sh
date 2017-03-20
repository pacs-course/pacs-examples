#!/bin/sh

set -x

g++ -std=c++11 -c -I../include -I$mkEigenInc -fPIC Utilities.cpp
g++ -shared -Wl,-soname,liblinearalgebra.so Utilities.o -o liblinearalgebra.so
g++ -std=c++11 -c -I../include -I$mkEigenInc -I$mkSuitesparseInc test_all.cpp
g++ -L$mkSuitesparseLib -lumfpack -L$PWD -llinearalgebra test_all.o -o test_all

set +x
