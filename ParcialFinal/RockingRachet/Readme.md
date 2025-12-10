# ⚙️ Simulación del Motor Molecular Ratchet (Asimétrico)

Este proyecto simula la dinámica de una partícula en un potencial asimétrico de diente de sierra (ratchet) impulsada por una fuerza externa periódica (*driving*).

El objetivo principal es demostrar el fenómeno de **rectificación del movimiento**, donde la asimetría del potencial convierte una fuerza oscilante simétrica en un movimiento direccional neto.

---

## 🔬 Descripción del Modelo

El sistema se rige por la ecuación de Newton:
$$m \frac{d^2 x}{dt^2} = F_{\text{pot}}(x) + F_{\text{driving}}(t)$$

### Potencial Asimétrico

Se utiliza un potencial periódico de **diente de sierra** $U(x)$ definido por tramos, con asimetría $\alpha$. La fuerza potencial es $F_{\text{pot}}(x) = -dU/dx$.

### Integración

Las ecuaciones de movimiento se integran numéricamente utilizando el método de **Velocity Verlet** debido a su estabilidad y capacidad para conservar la energía del sistema.

---

## 🛠️ Requisitos y Dependencias

Para compilar y ejecutar el proyecto:

### C++ (Simulación)
* **Compilador:** `g++` (con soporte C++17)
* **Herramienta de compilación:** `make`

### Python (Análisis y Visualización)
* **Intérprete:** Python 3.x
* **Librerías:** `numpy`, `matplotlib`, `pandas`.

Puedes instalar las dependencias de Python usando `pip`:
```bash
pip install numpy matplotlib pandas