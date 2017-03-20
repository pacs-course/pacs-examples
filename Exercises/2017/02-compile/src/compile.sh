#!/bin/sh


set -x

function do_distclean {

    do_clean

    if [[ -a test_all ]]
    then
        rm -rf ./*.o ./*.so test_all
    fi
}


function do_clean {
    if [[ -a Utilities.o  || -a test_all.o || -a liblinearalgebra.so ]]
    then
        rm -rf ./*.o ./*.so 
    fi
}


function do_build_library {
    if [[ ! ( -a Utilities.o ) || ( -a Utilities.o && Utilities.cpp -nt Utilities.o) ]]
    then
        g++ -std=c++11 -c -I../include -I$mkEigenInc -fPIC Utilities.cpp
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
       
    if [[ ! ( -a test_all.o ) || ( -a test_all.o && test_all.cpp -nt test_all.o) ]]
    then
        g++ -std=c++11 -c -I../include -I$mkEigenInc -I$mkSuitesparseInc test_all.cpp
    fi

    if [[ ! ( -a test_all ) || ( -a test_all && test_all.o -nt test_all) ]]
    then
        g++ -L$mkSuitesparseLib -lumfpack -L$PWD -llinearalgebra test_all.o -o test_all
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
