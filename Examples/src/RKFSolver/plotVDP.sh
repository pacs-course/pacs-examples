#!/bin/bash
gnuplot -p -e 'plot "resultVDP.dat" using 1:2 with linespoints, "resultVDP.dat" using 1:3 with linespoints'
