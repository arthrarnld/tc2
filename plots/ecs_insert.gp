#!/usr/bin/gnuplot

set terminal pdf
set output 'ecs_insert.pdf'

set title "Tempo de inserção de entidades"
set xlabel "Entidades inseridas"
set ylabel "Tempo de inserção (s)"

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
"../data/ecs/oo/insert_10000000_100000"  using 1:2 title "Orientado a Objetos"  w lines ls 1, \
"../data/ecs/do0/insert_10000000_100000"  using 1:2 title "Orientado a Dados"  w lines ls 2, \
"../data/ecs/do1/insert_10000000_100000"  using 1:2 title "Orientado a Dados (arrays particionados)"  w lines ls 3
        
