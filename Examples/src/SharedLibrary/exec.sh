#!/bin/bash
make clean
make

export LD_LIBRARY_PATH=`pwd`
echo "Exacuting main"
./main
echo "Linking to the new versione of the library"
set -x
rm -f libsmall.so libsmall.so.1
ln -s libsmall.so.1.1 libsmall.so
ln -s libsmall.so.1.1 libsmall.so.1
set +x
echo "Rexecuting main"
./main

