#!/bin/bash
gnuplot -p -e 'set xlabel "Day"; set ylabel "No. of people";
               plot "results_SIR.dat" using 1:2 with lines linewidth 2 title "Susceptible",
                    "results_SIR.dat" using 1:3 with lines linewidth 2 title "Infected",
                    "results_SIR.dat" using 1:4 with lines linewidth 2 title "Recovered"'
