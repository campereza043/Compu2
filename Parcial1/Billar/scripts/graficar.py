# graficar.py - Script Python para visualización de partículas
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
import os

def main():
    # Leer parámetros del archivo
    W, H, R_BOLA = leer_parametros()
    
    print("Parámetros leídos del archivo:")
    print(f"  Ancho de caja (W): {W}")
    print(f"  Alto de caja (H): {H}")
    print(f"  Radio de partículas: {R_BOLA}")
    
    # Leer datos de trayectorias
    tiempos, datos = leer_datos_trayectorias()
    
    N = datos.shape[1] // 2  # Número de partículas (cada una tiene x,y)
    total_frames = len(tiempos)
    
    print(f"Número de partículas detectadas: {N}")
    print(f"Total de frames: {total_frames}")
    
    # Configurar muestreo si hay muchos frames
    muestreo = 1
    if total_frames > 500:
        muestreo = total_frames // 300
        print(f"Muestreando: usando 1 de cada {muestreo} frames")
    
    frames_a_procesar = total_frames // muestreo
    print(f"Frames a procesar: {frames_a_procesar}")
    
    # Crear figura y ejes
    fig, ax = plt.subplots(figsize=(8, 6))
    
    # Configurar los límites y aspecto
    ax.set_xlim(0, W)
    ax.set_ylim(0, H)
    ax.set_aspect('equal')
    ax.grid(True)
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    
    # Dibujar el borde de la caja
    rect = plt.Rectangle((0, 0), W, H, fill=False, edgecolor='black', linewidth=2)
    ax.add_patch(rect)
    
    # Inicializar puntos para las partículas (todas del mismo color y forma)
    puntos = ax.plot([], [], 'bo', markersize=6)[0]  # 'bo': puntos azules
    
    def animar(frame):
        # Calcular el índice real considerando el muestreo
        idx = frame * muestreo
        if idx >= total_frames:
            idx = total_frames - 1
        
        # Obtener datos para este frame
        frame_data = datos[idx]
        
        # Separar coordenadas x e y de todas las partículas
        x_coords = frame_data[0::2]  # Todas las x (columnas pares: 0, 2, 4, ...)
        y_coords = frame_data[1::2]  # Todas las y (columnas impares: 1, 3, 5, ...)
        
        # Actualizar los puntos
        puntos.set_data(x_coords, y_coords)
        
        # Actualizar título
        ax.set_title(f'Tiempo: {tiempos[idx]:.2f} s - Frame: {frame+1}/{frames_a_procesar}')
        
        return puntos,
    
    # Crear la animación
    anim = animation.FuncAnimation(
        fig, animar, frames=frames_a_procesar,
        interval=50, blit=True, repeat=True
    )
    
    # Guardar el GIF
    output_path = '../results/animacion_billar_python.gif'
    anim.save(output_path, writer='pillow', fps=20)
    
    print("\n¡Animación completada!")
    print(f"GIF guardado en: {output_path}")
    print("Resumen:")
    print(f"  - Dimensiones: {W} x {H}")
    print(f"  - Frames procesados: {frames_a_procesar}")
    print(f"  - Partículas: {N}")

def leer_parametros():
    """Leer parámetros W, H y R_BOLA del archivo de datos"""
    file_path = '../results/trayectorias.dat'
    
    W, H, R_BOLA = 1, 1, 0.1  # Valores por defecto
    
    try:
        with open(file_path, 'r') as f:
            for line in f:
                if line.startswith('# W:'):
                    W = float(line.split()[2])
                elif line.startswith('# H:'):
                    H = float(line.split()[2])
                elif line.startswith('# R_BOLA:'):
                    R_BOLA = float(line.split()[2])
                elif not line.startswith('#'):
                    # Llegamos a los datos, salir del bucle
                    break
    except FileNotFoundError:
        print(f"Error: No se encontró el archivo {file_path}")
        exit(1)
    except Exception as e:
        print(f"Error leyendo parámetros: {e}")
        exit(1)
    
    return W, H, R_BOLA

def leer_datos_trayectorias():
    """Leer los datos de trayectorias del archivo"""
    file_path = '../results/trayectorias.dat'
    
    tiempos = []
    datos = []
    
    try:
        with open(file_path, 'r') as f:
            for line in f:
                # Saltar líneas de comentarios
                if line.startswith('#'):
                    continue
                
                # Procesar línea de datos
                partes = line.split()
                if len(partes) > 0:
                    # Primera columna es el tiempo
                    tiempos.append(float(partes[0]))
                    # El resto son datos de partículas (x0, y0, vx0, vy0, x1, y1, ...)
                    datos_particulas = [float(x) for x in partes[1:]]
                    datos.append(datos_particulas)
    
    except Exception as e:
        print(f"Error leyendo datos de trayectorias: {e}")
        exit(1)
    
    return np.array(tiempos), np.array(datos)

if __name__ == "__main__":
    main()