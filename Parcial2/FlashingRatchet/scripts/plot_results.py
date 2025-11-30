import pandas as pd
import matplotlib.pyplot as plt
import os

# Ajustar ruta para leer desde la carpeta results
csv_path = os.path.join(os.path.dirname(__file__), '../results/datos_ratchet.csv')
data = pd.read_csv(csv_path)

fig, axs = plt.subplots(3, 1, figsize=(10, 12), sharex=True)
plt.subplots_adjust(hspace=0.1)

# Zoom para ver detalles
subset = data[data['t'] < 200]
states = subset['state'].values
times = subset['t'].values

# 1. Trayectoria
axs[0].set_ylabel('Posición $x(t)$')
axs[0].set_title('Flashing Ratchet: Transporte Dirigido')
axs[0].plot(subset['t'], subset['x'], 'b-', lw=1.5, label='Partícula')

# Sombrear zonas OFF
for i in range(len(states)-1):
    if states[i] == 0:
        axs[0].axvspan(times[i], times[i+1], color='gray', alpha=0.2, lw=0)

# 2. Energía
axs[1].set_ylabel('Energía Total')
axs[1].plot(subset['t'], subset['E_total'], 'r-', lw=1)
for i in range(len(states)-1):
    if states[i] == 0:
        axs[1].axvspan(times[i], times[i+1], color='gray', alpha=0.2, lw=0)

# 3. Ciclos ON/OFF
axs[2].set_ylabel('Estado Potencial')
axs[2].set_xlabel('Tiempo')
axs[2].plot(subset['t'], subset['state'], 'k-', lw=2)
axs[2].fill_between(subset['t'], subset['state'], color='orange', alpha=0.3)
axs[2].set_yticks([0, 1])
axs[2].set_yticklabels(['OFF', 'ON'])

# Guardar en results
output_path = os.path.join(os.path.dirname(__file__), '../results/graficas_ratchet.png')
plt.savefig(output_path, dpi=300)
print(f"Gráfica guardada en: {output_path}")