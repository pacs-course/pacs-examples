#!/bin/bash
gnuplot -p -e 'plot "results_VDP.dat" using 1:2 with linespoints, "results_VDP.dat" using 1:3 with linespoints'
