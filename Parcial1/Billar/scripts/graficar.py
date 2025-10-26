import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import os
import pandas as pd

# --- Cargar datos y Parámetros ---
data_path = "../results/trayectorias.dat"
if not os.path.exists(data_path):
    raise FileNotFoundError(f"No se encontró {data_path}. Ejecuta primero la simulación.")

# Leer W, H y R_BOLA del archivo de datos
W, H, R_BOLA = None, None, None
with open(data_path, 'r') as f:
    for line in f:
        if line.startswith('# W:'):
            W = float(line.split(':')[1].strip())
        elif line.startswith('# H:'):
            H = float(line.split(':')[1].strip())
        elif line.startswith('# R_BOLA:'):
            R_BOLA = float(line.split(':')[1].strip())
        # Una vez que encontramos los 3, podemos parar si queremos optimizar
        if W is not None and H is not None and R_BOLA is not None:
            break

if W is None or H is None or R_BOLA is None:
    raise ValueError("No se pudieron leer W, H o R_BOLA del archivo de datos. Asegúrate de que C++ los escriba.")

print(f"Dimensiones de la caja cargadas: W={W}, H={H}, R={R_BOLA}")

# Ahora se carga el resto de los datos (ignorando las líneas con #)
data = pd.read_csv(data_path, delim_whitespace=True, comment='#').values
t = data[:, 0]
# Ahora hay 4 columnas por partícula (x, y, vx, vy)
N_particles = (data.shape[1] - 1) // 4

# --- Configurar figura ---
fig, ax = plt.subplots()
# **W y H ya no son constantes fijas, sino valores leídos:**
ax.set_xlim(0, W)
ax.set_ylim(0, H)
ax.set_aspect('equal')
# ... resto del código de graficar.py ...
