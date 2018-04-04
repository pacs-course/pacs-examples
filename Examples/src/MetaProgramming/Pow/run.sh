#!/bin/bash
#
# To run the example if you do not want to bother with LD_LIBRARY_PATH
#
# libpacs.so should be in the right directory (Examples/lib)

export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):../../../lib
./main $*
