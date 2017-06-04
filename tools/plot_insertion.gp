#!/usr/bin/gnuplot

set terminal qt persist

set xlabel "Número de entidades"
set ylabel "Tempo por inserção (s)"

set autoscale
set decimalsign ","

set title "Tempo de inserção em função do número de entidades"

set grid

set key top left

f(x) = 1.0/60.0

plot "data/insertion.oo_10000000"  using 1:2 title "Orientação a Objetos"  w lines lw 1, \
     "data/insertion.do0_10000000" using 1:2 title "Orientação a Dados" w lines lw 1, \
     "data/insertion.do1_10000000" using 1:2 title "Orientação a Dados com Particionamento de Arrays" w lines lw 1, \
     f(x) title "60 FPS" w lines lw 1
