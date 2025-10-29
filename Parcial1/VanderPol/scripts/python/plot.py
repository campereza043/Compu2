#!/usr/bin/env python3
import numpy as np
import matplotlib.pyplot as plt
import sys
import os

RESULTS = 'results'
DATAFILE = os.path.join(RESULTS, 'datos.txt')

if len(sys.argv) > 1:
    DATAFILE = sys.argv[1]

data = np.loadtxt(DATAFILE)
if data.size == 0:
    raise SystemExit('Archivo vacío: ' + DATAFILE)

t = data[:,0]
x1 = data[:,1]
v1 = data[:,2]
x2 = data[:,3]
v2 = data[:,4]

os.makedirs(RESULTS, exist_ok=True)

# x vs t
plt.figure(figsize=(8,5))
plt.plot(t, x1, label='x1')
plt.plot(t, x2, label='x2')
plt.xlabel('t')
plt.ylabel('x')
plt.title('Osciladores de Van der Pol acoplados: x(t)')
plt.legend()
plt.tight_layout()
plt.savefig(os.path.join(RESULTS, 'x_vs_t.png'))

# Lissajous x1 vs x2
plt.figure(figsize=(6,6))
plt.plot(x1, x2, linewidth=0.6)
plt.xlabel('x1')
plt.ylabel('x2')
plt.title('Lissajous: x1 vs x2')
plt.tight_layout()
plt.savefig(os.path.join(RESULTS, 'lissajous_py.png'))

# Optional: simple Fourier amplitude for x1 (to explore frecuencias)
from numpy.fft import rfft, rfftfreq
dt = t[1]-t[0]
yf = rfft(x1 - np.mean(x1))
xf = rfftfreq(len(t), dt)
plt.figure(figsize=(8,4))
plt.semilogy(xf, np.abs(yf))
plt.xlabel('freq [Hz]')
plt.ylabel('Amplitude')
plt.title('Espectro (x1)')
plt.tight_layout()
plt.savefig(os.path.join(RESULTS, 'spectrum_x1.png'))

print("Figuras guardadas en carpeta:", RESULTS)
# plt.show()  # opcional
