# --- Configuración del terminal para crear un GIF animado ---
set terminal gif animate delay 5 size 600,600
set output '../results/billar.gif'

# --- Parámetros de la caja y del archivo ---
datafile = '../results/trayectorias.dat'

# --- LECTURA DINÁMICA DE PARÁMETROS ---
# Función para extraer un valor (float) de una línea con formato "# Etiqueta: Valor"
get_param(tag) = real(system(sprintf("grep '^# %s:' %s | awk '{print $NF}'", tag, datafile)))

# Leer W, H y el radio r de las bolas
W = get_param("W")
H = get_param("H")
r = get_param("R_BOLA")

print "Dimensiones de la caja cargadas: W=", W, ", H=", H, ", R=", r

# --- Configuración de los ejes y estilo ---
set xrange [0:W]
set yrange [0:H]
set size ratio -1 # Mantiene la proporción de los ejes
set key off      # Desactiva la leyenda
set grid

# --- Contar el número de partículas (bolas) ---
# Se lee la primera línea de datos (encabezado después de los comentarios)
header = system("grep -v '#' ".datafile." | head -n 1")
# (N_columnas - 1) / 4 columnas por partícula (x,y,vx,vy)
N_particles = (words(header) - 1) / 4

# --- Bucle de animación ---
# stats cuenta el número de "bloques" de datos (fotogramas)
# Es necesario usar 'grep -v' para que stats ignore las líneas de comentario nuevas
stats '<grep -v "#" '.datafile nooutput
n_frames = STATS_blocks

do for [i=0:n_frames-1] {
    # El título muestra el tiempo actual, leído de la primera columna
    # También es necesario usar 'grep -v' aquí para el índice
    stats '<grep -v "#" '.datafile index i using 1 nooutput
    current_time = STATS_min
    set title sprintf("Simulación de Partículas | Tiempo = %.2fs | W=%.1f, H=%.1f", current_time, W, H)

    # Comando para graficar todas las partículas en el fotograma 'i'
    # La fuente de datos debe ser el resultado de 'grep -v "#" trayectorias.dat'
    plot for [j=1:N_particles] '<grep -v "#" '.datafile index i using (column(4*(j-1)+2)):(column(4*(j-1)+3)):(r) with circles lc j
}

print "GIF guardado en ../results/billar.gif"
