set terminal png size 800,640
set output 'cluster24.png'
set xrange [-0.1:1.1]
set yrange [-0.1:1.1]
set pointsize 2.5
plot 'cluster24.dat' w p palette title '',\
'<(head -n 6 cluster24.dat)'w p pt 7 title ''
