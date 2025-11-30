cmake_minimum_required(VERSION 3.10)
project(Billar)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- Directorios de inclusión ---
include_directories(include)

# --- Archivos fuente ---
set(SOURCES
    main.cpp
    src/Bola.cpp
    src/Caja.cpp
    src/Sistema.cpp
)

# --- Ejecutable principal ---
add_executable(simulacion ${SOURCES})

# --- Directorios útiles ---
set(RESULTS_DIR "${CMAKE_SOURCE_DIR}/results")
set(DOCS_DIR "${CMAKE_SOURCE_DIR}/documents")

# ==========================================================
#              CONFIGURACIÓN DE DOXYGEN
# ==========================================================
find_package(Doxygen REQUIRED)

set(DOXYFILE_IN ${CMAKE_SOURCE_DIR}/Doxyfile)
set(DOXYGEN_OUTPUT_DIR ${DOCS_DIR})

# --- Generar documentación HTML + LaTeX ---
add_custom_command(
    OUTPUT ${DOCS_DIR}/html/index.html
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_IN}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generando documentación con Doxygen (HTML y LaTeX)..."
    VERBATIM
)

# --- Compilar PDF desde el LaTeX de Doxygen ---
add_custom_command(
    OUTPUT ${DOCS_DIR}/latex/refman.pdf
    DEPENDS ${DOCS_DIR}/html/index.html
    COMMAND ${CMAKE_COMMAND} -E chdir ${DOCS_DIR}/latex make
    COMMENT "Compilando documentación LaTeX a PDF..."
)

add_custom_target(documents ALL
    DEPENDS ${DOCS_DIR}/html/index.html ${DOCS_DIR}/latex/refman.pdf
    COMMENT "Documentación HTML y PDF generadas correctamente."
)

# ==========================================================
#                LIMPIEZA DE RESULTADOS
# ==========================================================
add_custom_target(clean_all
    COMMAND ${CMAKE_COMMAND} -E echo "Eliminando resultados y ejecutable..."
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/simulacion
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${RESULTS_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${DOCS_DIR}
    COMMENT "Limpieza completa realizada."
)
