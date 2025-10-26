set terminal pngcairo size 800,600
set output '../../results/lissajous.png'
set xlabel 'x1'
set ylabel 'x2'
set title 'Lissajous: x1 vs x2'
plot '../../results/datos.txt' using 2:4 with lines notitle

