#!/bin/bash

set -x

g++ -c -I../include -I${mkEigenInc} -fPIC Utilities.cpp
g++ -shared -Wl,-soname,liblinearalgebra.so Utilities.o -o liblinearalgebra.so
g++ -c -I../include -I${mkEigenInc} -I${mkSuitesparseInc} test.cpp
g++ -L${mkSuitesparseLib} -lumfpack -L${PWD} -llinearalgebra -Wl,-rpath=${PWD} test.o -o test

set +x
