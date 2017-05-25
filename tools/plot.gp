set terminal qt persist

set xlabel "Número de entidades"
set ylabel "Tempo por iteração (s)"

set autoscale

set title "Tempo de processamento em função do número de entidades"

set grid

set key top left

set style line 1 lc rgb '#1abc9c' lt 1 lw 1 pt 7 ps 1
set style line 2 lc rgb '#c0392b' lt 1 lw 1 pt 9 ps 1.5

plot oo_filename using 1:2 title "Orientado a Objetos" w linespoints ls 1, \
     do_filename using 1:2 title "Orientado a Dados" with linespoints ls 2
