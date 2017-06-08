#!/usr/bin/gnuplot

set terminal pdfcairo
set output "plot.pdf"

set xlabel "Número de entidades"
set ylabel "Tempo por iteração (s)"

set autoscale
set decimalsign ","

set title "Tempo de processamento em função do número de entidades"

set grid

set key top left

f(x) = 1.0/60.0

plot "data/oo"  using 1:2 title "Orientação a Objetos"  w lines lw 1, \
     "data/do0" using 1:2 title "Orientação a Dados" w lines lw 1, \
     "data/do1" using 1:2 title "Orientação a Dados (arrays particionados)" w lines lw 1, \
     f(x) title "60 FPS" w lines lw 1
