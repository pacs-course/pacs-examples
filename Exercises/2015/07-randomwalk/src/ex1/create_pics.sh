#! /bin/bash

# loop on files
for n in {1..20}; do

  # keep n stored before reassigning it
  n_stripped=${n}

  # add zeroes in front of n
  n=$(printf "%02d" $n)

  echo "creating fig ${n}" 

  # write down a temporary file with gnuplot commands
  cat > tmp.gnuplot << EOF
set terminal png
set output 'randomlap${n}.png'

set xrange [-19:19]
set yrange [0:0.51]

gauss(x,D,t) = 2*exp(-x*x / (4*D*t))/sqrt( 4*pi*D*t )

plot 'randomlap${n}.out' w boxes lw 2, gauss(x,0.5,${n_stripped}) w l lw 3
EOF

  # invoke gnuplot to print out the figure
  gnuplot tmp.gnuplot

  # clean up the temporary file
  rm tmp.gnuplot

done
