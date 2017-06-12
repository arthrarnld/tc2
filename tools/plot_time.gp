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

plot "data/ecs/time_oo_3000_30_15_500"  using 1:2 title "Orientação a Objetos"  w linespoints pt 9 ps .4 lw 1, \
     "data/ecs/time_do0_3000_30_15_500" using 1:2 title "Orientação a Dados" w linespoints pt 6 ps .4 lw 1, \
     "data/ecs/time_do1_3000_30_15_500" using 1:2 title "Orientação a Dados (arrays particionados)" w linespoints pt 7 ps .4 lw 1, \
     f(x) title "60 FPS" w lines lw 1
