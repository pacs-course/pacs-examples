set term postscript enh monochrome landscape 20

set key
set xlabel 'x'
set ylabel 'T'
set output "fin.profile.eps"
p 'fin.xy' u 1:2 w lp t "uh", 'fin.xy' u 1:3 w lp t "u"
