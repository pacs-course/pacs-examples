#!/bin/bash

set -x

function do_distclean {
    do_clean

    if [[ -e test ]]
    then
        rm -rf test
    fi
}


function do_clean {
    if [[ -e Utilities.o  || -e test.o || -e liblinearalgebra.so ]]
    then
        rm -rf ./*.o ./*.so 
    fi
}


function do_build_library {
    if [[ ! ( -e Utilities.o ) || ( -e Utilities.o && Utilities.cpp -nt Utilities.o) ]]
    then
        g++ -c -I../include -I${mkEigenInc} -fPIC Utilities.cpp
    fi

    if [[ ! ( -e liblinearalgebra.so ) || ( -e liblinearalgebra.so && Utilities.o -nt liblinearalgebra.so) ]]
    then
        g++ -shared -Wl,-soname,liblinearalgebra.so Utilities.o -o liblinearalgebra.so
    fi
}


function do_build_executable {
    if [[ ! ( -e liblinearalgebra.so ) ]]
    then
        do_build_library
    fi
       
    if [[ ! ( -e test.o ) || ( -e test.o && test.cpp -nt test.o) ]]
    then
        g++ -c -I../include -I${mkEigenInc} -I${mkSuitesparseInc} test.cpp
    fi

    if [[ ! ( -e test ) || ( -e test && test.o -nt test) ]]
    then
        g++ -L${mkSuitesparseLib} -lumfpack -L${PWD} -llinearalgebra -Wl,-rpath=${PWD} test.o -o test
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
