#!/bin/bash
echo "Silently recompiling the libraries"
echo "main is linked against libsmall.so"
make clean >/dev/null
make >/dev/null
rm -f libsmall.so libsmall.so.1
echo "Creating links for version 1.0 of the library"
set -x
ln -s libsmall.so.1.0 libsmall.so.1
ln -s libsmall.so.1 libsmall.so
set +x
export LD_LIBRARY_PATH=`pwd`
echo "Executing main"
./main
echo "Linking to the new version of the library"
set -x
rm -f libsmall.so.1
ln -s libsmall.so.1.1 libsmall.so.1
set +x
echo "Re-executing main with the new library"
./main

