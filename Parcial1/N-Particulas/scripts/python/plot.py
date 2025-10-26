import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import pandas as pd
import sys

def animate_simulation(data_file='../../results/datos.txt'):
    """
    Lee los datos de simulación y crea una animación.
    """
    try:
        # Usar pandas para leer fácilmente el archivo con formato de ancho fijo
        data = pd.read_fwf(data_file)
    except FileNotFoundError:
        print(f"Error: No se encontró el archivo '{data_file}'")
        sys.exit(1)
        
    time_steps = data['t'].values
    particle_cols = [col for col in data.columns if col != 't']
    num_particles = len(particle_cols) // 4
    
    # Extraer posiciones
    positions = np.zeros((len(time_steps), num_particles, 2))
    for i in range(num_particles):
        positions[:, i, 0] = data[f'x{i}'].values
        positions[:, i, 1] = data[f'y{i}'].values

    # Configuración de la figura
    fig, ax = plt.subplots()
    ax.set_xlim(0, 100) # Dimension W de la caja
    ax.set_ylim(0, 100) # Dimension H de la caja
    ax.set_aspect('equal', 'box')
    ax.set_title("Simulación de Partículas")
    
    # Crear los objetos de las partículas (círculos)
    # Usamos colores para distinguir las partículas
    colors = plt.cm.jet(np.linspace(0, 1, num_particles))
    particles = [ax.add_patch(plt.Circle((0, 0), radius=2.0, color=colors[i])) for i in range(num_particles)]
    time_text = ax.text(0.05, 0.95, '', transform=ax.transAxes, verticalalignment='top')

    def init():
        for p in particles:
            p.center = (0, 0)
        time_text.set_text('')
        return particles + [time_text]

    def update(frame):
        for i, p in enumerate(particles):
            p.center = (positions[frame, i, 0], positions[frame, i, 1])
        time_text.set_text(f'Tiempo: {time_steps[frame]:.2f} s')
        return particles + [time_text]

    # Crear la animación
    ani = FuncAnimation(fig, update, frames=len(time_steps),
                        init_func=init, blit=True, interval=20)

    # Guardar la animación
    output_filename = '../animacion.mp4'
    ani.save(output_filename, writer='ffmpeg', fps=30)
    print(f"Animación guardada en 'scripts/{output_filename}'")
    plt.show()

if __name__ == '__main__':
    animate_simulation()
