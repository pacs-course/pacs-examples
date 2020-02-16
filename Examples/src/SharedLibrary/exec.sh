#!/bin/bash
echo "Silently recompiling the libraries"
make clean >/dev/null
make >/dev/null

export LD_LIBRARY_PATH=`pwd`
echo "Executing main"
./main
echo "Linking to the new version of the library"
set -x
rm -f libsmall.so libsmall.so.1
ln -s libsmall.so.1.1 libsmall.so
ln -s libsmall.so.1.1 libsmall.so.1
set +x
echo "Re-executing main with the new library"
./main

