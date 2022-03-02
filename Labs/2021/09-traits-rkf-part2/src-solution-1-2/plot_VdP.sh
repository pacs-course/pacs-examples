#!/bin/bash

gnuplot -p -e 'plot "results_VdP.out" using 1:2 with linespoints, "results_VdP.out" using 1:3 with linespoints'
