#!/usr/bin/env python3
import numpy as np
import matplotlib.pyplot as plt

# Datos
traj_file = "results/motores_x1x2.dat"
energy_file = "results/motores_energia.dat"

# cargar trayectoria
data = np.loadtxt(traj_file, comments="#")
t = data[:,0]
x1 = data[:,1]
v1 = data[:,2]
x2 = data[:,3]
v2 = data[:,4]

# cargar energías
e = np.loadtxt(energy_file, comments="#")
te = e[:,0]
K1 = e[:,1]; K2 = e[:,2]
U1 = e[:,3]; U2 = e[:,4]; Uc = e[:,5]
E = e[:,6]

plt.figure(figsize=(8,4))
plt.plot(t, x1, label='x1 (Flashing)')
plt.plot(t, x2, label='x2 (Rocking)')
plt.xlabel('t'); plt.ylabel('x')
plt.legend()
plt.tight_layout()
plt.savefig('results/x1_x2.png', dpi=200)

plt.figure(figsize=(8,4))
plt.plot(t, x2-x1, label='x2 - x1')
plt.xlabel('t'); plt.ylabel('elongacion')
plt.legend()
plt.tight_layout()
plt.savefig('results/elongacion.png', dpi=200)

plt.figure(figsize=(8,4))
plt.plot(te, K1, label='K1'); plt.plot(te, K2, label='K2')
plt.plot(te, U1+U2+Uc, label='U_total')
plt.plot(te, E, label='E_total', linestyle='--')
plt.xlabel('t'); plt.ylabel('E')
plt.legend()
plt.tight_layout()
plt.savefig('results/energias.png', dpi=200)

print("Figuras guardadas en results/: x1_x2.png, elongacion.png, energias.png")
