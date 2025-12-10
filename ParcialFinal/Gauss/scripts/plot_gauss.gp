# ===========================================
# Histogram + Gaussian curve (correct)
# ===========================================

set terminal pngcairo size 900,600 enhanced font 'Verdana,12'
set output 'results/hist_gauss.png'

# Parámetros
sigma = 1.0/sqrt(2.0)
mu = 0.0
gauss(x) = (1.0/(sqrt(2*pi)*sigma)) * exp(-(x-mu)**2/(2*sigma**2))

# Bins
binwidth = 0.05
bin(x) = binwidth * floor(x/binwidth)

set boxwidth binwidth
set style fill solid 0.6
set xlabel 'x'
set ylabel 'Densidad'
set title 'Histograma normalizado + Gauss teórica'

# ===========================================
# Histograma NORMALIZADO
# smooth freq da frecuencias absolutas → NO sirven
# Solución: usar (1/(N*binwidth)) para normalizar
# ===========================================

N = 100000.0   # número de muestras

plot 'results/gauss.dat' using (bin($1)):(1.0/(N*binwidth)) \
     smooth freq with boxes lc rgb "blue" title 'Histograma', \
     gauss(x) with lines lw 3 lc rgb "red" title 'Gauss teórica'
