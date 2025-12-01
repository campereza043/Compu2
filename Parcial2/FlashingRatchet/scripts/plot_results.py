import pandas as pd
import matplotlib.pyplot as plt
import os

# --- CONFIGURACIÓN ---
# Definir rutas relativas
base_dir = os.path.dirname(os.path.abspath(__file__))
csv_path = os.path.join(base_dir, '../results/datos_ratchet.dat')
results_dir = os.path.join(base_dir, '../results')

# Cargar datos
try:
    data = pd.read_csv(csv_path, sep='\t')
except FileNotFoundError:
    print(f"Error: No se encuentra {csv_path}. Ejecuta la simulación primero.")
    exit()

# Filtrar datos para visualizar mejor (Zoom en los primeros 200s o 400s)
# Si quieres ver todo, comenta la siguiente línea o cambia el valor.
subset = data[data['t'] < 200]
states = subset['state'].values
times = subset['t'].values

# Configuración estética general
plt.rcParams.update({'font.size': 12})

# --- GRÁFICA 1: TRAYECTORIA (Transporte Dirigido) ---
plt.figure(figsize=(10, 5))
plt.plot(subset['t'], subset['x'], 'b-', lw=1.5, label='Partícula $x(t)$')
plt.title('Flashing Ratchet: Transporte Dirigido')
plt.ylabel('Posición $x$ (u.a.)')
plt.xlabel('Tiempo $t$ (u.a.)')
plt.grid(True, linestyle='--', alpha=0.6)

# Sombrear zonas OFF (Difusión libre)
ylim = plt.gca().get_ylim()
plt.fill_between(subset['t'], ylim[0], ylim[1], 
                 where=(subset['state'] == 0), 
                 color='gray', alpha=0.2, label='Fase OFF (Difusión)')

plt.legend(loc='upper left')
plt.tight_layout()
plt.savefig(os.path.join(results_dir, 'trayectoria.png'), dpi=300)
plt.close()
print("Generado: trayectoria.png")


# --- GRÁFICA 2: ENERGÍA (Conservación y Fluctuación) ---
plt.figure(figsize=(10, 5))
plt.plot(subset['t'], subset['E_total'], 'r-', lw=1.2, label='Energía Total')
plt.title('Evolución de la Energía Total')
plt.ylabel('Energía $E$')
plt.xlabel('Tiempo $t$')
plt.grid(True, linestyle='--', alpha=0.6)

# Sombrear zonas OFF
ylim = plt.gca().get_ylim()
plt.fill_between(subset['t'], ylim[0], ylim[1], 
                 where=(subset['state'] == 0), 
                 color='gray', alpha=0.2, label='Fase OFF')

plt.legend()
plt.tight_layout()
plt.savefig(os.path.join(results_dir, 'energia.png'), dpi=300)
plt.close()
print("Generado: energia.png")


# --- GRÁFICA 3: ESTADOS (Ciclo Químico / Motor) ---
plt.figure(figsize=(10, 3))
plt.plot(subset['t'], subset['state'], 'k-', lw=2)
plt.fill_between(subset['t'], subset['state'], color='orange', alpha=0.4, label='ON (Potencial Activo)')
plt.title('Ciclo de Trabajo del Motor (ON/OFF)')
plt.ylabel('Estado $s(t)$')
plt.xlabel('Tiempo $t$')
plt.yticks([0, 1], ['OFF (0)', 'ON (1)'])
plt.grid(True, axis='x', linestyle='--', alpha=0.6)
plt.legend(loc='upper right')
plt.tight_layout()
plt.savefig(os.path.join(results_dir, 'estados.png'), dpi=300)
plt.close()
print("Generado: estados.png")

# --- GRÁFICA 4: DESPLAZAMIENTO NETO (Trayectoria Completa) ---
plt.figure(figsize=(10, 5))

# Usamos 'data' directamente para ver todo el tiempo de simulación, no solo el 'subset'
plt.plot(data['t'], data['x'], color='darkcyan', lw=1.2, label='Trayectoria Global')

# Cálculo de la velocidad media (pendiente de la recta)
x_final = data['x'].iloc[-1]
x_inicial = data['x'].iloc[0]
t_final = data['t'].iloc[-1]
t_inicial = data['t'].iloc[0]
v_media = (x_final - x_inicial) / (t_final - t_inicial)

# Graficar línea de tendencia (velocidad media)
plt.plot([t_inicial, t_final], [x_inicial, x_final], 'r--', lw=1, 
         label=f'Velocidad Media: {v_media:.4f}')

plt.title(f'Desplazamiento Neto Acumulado (Simulación Completa)')
plt.ylabel('Desplazamiento $x$ (u.a.)')
plt.xlabel('Tiempo $t$ (u.a.)')
plt.grid(True, linestyle='-', alpha=0.3)
plt.legend(loc='upper left')

plt.tight_layout()
plt.savefig(os.path.join(results_dir, 'desplazamiento_neto.png'), dpi=300)
plt.close()
print("Generado: desplazamiento_neto.png")