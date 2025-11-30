# graficar.py - Script Python para GIF, trayectorias e histograma de velocidades
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
import os
from matplotlib import gridspec

def main():
    # Leer parámetros del archivo
    W, H, R_BOLA = leer_parametros()
    
    print("Parámetros leídos del archivo:")
    print(f"  Ancho de caja (W): {W}")
    print(f"  Alto de caja (H): {H}")
    print(f"  Radio de partículas: {R_BOLA}")
    
    # Leer datos de trayectorias
    tiempos, datos = leer_datos_trayectorias()
    
    N = datos.shape[1] // 4  # Número de partículas (cada una tiene x,y,vx,vy)
    total_frames = len(tiempos)
    
    print(f"Número de partículas detectadas: {N}")
    print(f"Total de frames: {total_frames}")
    
    # =============================================
    # 1. GENERAR GIF ANIMADO
    # =============================================
    print("\n" + "="*50)
    print("GENERANDO GIF ANIMADO")
    print("="*50)
    
    # Configurar muestreo si hay muchos frames
    muestreo_gif = 1
    if total_frames > 500:
        muestreo_gif = total_frames // 300
        print(f"Muestreando: usando 1 de cada {muestreo_gif} frames")
    
    frames_a_procesar = total_frames // muestreo_gif
    print(f"Frames a procesar: {frames_a_procesar}")
    
    # Crear figura y ejes para GIF
    fig_gif, ax_gif = plt.subplots(figsize=(8, 6))
    
    # Configurar los límites y aspecto
    ax_gif.set_xlim(0, W)
    ax_gif.set_ylim(0, H)
    ax_gif.set_aspect('equal')
    ax_gif.grid(True)
    ax_gif.set_xlabel('x')
    ax_gif.set_ylabel('y')
    
    # Dibujar el borde de la caja
    rect = plt.Rectangle((0, 0), W, H, fill=False, edgecolor='black', linewidth=2)
    ax_gif.add_patch(rect)
    
    # Inicializar puntos para las partículas (todas del mismo color y forma)
    puntos = ax_gif.plot([], [], 'bo', markersize=6)[0]  # 'bo': puntos azules
    
    def animar(frame):
        # Calcular el índice real considerando el muestreo
        idx = frame * muestreo_gif
        if idx >= total_frames:
            idx = total_frames - 1
        
        # Obtener datos para este frame
        frame_data = datos[idx]
        
        # Separar coordenadas x e y de todas las partículas
        x_coords = frame_data[0::4]  # x0, x1, x2, ... (cada 4 columnas)
        y_coords = frame_data[1::4]  # y0, y1, y2, ... (cada 4 columnas, desplazado 1)
        
        # Actualizar los puntos
        puntos.set_data(x_coords, y_coords)
        
        # Actualizar título
        ax_gif.set_title(f'Tiempo: {tiempos[idx]:.2f} s - Frame: {frame+1}/{frames_a_procesar}')
        
        return puntos,
    
    # Crear la animación
    anim = animation.FuncAnimation(
        fig_gif, animar, frames=frames_a_procesar,
        interval=50, blit=True, repeat=True
    )
    
    # Guardar el GIF
    output_gif_path = '../results/animacion_billar_python.gif'
    anim.save(output_gif_path, writer='pillow', fps=20)
    
    print(f"✓ GIF guardado en: {output_gif_path}")
    
    # Cerrar la figura del GIF para liberar memoria
    plt.close(fig_gif)
    
    # =============================================
    # 2. GENERAR DIAGRAMA DE TRAYECTORIAS (ARCHIVO SEPARADO)
    # =============================================
    print("\n" + "="*50)
    print("GENERANDO DIAGRAMA DE TRAYECTORIAS")
    print("="*50)
    
    # Crear figura para trayectorias
    fig_tray, ax_tray = plt.subplots(figsize=(8, 6))
    
    # Configurar el gráfico de trayectorias
    ax_tray.set_xlim(0, W)
    ax_tray.set_ylim(0, H)
    ax_tray.set_aspect('equal')
    ax_tray.grid(True)
    ax_tray.set_xlabel('x')
    ax_tray.set_ylabel('y')
    ax_tray.set_title('Trayectorias de las Partículas')
    
    # Dibujar el borde de la caja
    rect_tray = plt.Rectangle((0, 0), W, H, fill=False, edgecolor='black', linewidth=2)
    ax_tray.add_patch(rect_tray)
    
    # Graficar trayectorias para cada partícula
    for i in range(N):
        # Extraer coordenadas x e y para la partícula i
        x_coords = datos[:, 4*i]     # x en posición 0, 4, 8, ...
        y_coords = datos[:, 4*i + 1] # y en posición 1, 5, 9, ...
        
        # Graficar trayectoria
        ax_tray.plot(x_coords, y_coords, 'o', markersize=1.5, alpha=0.7)
    
    # Ajustar diseño y guardar
    plt.tight_layout()
    output_trayectorias_path = '../results/trayectorias_py.png'
    plt.savefig(output_trayectorias_path, dpi=300, bbox_inches='tight')
    plt.close(fig_tray)
    
    print(f"✓ Diagrama de trayectorias guardado en: {output_trayectorias_path}")
    
    # =============================================
    # 3. GENERAR HISTOGRAMA DE VELOCIDADES (ARCHIVO SEPARADO)
    # =============================================
    print("\n" + "="*50)
    print("GENERANDO HISTOGRAMA DE VELOCIDADES")
    print("="*50)
    
    # Procesar datos para velocidades
    magnitudes_velocidad = []
    
    # Recolectar velocidades de todas las partículas
    for i in range(N):
        # Extraer componentes de velocidad para la partícula i
        vx = datos[:, 4*i + 2]  # vx en posición 2, 6, 10, ...
        vy = datos[:, 4*i + 3]  # vy en posición 3, 7, 11, ...
        
        # Calcular magnitud de velocidad para esta partícula
        v_magnitudes = np.sqrt(vx**2 + vy**2)
        magnitudes_velocidad.extend(v_magnitudes)
    
    # Crear figura para histograma
    fig_hist, ax_hist = plt.subplots(figsize=(8, 6))
    
    magnitudes_velocidad = np.array(magnitudes_velocidad)
    
    # Calcular estadísticas
    velocidad_promedio = np.mean(magnitudes_velocidad)
    velocidad_max = np.max(magnitudes_velocidad)
    velocidad_min = np.min(magnitudes_velocidad)
    velocidad_std = np.std(magnitudes_velocidad)
    
    print(f"Estadísticas de velocidad:")
    print(f"  Velocidad promedio: {velocidad_promedio:.4f}")
    print(f"  Velocidad mínima: {velocidad_min:.4f}")
    print(f"  Velocidad máxima: {velocidad_max:.4f}")
    print(f"  Desviación estándar: {velocidad_std:.4f}")
    
    # Crear histograma
    n, bins, patches = ax_hist.hist(magnitudes_velocidad, bins=30, alpha=0.7, 
                                   color='purple', edgecolor='black', density=True)
    
    ax_hist.set_xlabel('Magnitud de Velocidad')
    ax_hist.set_ylabel('Densidad de Probabilidad')
    ax_hist.set_title('Distribución de Velocidades')
    ax_hist.grid(True, alpha=0.3)
    
    # Añadir línea para la velocidad promedio
    ax_hist.axvline(velocidad_promedio, color='pink', linestyle='--', linewidth=2, 
                   label=f'Promedio: {velocidad_promedio:.3f}')
    
    # Añadir líneas para ±1 desviación estándar
    ax_hist.axvline(velocidad_promedio - velocidad_std, color='orange', linestyle=':', 
                   linewidth=1.5, alpha=0.7, label=f'±1σ: {velocidad_std:.3f}')
    ax_hist.axvline(velocidad_promedio + velocidad_std, color='orange', linestyle=':', 
                   linewidth=1.5, alpha=0.7)
    
    ax_hist.legend()
    
    # Ajustar diseño y guardar
    plt.tight_layout()
    output_histograma_path = '../results/histograma_velocidades_py.png'
    plt.savefig(output_histograma_path, dpi=300, bbox_inches='tight')
    plt.close(fig_hist)
    
    print(f"✓ Histograma de velocidades guardado en: {output_histograma_path}")
    
    # =============================================
    # RESUMEN FINAL
    # =============================================
    print("\n" + "="*50)
    print("RESUMEN EJECUCIÓN")
    print("="*50)
    print(f"✓ GIF animado: {output_gif_path}")
    print(f"✓ Diagrama de trayectorias: {output_trayectorias_path}")
    print(f"✓ Histograma de velocidades: {output_histograma_path}")
    print(f"✓ Dimensiones de la caja: {W} x {H}")
    print(f"✓ Partículas simuladas: {N}")
    print(f"✓ Tiempo total de simulación: {tiempos[-1]:.2f} s")
    print(f"✓ Frames procesados en GIF: {frames_a_procesar}")
    print(f"✓ Velocidad promedio: {velocidad_promedio:.4f}")
    print("¡Todos los gráficos han sido generados exitosamente!")

def leer_parametros():
    """Leer parámetros W, H y R_BOLA del archivo de datos"""
    file_path = '../results/trayectorias.dat'
    
    W, H, R_BOLA = 1, 1, 0.2  # Valores por defecto
    
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