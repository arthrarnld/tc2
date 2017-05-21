set terminal qt persist

set xlabel "Número de entidades"
set ylabel "Tempo de processamento"

set autoscale

set title "Tempo de processamento em função do número de entidades"

set grid

plot oo_filename using 1:2 title "Orientado a Objetos" with linespoints, \
     do_filename using 1:2 title "Orientado a Dados" with linespoints
