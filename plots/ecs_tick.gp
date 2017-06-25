#!/usr/bin/gnuplot

set terminal pdf
set output 'ecs_tick.pdf'

set title "Tempo de atualização por número de entidades"
set xlabel "Entidades"
set ylabel "Tempo de atualização (ms)"

set linestyle 1 dt 1 lw 2 lc rgb '#111111'
set linestyle 2 dt 1 lw 2 lc rgb '#555555'
set linestyle 3 dt 1 lw 2 lc rgb '#999999'
set linestyle 4 dt 2 lw 2 lc rgb '#999999'

set autoscale
set decimalsign ","

set grid
set key top left

f(x) = 1.0/60.0

plot \
"../data/ecs/oo/time_10000_100_10_1000"  using 1:($2*1000) title "Orientado a Objetos"  w lines ls 1, \
"../data/ecs/do0/time_10000_100_10_1000"  using 1:($2*1000) title "Orientado a Dados"  w lines ls 2, \
"../data/ecs/do1/time_10000_100_10_1000"  using 1:($2*1000) title "Orientado a Dados (arrays particionados)"  w lines ls 3, \
1000*f(x) title "60 FPS" w lines ls 4
        
