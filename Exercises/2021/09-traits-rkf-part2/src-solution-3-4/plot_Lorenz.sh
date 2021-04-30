#!/bin/bash

gnuplot -p -e 'plot "results_Lorenz.out" using 1:2 with linespoints,
                    "results_Lorenz.out" using 1:3 with linespoints,
                    "results_Lorenz.out" using 1:4 with linespoints'

gnuplot -p -e 'splot "results_Lorenz.out" using 2:3:4 with linespoints;
               pause mouse close'

