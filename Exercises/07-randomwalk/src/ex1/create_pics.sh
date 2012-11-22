#! /bin/bash

for n in {1..20}; do
  n_padded=$(printf "%02d" $n)
  echo "creatinf fig ${n_padded}" 
  cat > tmp.gnuplot << EOF
set terminal png
set output 'randomlap${n_padded}.png'

set xrange [-19:19]
set yrange [0:0.51]

gauss(x,D,t) = 2*exp(-x*x / (4*D*t))/sqrt( 4*pi*D*t )

plot 'randomlap${n}.out' w boxes, gauss(x,0.5,${n}) w l
EOF

gnuplot tmp.gnuplot

done
