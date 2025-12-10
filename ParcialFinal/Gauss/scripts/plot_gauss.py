import numpy as np
import matplotlib.pyplot as plt
import math

# -------------------------------
# Cargar datos
# -------------------------------
data = np.loadtxt("results/gauss.dat")

# -------------------------------
# Parámetros teóricos
# -------------------------------
sigma = 1.0 / math.sqrt(2.0)
mu = 0.0

def gauss(x):
    return (1.0 / (math.sqrt(2*math.pi)*sigma)) * np.exp(-(x - mu)**2 / (2*sigma**2))

# -------------------------------
# Histograma normalizado
# -------------------------------
plt.figure(figsize=(8,5))
count, bins, _ = plt.hist(data, bins=100, density=True, alpha=0.6, label="Histograma")

# -------------------------------
# Curva teórica
# -------------------------------
x = np.linspace(bins[0], bins[-1], 400)
plt.plot(x, gauss(x), linewidth=2, label="Gauss teórica")

# -------------------------------
# Estética
# -------------------------------
plt.title("Histograma y curva teórica (σ = 1/√2)")
plt.xlabel("x")
plt.ylabel("Densidad de probabilidad")
plt.legend()
plt.grid(alpha=0.3)

# Guardar
plt.savefig("results/hist_gauss_python.png", dpi=200)
print("Figura guardada en results/hist_gauss_python.png")

