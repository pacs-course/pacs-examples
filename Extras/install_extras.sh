#!/bin/bash
extra_dirs="muparser muparserx json pybind11"
this_dir=$PWD
if ! test install
then 
   echo "ERROR: You need the install utility installed in your system"
   echo "In ubuntu you can do 'sudo apt-get install coreutils' to get it"
   echo "COMMAND ABORTED"
   exit 1
fi
for d in $extra_dirs
do
    cd $d
    /bin/bash ./install_PACS.sh
    cd $this_dir
done
    
