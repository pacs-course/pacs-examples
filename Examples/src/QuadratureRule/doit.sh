#!/bin/bash
set -x
#list=`find . -mindepth 1 -type d -print`
list="baseVersion  Montecarlo Error Adaptive" 
for f in $list
do
  dir=$f
  echo $dir

   pushd $dir
   make clean
   make library LIBTYPE=DYNAMIC DEBUG=no
  make clean
  make library LIBTYPE=STATIC DEBUG=no
  make install
  popd
done