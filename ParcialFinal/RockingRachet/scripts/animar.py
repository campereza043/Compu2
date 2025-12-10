import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import pandas as pd

# --- Parámetros del Potencial (DEBEN COINCIDIR CON main.cpp) ---
U0_pot = 1.0  # Amplitud del potencial
L_pot = 1.0   # Periodo espacial
alpha_pot = 0.8 # Asimetría del diente de sierra (0 < alpha < 1)

# --- Función del Potencial de Diente de Sierra (para graficar) ---
def potencial_diente_sierra(x, U0, L, alpha):
    # CORRECCIÓN DE PERIODICIDAD: Garantizar que xp esté en [0, L)
    xp = x - np.floor(x / L) * L
    
    x_peak = alpha * L
    
    # 2. Calcular U(x) por tramos
    if xp < x_peak:
        return U0 * (xp / x_peak)
    else:
        return U0 * (1.0 - (xp - x_peak) / (L - x_peak))

# --- Cargar datos de la trayectoria ---
try:
    # CORRECCIÓN DE SINTAXIS: Usar raw string r'\s+'
    data = pd.read_csv('results/trajectory.dat', sep=r'\s+', comment='#', header=None, names=['t', 'x', 'v'])
except FileNotFoundError:
    print("Error: No se encontró 'results/trajectory.dat'. Asegúrate de haber ejecutado la simulación.")
    exit()

times = data['t'].values
positions = data['x'].values
velocities = data['v'].values

# --- Configuración de la animación ---
fig, ax = plt.subplots(figsize=(10, 6))

# Rango de x para el potencial: dos periodos a cada lado
x_center = positions[0] 
x_min_plot = x_center - L_pot * 2
x_max_plot = x_center + L_pot * 2

x_pot = np.linspace(x_min_plot, x_max_plot, 500)
y_pot = [potencial_diente_sierra(val, U0_pot, L_pot, alpha_pot) for val in x_pot]

# Dibujar el potencial
line_pot, = ax.plot(x_pot, y_pot, 'b-', label='Potencial de Diente de Sierra')

# Dibujar la partícula (inicia vacío)
point, = ax.plot([], [], 'ro', markersize=8, label='Partícula')

# Configurar límites del gráfico
ax.set_xlim(x_min_plot, x_max_plot) 
ax.set_ylim(min(y_pot) - 0.2 * U0_pot, max(y_pot) + 0.2 * U0_pot) 

ax.set_xlabel("Posición (x)")
ax.set_ylabel("Energía Potencial U(x)")
ax.set_title("Animación del Motor Molecular con Diente de Sierra")
ax.grid(True)
ax.legend()

# --- Función de inicialización para la animación ---
def init():
    point.set_data([], [])
    return point,

# --- Función de actualización para cada fotograma ---
def animate(i):
    current_x = positions[i]
    current_y_pot = potencial_diente_sierra(current_x, U0_pot, L_pot, alpha_pot)
    
    # CORRECCIÓN DE MATPLOTLIB: set_data requiere secuencias (listas [])
    point.set_data([current_x], [current_y_pot]) 

    # Ajustar el eje X dinámicamente para seguir a la partícula
    x_center = current_x
    x_lim_min = x_center - L_pot * 2
    x_lim_max = x_center + L_pot * 2
    ax.set_xlim(x_lim_min, x_lim_max)

    # Actualizar la línea del potencial
    x_pot_updated = np.linspace(x_lim_min, x_lim_max, 500)
    y_pot_updated = [potencial_diente_sierra(val, U0_pot, L_pot, alpha_pot) for val in x_pot_updated]
    line_pot.set_data(x_pot_updated, y_pot_updated)

    ax.set_title(f"Tiempo: {times[i]:.4f} | Posición: {current_x:.4f}")

    return point, line_pot, 

# --- Crear y mostrar la animación ---
ani = animation.FuncAnimation(fig, animate, frames=len(positions), 
                              interval=10, blit=True, init_func=init)

plt.show()