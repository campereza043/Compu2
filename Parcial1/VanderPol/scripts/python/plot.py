#!/usr/bin/env python3
import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt('results/datos.txt')
if data.size == 0:
    raise SystemExit('Archivo vacío')

t = data[:,0]
x1 = data[:,1]
v1 = data[:,2]
x2 = data[:,3]
v2 = data[:,4]

plt.figure()
plt.plot(t, x1)
plt.plot(t, x2)
plt.legend(['x1', 'x2'])
plt.xlabel('t')
plt.ylabel('x')
plt.title('Osciladores de Van der Pol acoplados')
plt.savefig('results/x_vs_t.png')

plt.figure()
plt.plot(x1, x2)
plt.xlabel('x1')
plt.ylabel('x2')
plt.title('Lissajous')
plt.savefig('results/lissajous_py.png')

plt.show()
