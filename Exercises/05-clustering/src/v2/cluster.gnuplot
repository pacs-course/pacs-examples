set terminal svg size 410,250 fname 'Verdana, Helvetica, Arial, sans-serif' \
fsize '9' rounded dashed
set output 'cluster.svg'

set xrange [-0.1:1.1]
set yrange [-0.1:1.1]
#set pointsize 2.5

plot 'cluster.dat' with points palette

#pause -1