cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto actualizado para el Examen 2
project(GasLennardJones)

# Estándar C++17 requerido (necesario para filesystem y otras features modernas)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- Flags de Compilación ---
# Se agregan optimizaciones agresivas (-O3) para mejorar el rendimiento de la simulación numérica.
if(MSVC)
    add_compile_options(/O2 /W3)
else()
    add_compile_options(-O3 -march=native -Wall -Wextra)
endif()

# --- Directorios de inclusión ---
# Las cabeceras (.h) deben estar en la carpeta 'include'
include_directories(include)

# --- Archivos fuente ---
# Se asume la estructura: main.cpp en raíz, implementaciones en src/
set(SOURCES
    main.cpp
    src/Bola.cpp
    src/Caja.cpp
    src/Sistema.cpp
)

# --- Ejecutable principal ---
add_executable(simulacion ${SOURCES})

# --- Definición de directorios de salida ---
set(RESULTS_DIR "${CMAKE_SOURCE_DIR}/results")
set(DOCS_DIR "${CMAKE_SOURCE_DIR}/documents")

# ==========================================================
#              CONFIGURACIÓN DE DOXYGEN
# ==========================================================
find_package(Doxygen)

if(DOXYGEN_FOUND)
    set(DOXYFILE_IN ${CMAKE_SOURCE_DIR}/Doxyfile)
    
    # Objetivo personalizado para generar documentación
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_IN}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generando documentación con Doxygen..."
        VERBATIM
    )
else()
    message(WARNING "Doxygen no encontrado. El target 'docs' no estará disponible.")
endif()

# ==========================================================
#                LIMPIEZA DE RESULTADOS
# ==========================================================
# Este target borra el ejecutable, los resultados y la documentación generada
add_custom_target(clean_all
    COMMAND ${CMAKE_COMMAND} -E echo "Limpiando todo..."
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/simulacion
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${RESULTS_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${DOCS_DIR}
    # Recrear la carpeta results vacía para evitar errores en ejecución
    COMMAND ${CMAKE_COMMAND} -E make_directory ${RESULTS_DIR}
    COMMENT "Limpieza completada: binarios y datos eliminados."
)