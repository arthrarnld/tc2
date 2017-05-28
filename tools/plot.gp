#!/usr/bin/gnuplot

set terminal qt persist

set xlabel "Número de entidades"
set ylabel "Tempo por iteração (s)"

set autoscale
set decimalsign ","

set title "Tempo de processamento em função do número de entidades"

set grid

set key top left

f(x) = 1.0/60.0

plot "data/oo_20000_10.dat"  using 1:2 title "OO"  w lines lw 1, \
     "data/do0_20000_10.dat" using 1:2 title "DO0" w lines lw 1, \
     "data/do1_20000_10.dat" using 1:2 title "DO1" w lines lw 1, \
     f(x) title "60Hz" w lines lw 1
