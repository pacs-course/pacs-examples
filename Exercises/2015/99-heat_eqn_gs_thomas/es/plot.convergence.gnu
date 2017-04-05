set term postscript enh monochrome landscape 20

set xlabel "h"

set grid
set logscale x
set logscale y
set key right bottom width 7 spacing 2 Left

set output "plots/fin.convergence.eps"
p 'fin.convergence.xy' u 1:2 w lp t "linf"
