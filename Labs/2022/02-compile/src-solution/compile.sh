#!/bin/bash

set -x

function do_distclean {

    do_clean

    if [[ -a test ]]
    then
        rm -rf ./*.o ./*.so test
    fi
}


function do_clean {
    if [[ -a Utilities.o  || -a test.o || -a liblinearalgebra.so ]]
    then
        rm -rf ./*.o ./*.so 
    fi
}


function do_build_library {
    if [[ ! ( -a Utilities.o ) || ( -a Utilities.o && Utilities.cpp -nt Utilities.o) ]]
    then
        g++ -c -I../include -I$mkEigenInc -fPIC Utilities.cpp
    fi

    if [[ ! ( -a liblinearalgebra.so ) || ( -a liblinearalgebra.so && Utilities.o -nt liblinearalgebra.so) ]]
    then
        g++ -shared -Wl,-soname,liblinearalgebra.so Utilities.o -o liblinearalgebra.so
    fi
}

function do_build_executable {

    if [[ ! ( -a liblinearalgebra.so ) ]]
    then
        do_build_library
    fi
       
    if [[ ! ( -a test.o ) || ( -a test.o && test.cpp -nt test.o) ]]
    then
        g++ -c -I../include -I$mkEigenInc -I$mkSuitesparseInc test.cpp
    fi

    if [[ ! ( -a test ) || ( -a test && test.o -nt test) ]]
    then
        g++ -L$mkSuitesparseLib -lumfpack -L$PWD -llinearalgebra -Wl,-rpath=$PWD test.o -o test
    fi
}


case $1 in
    distclean*)        
        do_distclean
        ;;
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

set +x
