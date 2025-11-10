Simulación de Billar 2D

Descripción:
Simulación de un sistema de bolas en movimiento con colisiones elásticas en 2D.

Estructura del proyecto:
Billar/
├── src/           # Código fuente (.h y .cpp)
├── scripts/       # Scripts auxiliares
├── graficar.py    # Visualización de resultados
├── CMakeLists.txt # Configuración CMake
├── Doxyfile       # Configuración documentación
├── main.cpp       # Programa principal
└── README.md      # Este archivo

Compilación:
mkdir build && cd build
cmake ..
make

Ejecución:
./simulación

Características principales:
- Integradores: Euler y Verlet
- Colisiones elásticas entre bolas
- Colisiones con paredes
- Configuración inicial en rejilla
- Salida de datos para visualización

Visualización de resultados:
python graficar.py datos_salida.txt

Generación de documentación:
doxygen Doxyfile

Notas:
- Proyecto educativo para simulación de sistemas físicos
- Requiere compilador C++11 y CMake
- Los parámetros se modifican en main.cpp