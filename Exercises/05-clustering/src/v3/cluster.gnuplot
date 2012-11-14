#set terminal svg size 800,640 fname 'Verdana, Helvetica, Arial, sans-serif' \
#fsize '9' rounded dashed
set terminal png size 800,640
set output 'cluster.png'

set xrange [-0.1:1.1]
set yrange [-0.1:1.1]
#set palette model HSV
set pointsize 2.5

#plot 'cluster.dat' with points pt 7 palette title ''

plot '<(head -n 4 cluster.dat)' w p palette title ''

#pause -1
