#!/usr/bin/env python3
"""
Script de Python para visualizar los resultados de la simulación
"""

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import sys
import os

def leer_datos(archivo):
    """Lee el archivo de datos de la simulación"""
    try:
        data = np.loadtxt(archivo)
    except Exception as e:
        print(f"Error al leer el archivo: {e}")
        sys.exit(1)

    # Manejar caso de array 1D (solo una línea de datos)
    if data.ndim == 1:
        data = data.reshape(1, -1)
    
    tiempos = data[:, 0]
    
    # Determinar número de partículas
    n_columnas = data.shape[1] - 1  # Excluir columna de tiempo
    n_particulas = n_columnas // 4  # Cada partícula tiene x, y, vx, vy
    
    # Reorganizar datos
    posiciones = np.zeros((len(tiempos), n_particulas, 2))
    velocidades = np.zeros((len(tiempos), n_particulas, 2))
    
    for i in range(n_particulas):
        posiciones[:, i, 0] = data[:, 1 + i*4]     # x
        posiciones[:, i, 1] = data[:, 2 + i*4]     # y
        velocidades[:, i, 0] = data[:, 3 + i*4]    # vx
        velocidades[:, i, 1] = data[:, 4 + i*4]    # vy
    
    return tiempos, posiciones, velocidades, n_particulas

def graficar_trayectorias(archivo_datos):
    """Grafica las trayectorias de todas las partículas"""
    tiempos, posiciones, velocidades, n_particulas = leer_datos(archivo_datos)
    
    plt.figure(figsize=(12, 8))
    
    # Graficar trayectorias
    for i in range(min(n_particulas, 20)):  # Máximo 20 partículas para claridad
        plt.plot(posiciones[:, i, 0], posiciones[:, i, 1], 
                alpha=0.7, linewidth=1, label=f'Partícula {i+1}')
    
    plt.xlabel('x')
    plt.ylabel('y')
    plt.title(f'Trayectorias de {n_particulas} partículas')
    plt.grid(True, alpha=0.3)
    if n_particulas <= 10:
        plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left')
    plt.tight_layout()
    plt.savefig('results/trayectorias.png', dpi=300, bbox_inches='tight')
    plt.show()

def histograma_velocidades(archivo_datos):
    """Grafica histograma de distribución de velocidades"""
    tiempos, posiciones, velocidades, n_particulas = leer_datos(archivo_datos)
    
    # Calcular magnitudes de velocidad
    v_magnitudes = np.linalg.norm(velocidades, axis=2)
    
    # Usar todos los datos
    v_flat = v_magnitudes.flatten()
    
    plt.figure(figsize=(10, 6))
    
    # Histograma
    n, bins, patches = plt.hist(v_flat, bins=50, density=True, 
                               alpha=0.7, color='skyblue', edgecolor='black')
    
    # Distribución teórica Maxwell-Boltzmann (para comparación)
    v_mean = np.mean(v_flat)
    v_rms = np.sqrt(np.mean(v_flat**2))
    
    # Función de distribución de Maxwell-Boltzmann en 2D
    def maxwell_boltzmann_2d(v, sigma):
        return (v / sigma**2) * np.exp(-v**2 / (2 * sigma**2))
    
    sigma = v_rms / np.sqrt(2)  # Para distribución 2D
    v_plot = np.linspace(0, np.max(v_flat), 1000)
    plt.plot(v_plot, maxwell_boltzmann_2d(v_plot, sigma), 
             'r-', linewidth=2, label='Maxwell-Boltzmann (2D)')
    
    plt.xlabel('Velocidad |v|')
    plt.ylabel('Densidad de probabilidad')
    plt.title('Distribución de velocidades')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.savefig('results/histograma_velocidades.png', dpi=300)
    plt.show()
    
    print(f"Velocidad media: {v_mean:.4f}")
    print(f"Velocidad RMS: {v_rms:.4f}")

def animar_simulacion(archivo_datos):
    """Crea una animación de la simulación"""
    tiempos, posiciones, velocidades, n_particulas = leer_datos(archivo_datos)
    
    # Limitar a 50 frames para no hacer la animación demasiado pesada
    skip = max(1, len(tiempos) // 50)
    indices = range(0, len(tiempos), skip)
    
    fig, ax = plt.subplots(figsize=(10, 8))
    
    # Configurar límites del gráfico
    margin = 0.1
    x_min, x_max = np.min(posiciones[:, :, 0]), np.max(posiciones[:, :, 0])
    y_min, y_max = np.min(posiciones[:, :, 1]), np.max(posiciones[:, :, 1])
    x_range = x_max - x_min
    y_range = y_max - y_min
    
    ax.set_xlim(x_min - margin * x_range, x_max + margin * x_range)
    ax.set_ylim(y_min - margin * y_range, y_max + margin * y_range)
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_title('Simulación de partículas en caja')
    ax.grid(True, alpha=0.3)
    
    # Crear scatter plot para las partículas
    scatter = ax.scatter([], [], s=50, alpha=0.7, c='blue')
    
    def animate(frame_idx):
        frame = indices[frame_idx]
        scatter.set_offsets(posiciones[frame])
        ax.set_title(f'Simulación - Tiempo: {tiempos[frame]:.2f}')
        return scatter,
    
    # Crear animación
    anim = FuncAnimation(fig, animate, frames=len(indices), 
                        interval=100, blit=True, repeat=True)
    
    plt.tight_layout()
    
    # Guardar animación
    print("Guardando animación...")
    anim.save('results/animacion_simulacion.gif', writer='pillow', fps=10)
    print("Animación guardada como 'results/animacion_simulacion.gif'")
    
    plt.show()

if __name__ == "__main__":
    archivo = "results/datos_simulacion.txt"
    
    if not os.path.exists(archivo):
        print(f"Error: No se encuentra el archivo {archivo}")
        print("Ejecute primero la simulación")
        sys.exit(1)
    
    # Verificar si el archivo tiene datos
    if os.path.getsize(archivo) == 0:
        print(f"Error: El archivo {archivo} está vacío")
        print("La simulación no generó datos. Verifique los parámetros.")
        sys.exit(1)
    
    print("Seleccione tipo de visualización:")
    print("1. Trayectorias")
    print("2. Histograma de velocidades") 
    print("3. Animación")
    
    try:
        opcion = input("Opción (1-3): ").strip()
        
        if opcion == "1":
            graficar_trayectorias(archivo)
        elif opcion == "2":
            histograma_velocidades(archivo)
        elif opcion == "3":
            animar_simulacion(archivo)
        else:
            print("Opción no válida")
    except KeyboardInterrupt:
        print("\nEjecución interrumpida por el usuario")
    except Exception as e:
        print(f"Error durante la visualización: {e}")
