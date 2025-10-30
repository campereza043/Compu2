#!/usr/bin/env python3
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import os
import sys
from pathlib import Path

def create_lissajous_gif(datafile, output_gif, max_frames=300, fps=25):
    """Crea un GIF animado de la figura de Lissajous"""
    
    try:
        data = np.loadtxt(datafile)
        if data.size == 0:
            print("❌ Archivo vacío")
            return False
    except Exception as e:
        print(f"❌ Error leyendo archivo: {e}")
        return False
    
    t, x1, v1, x2, v2 = data[:,0], data[:,1], data[:,2], data[:,3], data[:,4]
    
    print(f"   Creando GIF con {len(t)} puntos...")
    
    # Limitar frames para GIF razonable
    step = max(1, len(t) // max_frames)
    indices = range(0, len(t), step)
    
    fig, ax = plt.subplots(figsize=(8, 8))
    
    # Calcular límites con márgenes
    x1_range = max(x1) - min(x1)
    x2_range = max(x2) - min(x2)
    x1_margin = x1_range * 0.1
    x2_margin = x2_range * 0.1
    
    ax.set_xlim(min(x1) - x1_margin, max(x1) + x1_margin)
    ax.set_ylim(min(x2) - x2_margin, max(x2) + x2_margin)
    ax.set_xlabel('x1')
    ax.set_ylabel('x2')
    ax.set_title('Evolución de la Figura de Lissajous\nOsciladores de Van der Pol Acoplados')
    ax.grid(True, alpha=0.3)
    
    line, = ax.plot([], [], 'b-', alpha=0.8, linewidth=1.2)
    point, = ax.plot([], [], 'ro', markersize=4)
    time_text = ax.text(0.02, 0.95, '', transform=ax.transAxes, fontsize=12,
                       bbox=dict(boxstyle="round,pad=0.3", facecolor="white", alpha=0.8))
    
    def init():
        line.set_data([], [])
        point.set_data([], [])
        time_text.set_text('')
        return line, point, time_text
    
    def animate(i):
        idx = indices[i]
        current_t = t[idx]
        
        # Actualizar línea (historial)
        line.set_data(x1[:idx+1:step], x2[:idx+1:step])
        
        # Actualizar punto (posición actual)
        point.set_data([x1[idx]], [x2[idx]])
        
        # Actualizar texto
        progress = (i + 1) / len(indices)
        time_text.set_text(f'Tiempo: {current_t:.1f}s\nProgreso: {progress:.1%}')
        
        return line, point, time_text
    
    print("   Generando animación...")
    anim = animation.FuncAnimation(
        fig, animate, init_func=init,
        frames=len(indices), interval=1000/fps, 
        blit=True, repeat=True
    )
    
    print(f"   Guardando GIF: {output_gif}")
    try:
        anim.save(output_gif, writer='pillow', fps=fps, dpi=120)
        plt.close()
        return True
    except Exception as e:
        print(f"❌ Error guardando GIF: {e}")
        plt.close()
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