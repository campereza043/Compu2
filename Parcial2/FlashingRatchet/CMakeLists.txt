cmake_minimum_required(VERSION 3.10)
project(FlashingRatchet)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(simulacion ${SOURCES})

# Crear carpeta results
add_custom_command(TARGET simulacion PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/results
)

# Target pipeline: Ejecuta la simulación con parámetros por defecto
# Parámetros: T_on=50 T_off=20 Time=1000 Gamma=0.5 kBT=0.8
add_custom_target(run_pipeline
    COMMAND ./simulacion 50.0 20.0 1000.0 0.5 0.8
    COMMAND python3 ../scripts/plot_results.py
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS simulacion
    COMMENT "Ejecutando simulacion automatica (Batch Mode) y generando graficas..."
)