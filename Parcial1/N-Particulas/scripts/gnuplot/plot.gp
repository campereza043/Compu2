# Configuración de la animación
set terminal gif animate delay 5
set output '../animacion.gif'

# --- Parámetros ---
W = 100.0
H = 100.0
FRAME_DT = 0.01 # Corresponde al DT_FRAME en C++

# Rango de los ejes
set xrange [0:W]
set yrange [0:H]
set size ratio -1 # Aspecto 1:1

# Leer el número de partículas del encabezado
header = system("head -n 1 ../../results/datos.txt")
N = (words(header) - 1) / 4

stats '../../results/datos.txt' nooutput

# Bucle de animación para cada fotograma (bloque) en el archivo
do for [i=0:int(STATS_blocks)-2] {
    # El título ahora usa la variable FRAME_DT para el tiempo
    set title sprintf("Simulacion de Particulas | Tiempo: %.2f", i*FRAME_DT)
    
    # Genera el comando de plot para N partículas
    plot_cmd = "plot for [j=1:".N."] '../../results/datos.txt' index i u (column(4*(j-1)+2)):(column(4*(j-1)+3)) with circles lc j title ''"
    
    eval(plot_cmd)
}

set output
print "Animacion guardada en 'scripts/animacion.gif'"
