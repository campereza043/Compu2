# graficar.gnuplot - Script con partículas del mismo color y forma
reset session

# Configuración del terminal para GIF animado
set terminal gif animate delay 5 loop 0 size 800,600
set output '../results/animacion_billar.gif'

# Inicializar variables (serán sobrescritas con los valores del archivo)
W = 1
H = 1
R_BOLA = 0.2
N = 5

# Función para leer parámetros del archivo
leer_parametro(param) = system(sprintf("grep '^# %s:' ../results/trayectorias.dat | awk '{print $3}'", param))

# Leer parámetros del archivo
W = real(leer_parametro("W"))
H = real(leer_parametro("H")) 
R_BOLA = real(leer_parametro("R_BOLA"))

print "Parámetros leídos del archivo:"
print "  Ancho de caja (W): ", W
print "  Alto de caja (H): ", H  
print "  Radio de partículas: ", R_BOLA

# Configuración de los rangos y aspecto
set xrange [0:W]
set yrange [0:H]
set size ratio -1
set grid
set xlabel 'x'
set ylabel 'y'
set key off

# Dibujar el borde de la caja
set object 1 rect from 0,0 to W,H front fillstyle empty border lc rgb 'black' lw 2

# Configuración para leer el archivo
set datafile commentschars "#"
set datafile separator whitespace

# Contar número de frames
stats '../results/trayectorias.dat' using 1 nooutput
total_frames = STATS_records

# Determinar número de partículas contando columnas
stats '../results/trayectorias.dat' using 2 nooutput
num_columns = STATS_columns
N = (num_columns - 1) / 4  # Restamos columna de tiempo, cada partícula tiene 4 columnas

print "Número de partículas detectadas: ", N
print "Total de frames: ", total_frames

# Muestreo para no hacer el GIF demasiado grande
muestreo = 1
if (total_frames > 500) {
    muestreo = int(total_frames / 300)
    print "Muestreando: usando 1 de cada ", muestreo, " frames"
}

print "Frames a procesar: ", int(total_frames/muestreo)

# Función para obtener columnas de posición
part_x(i) = 2 + 4*(i-1)    # Columna x para partícula i
part_y(i) = 3 + 4*(i-1)    # Columna y para partícula i

# Generar la animación
frame_count = 0
do for [i=0:total_frames-1:muestreo] {
    # Leer el tiempo del frame actual
    stats '../results/trayectorias.dat' every ::i::i using 1 nooutput
    current_time = STATS_min
    
    set title sprintf('Tiempo: %.2f s - Frame: %d/%d', current_time, frame_count+1, int(total_frames/muestreo))
    
    # Plot para cada partícula - TODAS CON EL MISMO COLOR Y FORMA
    plot for [j=1:N] '../results/trayectorias.dat' \
        every ::i::i \
        using (column(part_x(j))):(column(part_y(j))) \
        with points pt 7 ps 1.5 lc rgb 'purple' title ''
    
    frame_count = frame_count + 1
}

set output  # Cerrar el archivo GIF

print " "
print "¡Animación completada!"
print "GIF guardado en: ../results/animacion_billar.gif"
print "Resumen:"
print "  - Dimensiones: ", W, "x", H
print "  - Frames procesados: ", frame_count
print "  - Partículas: ", N