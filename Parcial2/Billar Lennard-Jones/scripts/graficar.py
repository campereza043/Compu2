import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import os
import sys

# --- Configuración ---
FILENAME = "../results/trayectorias.dat"
# Valores por defecto si no se pasan argumentos
W_default, H_default = 20.0, 20.0 

# Leer argumentos de línea de comandos pasados por C++
if len(sys.argv) >= 3:
    try:
        W = float(sys.argv[1])
        H = float(sys.argv[2])
        print(f"Dimensiones recibidas de C++: W={W}, H={H}")
    except ValueError:
        print("Error al leer dimensiones, usando valores por defecto.")
        W, H = W_default, H_default
else:
    print("No se recibieron argumentos W, H. Usando valores por defecto.")
    W, H = W_default, H_default

EPSILON = 1.0      
SIGMA = 1.0        
MASS = 1.0         

def calcular_energia(df, N):
    pasos = len(df)
    K = np.zeros(pasos)
    U = np.zeros(pasos)
    
    print("Calculando energías...")
    
    for i in range(pasos):
        row = df.iloc[i]
        k_step = 0
        u_step = 0
        
        # En el .dat, la columna 0 es 't'. 
        # Las partículas empiezan en índice 1. Cada una tiene 4 columnas (x, y, vx, vy)
        
        # Arrays temporales
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
            
            k_step += 0.5 * MASS * (vx**2 + vy**2)
            
        for p1 in range(N):
            for p2 in range(p1 + 1, N):
                dx = xs[p2] - xs[p1]
                dy = ys[p2] - ys[p1]
                r2 = dx*dx + dy*dy
                
                if r2 > 0: 
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
        print("Asegúrate de ejecutar primero la simulación en C++.")
        return

    print("Cargando datos .dat...")
    # 'sep="\s+"' permite leer espacios o tabulaciones como delimitador
    data = pd.read_csv(FILENAME, sep="\s+")
    
    # Deducir número de partículas
    # Columnas: t, (x, y, vx, vy) * N -> Total = 1 + 4N
    N = (data.shape[1] - 1) // 4
    print(f"Detectadas {N} partículas.")

    # 2. Análisis de Energía
    K, U, E_total = calcular_energia(data, N)
    t = data['t']
    
    plt.figure(figsize=(10, 6))
    plt.plot(t, K, label='Cinética (K)', alpha=0.7)
    plt.plot(t, U, label='Potencial (U)', alpha=0.7)
    plt.plot(t, E_total, label='Total (E)', color='black', linewidth=2)
    plt.xlabel('Tiempo')
    plt.ylabel('Energía')
    plt.title(f'Energía (N={N}, W={W}, H={H})')
    plt.legend()
    plt.grid(True)
    plt.savefig("../results/energia.png")
    print("Gráfica de energía guardada.")

    # 3. Histograma
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
    plt.title('Distribución de Velocidades Final')
    plt.grid(True)
    plt.savefig("../results/histograma.png")
    print("Histograma guardado.")

    # 4. Animación
    print("Generando animación...")
    fig, ax = plt.subplots(figsize=(6, 6))
    ax.set_xlim(0, W)
    ax.set_ylim(0, H)
    ax.set_aspect('equal')
    ax.grid(True, linestyle='--', alpha=0.5)
    ax.set_title(f"Gas Lennard-Jones (N={N})")
    
    particles, = ax.plot([], [], 'bo', markersize=8, markeredgecolor='k')
    time_text = ax.text(0.02, 0.95, '', transform=ax.transAxes)
    
    def init():
        particles.set_data([], [])
        time_text.set_text('')
        return particles, time_text

    def animate(i):
        row = anim_data.iloc[i]
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

    # Reducir frames si hay demasiados datos
    step = max(1, len(data) // 400) 
    anim_data = data.iloc[::step]
    
    ani = animation.FuncAnimation(fig, animate, frames=len(anim_data), 
                                  init_func=init, interval=30, blit=True)
    
    print("Mostrando animación... (Cierra la ventana para terminar)")
    plt.show()

if __name__ == "__main__":
    main()