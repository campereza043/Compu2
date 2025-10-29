# graficar.gnuplot - Script completo para GIF, trayectorias e histograma
reset session

# =============================================
# CONFIGURACIÓN INICIAL Y LECTURA DE PARÁMETROS
# =============================================

print "INICIANDO GENERACIÓN DE GRÁFICOS"

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

# Configuración para leer el archivo
set datafile commentschars "#"
set datafile separator whitespace

# Contar número de frames y determinar número de partículas
stats '../results/trayectorias.dat' using 1 nooutput
total_frames = STATS_records

stats '../results/trayectorias.dat' using 2 nooutput
num_columns = STATS_columns
N = (num_columns - 1) / 4  # Restamos columna de tiempo, cada partícula tiene 4 columnas

print "Número de partículas detectadas: ", N
print "Total de frames: ", total_frames

# =============================================
# 1. GENERAR GIF ANIMADO
# =============================================
print "\n"
print "GENERANDO GIF ANIMADO"

# Configuración del terminal para GIF animado
set terminal gif animate delay 5 loop 0 size 800,600
set output '../results/animacion_billar.gif'

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

# Muestreo para no hacer el GIF demasiado grande
muestreo = 1
if (total_frames > 500) {
    muestreo = int(total_frames / 300)
    print "Muestreando: usando 1 de cada ", muestreo, " frames"
}

frames_a_procesar = int(total_frames/muestreo)
print "Frames a procesar: ", frames_a_procesar

# Función para obtener columnas de posición
part_x(i) = 2 + 4*(i-1)    # Columna x para partícula i
part_y(i) = 3 + 4*(i-1)    # Columna y para partícula i

# Generar la animación
frame_count = 0
do for [i=0:total_frames-1:muestreo] {
    # Leer el tiempo del frame actual
    stats '../results/trayectorias.dat' every ::i::i using 1 nooutput
    current_time = STATS_min
    
    set title sprintf('Tiempo: %.2f s - Frame: %d/%d', current_time, frame_count+1, frames_a_procesar)
    
    # Plot para cada partícula - TODAS CON EL MISMO COLOR Y FORMA
    plot for [j=1:N] '../results/trayectorias.dat' \
        every ::i::i \
        using (column(part_x(j))):(column(part_y(j))) \
        with points pt 7 ps 1.5 lc rgb 'purple' title ''
    
    frame_count = frame_count + 1
}

set output  # Cerrar el archivo GIF

print "✓ GIF animado guardado en: ../results/animacion_billar.gif"

# =============================================
# 2. GENERAR DIAGRAMA DE TRAYECTORIAS
# =============================================
print "\n"
print "GENERANDO DIAGRAMA DE TRAYECTORIAS"

# Configuración para PNG
set terminal pngcairo size 800,600 enhanced font 'Arial,12'
set output '../results/trayectorias_gp.png'

# Configuración del gráfico
set xrange [0:W]
set yrange [0:H]
set size ratio -1
set grid
set xlabel 'x'
set ylabel 'y'
set title 'Trayectorias de las Partículas'
set key off

# Dibujar el borde de la caja
set object 1 rect from 0,0 to W,H front fillstyle empty border lc rgb 'black' lw 2

# Graficar todas las trayectorias
plot for [j=1:N] '../results/trayectorias.dat' \
    using (column(part_x(j))):(column(part_y(j))) \
    with points pt 7 ps 0.5 lc rgb 'purple' title ''

set output
print "✓ Diagrama de trayectorias guardado en: ../results/trayectorias_gp.png"

# =============================================
# 3. GENERAR HISTOGRAMA DE VELOCIDADES (SOLO GNUPLOT)
# =============================================
print "\n"
print "GENERANDO HISTOGRAMA DE VELOCIDADES"

# Configuración para PNG del histograma
set terminal pngcairo size 800,600 enhanced font 'Arial,12'
set output '../results/histograma_velocidades_gp.png'

# Resetear configuraciones para el histograma
unset object
set size noratio
set autoscale

# Función para obtener columnas de velocidad
vel_x(i) = 4 + 4*(i-1)    # Columna vx para partícula i
vel_y(i) = 5 + 4*(i-1)    # Columna vy para partícula i

# Crear archivo temporal usando solo gnuplot
set print '../results/velocidades_tmp.dat'
do for [i=0:total_frames-1] {
    stats '../results/trayectorias.dat' every ::i::i using 1 nooutput
    do for [j=1:N] {
        stats '../results/trayectorias.dat' every ::i::i using (column(vel_x(j))) nooutput
        vx_val = STATS_min
        stats '../results/trayectorias.dat' every ::i::i using (column(vel_y(j))) nooutput
        vy_val = STATS_min
        velocidad = sqrt(vx_val**2 + vy_val**2)
        print sprintf("%.6f", velocidad)
    }
}
set print

# Leer el archivo temporal y calcular estadísticas
stats '../results/velocidades_tmp.dat' using 1 nooutput

velocidad_promedio = STATS_mean
velocidad_min = STATS_min
velocidad_max = STATS_max
velocidad_std = STATS_stddev

print "Estadísticas de velocidad:"
print sprintf("  Velocidad promedio: %.4f", velocidad_promedio)
print sprintf("  Velocidad mínima: %.4f", velocidad_min)
print sprintf("  Velocidad máxima: %.4f", velocidad_max)
print sprintf("  Desviación estándar: %.4f", velocidad_std)

# Configurar el histograma
set xlabel 'Magnitud de Velocidad'
set ylabel 'Frecuencia'
set title 'Distribución de Velocidades'
set grid
set key top right

# Crear histograma usando smooth frequency
bin_number = 30
set style fill solid 0.5 border

plot '../results/velocidades_tmp.dat' using 1 smooth frequency with boxes lc rgb 'purple' title 'Distribución', \
     velocidad_promedio with lines lw 2 lc rgb 'red' title sprintf('Promedio: %.3f', velocidad_promedio)

set output
print "✓ Histograma de velocidades guardado en: ../results/histograma_velocidades_gp.png"

# Limpiar archivo temporal
system("rm -f ../results/velocidades_tmp.dat")

# =============================================
# RESUMEN FINAL
# =============================================
print "\n"
print "RESUMEN EJECUCIÓN"
print "✓ GIF animado: ../results/animacion_billar.gif"
print "✓ Diagrama de trayectorias: ../results/trayectorias_gp.png"
print "✓ Histograma de velocidades: ../results/histograma_velocidades_gp.png"
print "✓ Dimensiones de la caja: ", W, " x ", H
print "✓ Partículas simuladas: ", N
print "✓ Tiempo total de simulación: ", current_time, " s"
print "✓ Frames procesados en GIF: ", frame_count
print sprintf("✓ Velocidad promedio: %.4f", velocidad_promedio)
print "¡Todos los gráficos han sido generados exitosamente!"