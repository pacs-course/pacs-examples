#/bin/bash

set -x # Print to output the commands executed.

function do_build_library {
  if [[ !(-a Utilities.o) || (-a Utilities.o && Utilities.cpp -nt Utilities.o) ]]
  then
      g++ -c -I../include -I$mkEigenInc -fPIC Utilities.cpp
  fi

  if [[ !(-a liblinearalgebra.so) || (-a liblinearalgebra.so && Utilities.o -nt liblinearalgebra.so) ]]
  then
      g++ -shared -Wl,-soname,liblinearalgebra.so Utilities.o -o liblinearalgebra.so
  fi
}

function do_build_executable {
  # We need to execute this command only if test.cpp has been modified
  # more recently than when test.o was generated or if test.o does not exist.
  if [[ !(-a test.o) || (-a test.o && test.cpp -nt test.o) ]]
  then
      g++ -c -I../include -I$mkEigenInc -I$mkSuitesparseInc test.cpp # This generates test.o
  fi

  if [[ !(-a test) || (-a test && test.o -nt test) ]]
  then
      g++ -L$mkSuitesparseLib -lumfpack -L$PWD -llinearalgebra -Wl,-rpath=$PWD test.o -o test
  fi
}

function do_clean {
  rm *.o *.so test
}

case $1 in
  clean*)
    do_clean
    ;;
  library*)
    do_build_library
    ;;
  executable*)
    do_build_executable
    ;;
esac

set +x # Restore previous set of flags.
