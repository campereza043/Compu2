import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import os

# --- Configuración de la Simulación (Debe coincidir con main.cpp) ---
FILENAME = "../results/trayectorias.csv"
W, H = 20.0, 20.0  # Dimensiones de la caja
EPSILON = 1.0      # Parámetro LJ
SIGMA = 1.0        # Parámetro LJ
MASS = 1.0         # Masa de partículas

def calcular_energia(df, N):
    """
    Calcula la energía cinética, potencial y total para cada paso de tiempo.
    """
    pasos = len(df)
    K = np.zeros(pasos)
    U = np.zeros(pasos)
    
    # Extraer posiciones y velocidades
    # Formato: t, x0, y0, vx0, vy0, x1, y1...
    
    print("Calculando energías...")
    
    for i in range(pasos):
        row = df.iloc[i]
        k_step = 0
        u_step = 0
        
        # Arrays temporales para posiciones en este paso
        xs = []
        ys = []
        
        for p in range(N):
            idx_base = 1 + p*4
            x = row.iloc[idx_base]
            y = row.iloc[idx_base+1]
            vx = row.iloc[idx_base+2]
            vy = row.iloc[idx_base+3]
            
            xs.append(x)
            ys.append(y)
            
            # Energía Cinética: 0.5 * m * v^2
            k_step += 0.5 * MASS * (vx**2 + vy**2)
            
        # Energía Potencial (Lennard-Jones)
        # Doble bucle optimizado (j > i)
        for p1 in range(N):
            for p2 in range(p1 + 1, N):
                dx = xs[p2] - xs[p1]
                dy = ys[p2] - ys[p1]
                r2 = dx*dx + dy*dy
                
                if r2 > 0: # Evitar división por cero
                    r = np.sqrt(r2)
                    sr6 = (SIGMA / r)**6
                    sr12 = sr6 * sr6
                    u_step += 4 * EPSILON * (sr12 - sr6)
        
        K[i] = k_step
        U[i] = u_step
        
    return K, U, K + U

def main():
    if not os.path.exists(FILENAME):
        print(f"Error: No se encuentra el archivo {FILENAME}")
        return

    # 1. Cargar datos
    print("Cargando datos...")
    data = pd.read_csv(FILENAME)
    
    # Deducir número de partículas
    # Columnas: t, (x, y, vx, vy) * N -> Total = 1 + 4N
    N = (data.shape[1] - 1) // 4
    print(f"Detectadas {N} partículas.")

    # 2. Análisis de Energía (Validación)
    K, U, E_total = calcular_energia(data, N)
    
    t = data['t']
    
    plt.figure(figsize=(10, 6))
    plt.plot(t, K, label='Cinética (K)', alpha=0.7)
    plt.plot(t, U, label='Potencial (U)', alpha=0.7)
    plt.plot(t, E_total, label='Total (E)', color='black', linewidth=2)
    plt.xlabel('Tiempo')
    plt.ylabel('Energía')
    plt.title(f'Conservación de Energía (Gas LJ, N={N})')
    plt.legend()
    plt.grid(True)
    plt.savefig("../results/energia.png")
    print("Gráfica de energía guardada en ../results/energia.png")

    # 3. Histograma de Velocidades (Estado Final)
    velocidades_finales = []
    last_row = data.iloc[-1]
    for p in range(N):
        vx = last_row.iloc[1 + p*4 + 2]
        vy = last_row.iloc[1 + p*4 + 3]
        v = np.sqrt(vx**2 + vy**2)
        velocidades_finales.append(v)
        
    plt.figure(figsize=(8, 6))
    plt.hist(velocidades_finales, bins=10, density=True, alpha=0.6, color='g', edgecolor='black')
    plt.xlabel('Velocidad (v)')
    plt.ylabel('Densidad de Probabilidad')
    plt.title('Distribución de Velocidades (Final)')
    plt.grid(True)
    plt.savefig("../results/histograma.png")
    print("Histograma guardado en ../results/histograma.png")

    # 4. Animación de Trayectorias
    print("Generando animación...")
    fig, ax = plt.subplots(figsize=(6, 6))
    ax.set_xlim(0, W)
    ax.set_ylim(0, H)
    ax.set_aspect('equal')
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.set_title("Simulación Gas Lennard-Jones")
    
    # Elementos gráficos
    particles, = ax.plot([], [], 'bo', markersize=8, markeredgecolor='k')
    time_text = ax.text(0.02, 0.95, '', transform=ax.transAxes)
    
    # Inicialización
    def init():
        particles.set_data([], [])
        time_text.set_text('')
        return particles, time_text

    # Función de actualización para la animación
    def animate(i):
        row = data.iloc[i]
        x_data = []
        y_data = []
        
        for p in range(N):
            x = row.iloc[1 + p*4]
            y = row.iloc[1 + p*4 + 1]
            x_data.append(x)
            y_data.append(y)
            
        particles.set_data(x_data, y_data)
        time_text.set_text(f't = {row["t"]:.2f}')
        return particles, time_text

    # Crear animación
    # Usamos saltos (slice) si hay demasiados datos para que la animación sea fluida
    step = max(1, len(data) // 500) 
    anim_data = data.iloc[::step]
    
    ani = animation.FuncAnimation(fig, animate, frames=len(anim_data), 
                                  init_func=init, interval=30, blit=True)
    
    # Mostrar o guardar
    print("Mostrando animación... (Cierra la ventana para terminar)")
    plt.show()
    
    # Opcional: Guardar como MP4 (requiere ffmpeg)
    # ani.save('../results/simulacion.mp4', writer='ffmpeg', fps=30)

if __name__ == "__main__":
    main()