import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import os
import sys
from pathlib import Path

# Se elimina el parámetro max_frames de aquí, usaremos la longitud total de los datos.
def create_lissajous_gif(datafile, output_gif, fps=240): 
    """Crea un GIF animado de la figura de Lissajous utilizando la máxima resolución de datos."""
    
    try:
        data = np.loadtxt(datafile)
        if data.size == 0:
            print(" Archivo vacío")
            return False
    except Exception as e:
        print(f" Error leyendo archivo: {e}")
        return False
    
    if data.shape[1] < 5:
        print(" Error: El archivo de datos debe tener al menos 5 columnas (t, x1, v1, x2, v2).")
        return False

    t, x1, v1, x2, v2 = data[:,0], data[:,1], data[:,2], data[:,3], data[:,4]
    
    # -------------------------------------------------------------
    # CAMBIO CLAVE: Usar la longitud total de los datos para los frames.
    # Esto elimina el "salto" de muestreo.
    num_data_points = len(t)
    indices = range(num_data_points)
    # -------------------------------------------------------------
    
    print(f"    Creando GIF con la máxima resolución: {num_data_points} puntos/frames...")
    
    fig, ax = plt.subplots(figsize=(8, 8))
    
    # Calcular límites con márgenes
    x1_margin = (max(x1) - min(x1)) * 0.1
    x2_margin = (max(x2) - min(x2)) * 0.1
    
    ax.set_xlim(min(x1) - x1_margin, max(x1) + x1_margin)
    ax.set_ylim(min(x2) - x2_margin, max(x2) + x2_margin)
    ax.set_xlabel('x1')
    ax.set_ylabel('x2')
    ax.set_title('Evolución de la Figura de Lissajous\nOsciladores de Van der Pol Acoplados')
    ax.grid(True, alpha=0.3)
    
    # El historial (línea) y el punto actual. 
    line, = ax.plot([], [], 'b-', alpha=0.5, linewidth=1.5, zorder=1)
    point, = ax.plot([], [], 'ro', markersize=6, zorder=3)
    
    time_text = ax.text(0.02, 0.95, '', transform=ax.transAxes, fontsize=12,
                        bbox=dict(boxstyle="round,pad=0.3", facecolor="white", alpha=0.8))
    
    def init():
        line.set_data([], [])
        point.set_data([], [])
        time_text.set_text('')
        return line, point, time_text
    
    def animate(i):
        # i es el índice directo, ya no necesitamos la lista 'indices' ni 'step'
        idx = i 
        current_t = t[idx]
        
        # Dibuja la línea SIN saltos (usando todos los puntos hasta idx)
        line.set_data(x1[:idx+1], x2[:idx+1])
        
        # Actualiza el punto (que se mueve un solo paso de tiempo por frame)
        point.set_data([x1[idx]], [x2[idx]])
        
        # Actualizar texto
        progress = (i + 1) / num_data_points
        time_text.set_text(f'Tiempo: {current_t:.3f}s\nProgreso: {progress:.1%}')
        
        return line, point, time_text
    
    print("    Generando animación...")
    anim = animation.FuncAnimation(
        fig, animate, init_func=init,
        frames=num_data_points, # CAMBIO CLAVE: Usar todos los puntos como frames
        interval=1000/fps, 
        blit=False, # Necesario para evitar problemas visuales
        repeat=True
    )
    
    print(f"    Guardando GIF: {output_gif}")
    try:
        # Esto podría tomar más tiempo y crear un GIF grande, pero será el más suave.
        anim.save(output_gif, writer='pillow', fps=fps, dpi=120) 
        plt.close(fig)
        return True
    except Exception as e:
        print(f" Error guardando GIF: {e}. Asegúrese de tener 'Pillow' y 'ffmpeg' instalados.")
        plt.close(fig)
        return False

if __name__ == "__main__":
    # Obtener parámetros
    datafile = sys.argv[1] if len(sys.argv) > 1 else None
    output_gif = sys.argv[2] if len(sys.argv) > 2 else None
    
    if not datafile or not output_gif:
        print("Uso: python animate_lissajous.py <datafile> <output_gif>")
        sys.exit(1)
    
    # Asegurar directorio de salida
    output_path = Path(output_gif)
    output_path.parent.mkdir(parents=True, exist_ok=True)
    
    success = create_lissajous_gif(datafile, output_gif)
    sys.exit(0 if success else 1)