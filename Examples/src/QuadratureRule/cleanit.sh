#!/bin/bash
set -x
#list=`find . -mindepth 1 -type d -print`
list="baseVersion  Montecarlo Error Adaptive" 
for f in $list
do
  dir=$f
  echo $dir

   pushd $dir
   make distclean
  popd
done