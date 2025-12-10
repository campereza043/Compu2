import numpy as np
import matplotlib.pyplot as plt

# parámetros del potencial (puedes ajustarlos según tu simulación)
U0 = 1.0  # energía en unidades de tu simulación
L  = 1.0  # longitud periódica
m  = 1.0  # masa de la partícula

# leer datos
data = np.loadtxt("results/trajectory.dat")
t = data[:,0]
x = data[:,1]
v = data[:,2]

# energía potencial
V = U0 * (np.sin(2*np.pi*x/L) + 0.25 * np.sin(4*np.pi*x/L))

# energía cinética
K = 0.5 * m * v**2

# energía total
E = V + K

# graficar
plt.figure(figsize=(10,6))
plt.plot(t, V, label='Energía potencial V(x)')
plt.plot(t, K, label='Energía cinética K')
plt.plot(t, E, label='Energía total E', linestyle='--')
plt.xlabel('Tiempo')
plt.ylabel('Energía')
plt.title('Energías de la partícula en el motor molecular')
plt.legend()
plt.grid(True)
plt.show()
