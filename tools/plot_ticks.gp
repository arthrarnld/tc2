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

plot "data/rticks.oo_20000_10"  using 1:2 title "Orientação a Objetos"  w lines lw 2, \
     "data/rticks.do0_20000_10" using 1:2 title "Orientação a Dados" w lines lw 2, \
     "data/rticks.do1_20000_10" using 1:2 title "Orientação a Dados (arrays particionados)" w lines lw 2, \
     f(x) title "60 FPS" w lines lw 1
