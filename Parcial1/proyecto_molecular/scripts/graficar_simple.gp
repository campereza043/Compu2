#!/usr/bin/gnuplot
#
# Script simple de Gnuplot para visualizar resultados
#

set terminal pngcairo size 1000,800 enhanced font "Arial,12"
set datafile commentschars "#"

# Trayectorias simples
set output 'results/trayectorias_simple.png'
set xlabel 'x'
set ylabel 'y'
set title 'Trayectorias de partículas'
set grid

# Graficar solo las primeras 3 partículas para claridad
plot 'results/datos_simulacion.txt' using 2:3 with lines title 'Partícula 1', \
     '' using 6:7 with lines title 'Partícula 2', \
     '' using 10:11 with lines title 'Partícula 3'

print "Gráfico de trayectorias guardado en results/trayectorias_simple.png"

# Energía total aproximada
set output 'results/energia_simple.png'
set xlabel 'Tiempo'
set ylabel 'Energía cinética aprox.'

# Suma de cuadrados de velocidades (asumiendo masa=1)
plot 'results/datos_simulacion.txt' using 1:($4**2+$5**2+$8**2+$9**2+$12**2+$13**2) with lines title 'Energía total aprox.'

print "Gráfico de energía guardado en results/energia_simple.png"
