#/bin/bash

set -x # Print to output the commands executed.

g++ -c -I../include -I$mkEigenInc -fPIC Utilities.cpp

g++ -shared -Wl,-soname,liblinearalgebra.so Utilities.o -o liblinearalgebra.so

g++ -c -I../include -I$mkEigenInc -I$mkSuitesparseInc test.cpp

g++ -L$mkSuitesparseLib -lumfpack -L$PWD -llinearalgebra -Wl,-rpath=$PWD test.o -o test

set +x # Restore previous set of flags.
