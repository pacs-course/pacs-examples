set terminal png
set output 'randomlap5.png'

set xrange [-19:19]
set yrange [0:0.51]

gauss(x,D,t) = 2*exp(-x*x / (4*D*t))/sqrt( 4*pi*D*t )

plot 'randomlap5.out' w boxes, gauss(x,0.5,6) w l


