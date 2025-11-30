cmake_minimum_required(VERSION 3.10)
project(FlashingRatchet)

set(CMAKE_CXX_STANDARD 14)

# Incluir cabeceras
include_directories(include)

# Buscar archivos fuente
file(GLOB SOURCES "src/*.cpp")

# Crear ejecutable
add_executable(simulacion ${SOURCES})