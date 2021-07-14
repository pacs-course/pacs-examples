#!/bin/bash
gnuplot -p -e 'plot "resultstiff.dat" using 1:2 with linespoints, "resultstiff.dat" using 1:3 with linespoints'
