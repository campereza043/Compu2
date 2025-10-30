# Sistema de osciladores de Van der Pol acoplados

Este proyecto implementa dos osciladores de Van der Pol acoplados linealmente mediante una constante de acoplamiento `k`.
La integración numérica se realiza usando el método de Runge–Kutta de cuarto orden (RK4).
El código también genera automáticamente gráficos y animaciones que se guardan en la carpeta `results/`.

---

## Organización del proyecto

```
VanderPol/
├── build/
├── documents/
│   └── Exam_1_vander.pdf
│
├── include/
│   ├── Sistema.h
│   └── VanDerPol.h
│
├── results/
│   ├── datos.txt
│   ├── lissajous_py.png
│   └── x_vs_t.png
│
├── scripts/
│   ├── gnuplot/
│       └── plot.gp
│   └── python/
│       └── plot.py
│
├── src/
│   ├── main.cpp
│   ├── Sistema.cpp
│   └── VanDerPol.cpp
│
├── CMakeLists.txt
├── Doxyfile
└── README.md

```

---

## Compilación

El proyecto usa **CMake** para compilar.

Desde la raíz del proyecto (`VanderPol/`):

```bash
mkdir -p build
cd build
cmake ..
make
```

Esto generará un ejecutable dentro de `build/`, por ejemplo:

```
build/vanderpol
```

---

## Ejecución

Ejecutar el programa desde la carpeta `build/`:

```bash
./vanderpol
```

Durante la ejecución, el programa:

1. Integra las ecuaciones del sistema de Van der Pol acoplado.
2. Guarda los resultados numéricos en `results/datos.txt`.
3. Llama automáticamente a los scripts de **Python** y **Gnuplot** para generar las figuras y animaciones.

Al finalizar, verás mensajes indicando la ruta de los archivos generados.

---

## Archivos generados

Después de ejecutar el programa, en la carpeta `results/` se encontrarán:

```
results/
├── datos.txt         → Datos de simulación (t, x1, v1, x2, v2)
├── x_vs_t.png        → Posiciones x1 y x2 en función del tiempo
└── lissajous_py.png  → Trayectorias en el espacio de fase (x1 vs x2)
```

Estos archivos se crean automáticamente mediante los scripts en `scripts/python/plot.py` y `scripts/gnuplot/plot.gp`.

---

## Cómo abrir las figuras y resultados

Puedes abrir los resultados de distintas formas:

**1. Desde la terminal (por comando):**

```bash
xdg-open ../results/x_vs_t.png
xdg-open ../results/lissajous_py.png
```

**2. Explorando la carpeta manualmente:**

Abre el directorio `results/` desde tu explorador de archivos y haz doble clic sobre las imágenes o el archivo `datos.txt`.

---

## Scripts de generación

* `scripts/python/plot.py`: genera gráficos en formato PNG y opcionalmente animaciones.
* `scripts/gnuplot/plot.gp`: alternativa basada en Gnuplot para generar las mismas figuras.

Ambos scripts son llamados automáticamente al finalizar la simulación, pero pueden ejecutarse manualmente si se desea actualizar las figuras sin volver a simular.

Ejemplo manual:

```bash
python3 ../scripts/python/plot.py
gnuplot ../scripts/gnuplot/plot.gp
```

---

## Parámetros modificables

Los parámetros principales se configuran en `src/main.cpp`:

* `mu1`, `mu2`: parámetros de no linealidad.
* `w01`, `w02`: frecuencias naturales.
* `k`: constante de acoplamiento.
* `t0`, `tf`, `dt`: intervalo y paso temporal.
* `x10`, `v10`, `x20`, `v20`: condiciones iniciales.

---

## Resultados

El archivo `results/datos.txt` contiene las siguientes columnas:

```
# t   x1   v1   x2   v2
```

Este formato facilita la visualización o análisis posterior en Python, Gnuplot o cualquier otra herramienta científica.

---

## Documentación

La documentación del código puede generarse con **Doxygen**:

```bash
doxygen Doxyfile
```

Esto creará la documentación en formato HTML en el directorio especificado por el Doxyfile.

---

## Licencia

Este código puede utilizarse libremente con fines académicos o de investigación, citando al autor original del proyecto.
