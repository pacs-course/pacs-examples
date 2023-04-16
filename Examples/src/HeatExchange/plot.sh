#!/bin/bash
# A script that launches gnuplot on the file result.dat
#
# you can also launch gnuplot and type the command at the gnuplot prompt
#

gnuplot --persist <<EOF
plot "result.dat" u 1:2 w lp lw 2 title "uh", "result.dat" u 1:3 w l lw 2title "uex"
pause -1
EOF
