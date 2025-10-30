#!/usr/bin/env python3
import numpy as np
import matplotlib.pyplot as plt
import sys
import os

RESULTS = 'results'
if len(sys.argv) > 1:
    DATAFILE = sys.argv[1]
else:
    DATAFILE = os.path.join(RESULTS, 'datos.txt')

if not os.path.exists(DATAFILE):
    raise SystemExit("Archivo no encontrado: " + DATAFILE)

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
plt.figure(figsize=(9,5))
plt.plot(t, x1, label='x1')
plt.plot(t, x2, label='x2')
plt.xlabel('t')
plt.ylabel('x')
plt.title('Osciladores de Van der Pol acoplados: x(t)')
plt.legend()
plt.tight_layout()
plt.savefig(os.path.join(RESULTS, 'x_vs_t.png'))
plt.close()

# Lissajous x1 vs x2
plt.figure(figsize=(6,6))
plt.plot(x1, x2, linewidth=0.6)
plt.xlabel('x1')
plt.ylabel('x2')
plt.title('Lissajous: x1 vs x2')
plt.tight_layout()
plt.savefig(os.path.join(RESULTS, 'lissajous_py.png'))
plt.close()

# Spectrum (x1)
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
plt.close()

# Poincaré (if exists)
poinfile = os.path.join(RESULTS, 'poincare.txt')
if os.path.exists(poinfile):
    pdat = np.loadtxt(poinfile)
    if pdat.ndim == 1 and pdat.size==5:
        pdat = pdat.reshape((1,5))
    if pdat.size>0:
        # columns: t x1 v1 x2 v2
        plt.figure(figsize=(6,6))
        plt.scatter(pdat[:,3], pdat[:,4], s=6)  # x2 vs v2 at section
        plt.xlabel('x2')
        plt.ylabel('v2')
        plt.title('Poincaré (x1=0, v1>0): x2 vs v2')
        plt.tight_layout()
        plt.savefig(os.path.join(RESULTS, 'poincare_x2_v2.png'))
        plt.close()

# Lyapunov progress (if exists)
lyap_prog = os.path.join(RESULTS, 'lyapunov_progress.txt')
lyap_final = os.path.join(RESULTS, 'lyapunov_final.txt')
if os.path.exists(lyap_prog):
    lp = np.loadtxt(lyap_prog)
    if lp.size>0:
        # ensure 2D
        if lp.ndim==1:
            lp=lp.reshape((1,2))
        plt.figure(figsize=(8,4))
        plt.plot(lp[:,0], lp[:,1])
        plt.xlabel('t')
        plt.ylabel('lambda_running')
        plt.title('Mayor exponente de Lyapunov (running)')
        plt.tight_layout()
        plt.savefig(os.path.join(RESULTS, 'lyapunov_running.png'))
        plt.close()
if os.path.exists(lyap_final):
    with open(lyap_final,'r') as f:
        lines = [L for L in f if not L.strip().startswith('#')]
    if len(lines)>0:
        val = float(lines[-1].strip())
        print("Lyapunov final (aprox):", val)

print("Figuras guardadas en carpeta:", RESULTS)
