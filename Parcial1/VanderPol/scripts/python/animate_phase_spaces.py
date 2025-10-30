#!/usr/bin/env python3
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import os
import sys
from pathlib import Path

def create_phase_space_gif(datafile, output_gif, x_data, y_data, x_label, y_label, title, max_frames=300, fps=25):
    """Crea un GIF animado del espacio de fase"""
    
    print(f"   Creando GIF {title} con {len(x_data)} puntos...")
    
    # Limitar frames para GIF razonable
    step = max(1, len(x_data) // max_frames)
    indices = range(0, len(x_data), step)
    
    fig, ax = plt.subplots(figsize=(8, 8))
    
    # Calcular límites con márgenes
    x_range = max(x_data) - min(x_data)
    y_range = max(y_data) - min(y_data)
    x_margin = x_range * 0.1
    y_margin = y_range * 0.1
    
    ax.set_xlim(min(x_data) - x_margin, max(x_data) + x_margin)
    ax.set_ylim(min(y_data) - y_margin, max(y_data) + y_margin)
    ax.set_xlabel(x_label)
    ax.set_ylabel(y_label)
    ax.set_title(title)
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
        
        # Actualizar línea (historial)
        line.set_data(x_data[:idx+1:step], y_data[:idx+1:step])
        
        # Actualizar punto (posición actual)
        point.set_data([x_data[idx]], [y_data[idx]])
        
        # Actualizar texto
        progress = (i + 1) / len(indices)
        time_text.set_text(f'Puntos: {idx+1}/{len(x_data)}\nProgreso: {progress:.1%}')
        
        return line, point, time_text
    
    print(f"   Generando animación {title}...")
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
        print(f"❌ Error guardando GIF {title}: {e}")
        plt.close()
        return False

def main():
    # Obtener parámetros
    datafile = sys.argv[1] if len(sys.argv) > 1 else None
    output_dir = sys.argv[2] if len(sys.argv) > 2 else None
    
    if not datafile or not output_dir:
        print("Uso: python animate_phase_spaces.py <datafile> <output_dir>")
        sys.exit(1)
    
    # Asegurar directorio de salida
    output_path = Path(output_dir)
    output_path.mkdir(parents=True, exist_ok=True)
    
    try:
        data = np.loadtxt(datafile)
        if data.size == 0:
            print("❌ Archivo vacío")
            return 1
    except Exception as e:
        print(f"❌ Error leyendo archivo: {e}")
        return 1
    
    t, x1, v1, x2, v2 = data[:,0], data[:,1], data[:,2], data[:,3], data[:,4]
    
    print(f"📊 Procesando {len(t)} puntos para GIFs de espacios de fase...")
    
    # Crear GIF para el espacio de fase del oscilador 1 (x1 vs v1)
    success1 = create_phase_space_gif(
        datafile, 
        output_path / "phase_space_osc1.gif",
        x1, v1, 
        'x1', 'v1', 
        'Evolución del Espacio de Fase\nOscilador 1 (x1 vs v1)'
    )
    
    # Crear GIF para el espacio de fase del oscilador 2 (x2 vs v2)
    success2 = create_phase_space_gif(
        datafile,
        output_path / "phase_space_osc2.gif", 
        x2, v2,
        'x2', 'v2',
        'Evolución del Espacio de Fase\nOscilador 2 (x2 vs v2)'
    )
    
    # Crear GIF para el espacio de fase conjunto (x1 vs x2) - Lissajous
    success3 = create_phase_space_gif(
        datafile,
        output_path / "lissajous_evolution.gif",
        x1, x2,
        'x1', 'x2', 
        'Evolución de la Figura de Lissajous\nx1 vs x2'
    )
    
    if success1 and success2 and success3:
        print("✅ Todos los GIFs de espacios de fase creados exitosamente")
        return 0
    else:
        print("⚠️ Algunos GIFs no se pudieron crear")
        return 1

if __name__ == "__main__":
    sys.exit(main())