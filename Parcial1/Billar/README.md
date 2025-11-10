# Autores y propósito

Proyecto desarrollado en C++ como parte de un estudio de simulación física de partículas confinadas,
utilizando un modelo de colisiones elásticas y análisis estadístico de configuraciones.

Autor: Camila Pérez y Sergio Carrasquilla
Lenguaje: C++17
Documentación: Doxygen

# Simulación de Pepas en una Caja

Este proyecto simula el movimiento de pequeñas esferas (“pepas”) dentro de una caja.  
El código está modularizado en clases (`Bola`, `Caja`, `Sistema`) y permite observar la evolución temporal del sistema, guardar resultados y generar documentación automática con Doxygen.

---

## Estructura del proyecto

Billar/
├── CMakeLists.txt
├── Doxyfile
├── README.md
├── include/
│ ├── Bola.h
│ ├── Caja.h
│ └── Sistema.h
├── src/
│ ├── Bola.cpp
│ ├── Caja.cpp
│ └── Sistema.cpp
├── results/
│ ├── datos/
│ ├── graficas/
│ └── videos/
├── scripts/
│ ├── python/
│ │ └── plot.py
│ └── utils/
├── documents/
│ └── (Aquí se generan los archivos de documentación: HTML, LaTeX y PDF)
└── main.cpp


---

##  Compilación del proyecto

Para compilar el programa principal utilizando **CMake**, ejecuta:

```bash
cmake -S . -B build
cmake --build build

Esto generará el ejecutable simulacion dentro de la carpeta build/.

Para ejecutarlo:

./build/simulacion

---

## Generación de documentación (Doxygen)

Este proyecto usa Doxygen para generar la documentación tanto en HTML como en LaTeX (PDF).

1. Asegúrate de tener instalados:

sudo apt install doxygen doxygen-latex

2. Genera la documentación:

cmake --build build --target documents

Esto creará la documentación en la carpeta:

documents/html/
documents/latex/

3. Compila el PDF (automático en la regla make documents):
El archivo resultante será:

documents/latex/refman.pdf

Limpieza

Puedes limpiar los archivos generados con los siguientes comandos:

cmake --build build --target clean

Si deseas eliminar también los resultados y la documentación generada:

rm -rf build/ results/* documents/*


