#!/bin/bash
export LD_LIBRARY_PATH+=:../../lib
./main_testGenerators $*
gnuplot -e "plot 'variable.dat'; pause -1"
