# Versión mínima de CMake requerida
cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(SimulacionParticulas CXX)

# Estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Directorio de los archivos de encabezado
include_directories(include)

# Encontrar todos los archivos fuente en el directorio src
file(GLOB SOURCES "src/*.cpp")

# Crear el ejecutable
add_executable(simulador ${SOURCES})

# Mensaje para el usuario
message(STATUS "Proyecto configurado. Compila con 'make' en la carpeta 'build'.")
