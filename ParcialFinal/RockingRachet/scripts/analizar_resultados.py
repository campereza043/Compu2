import numpy as np
import matplotlib.pyplot as plt
import sys

# Recibe el archivo desde la línea de comandos
filename = sys.argv[1]

data = np.loadtxt(filename, comments="#")
t = data[:, 0]
x = data[:, 1]
v = data[:, 2]

plt.figure()
plt.plot(t, x)
plt.xlabel("t")
plt.ylabel("x(t)")
plt.title("Trayectoria")

plt.figure()
plt.plot(t, v)
plt.xlabel("t")
plt.ylabel("v(t)")
plt.title("Velocidad")

plt.show()
