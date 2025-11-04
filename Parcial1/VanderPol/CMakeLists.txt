cmake_minimum_required(VERSION 3.10)
project(VanDerPolCoupled CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configuración de compilación
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3 -march=native")

# Incluir directorios
include_directories(include)

# Archivos fuente
file(GLOB SOURCES "src/*.cpp")

# Ejecutable principal
add_executable(vanderpol ${SOURCES})

# Opciones de instalación
install(TARGETS vanderpol DESTINATION bin)

# --- Configuración para documentación (make documents) ---
find_package(Doxygen)
if(DOXYGEN_FOUND)
    
    # 1. Definimos el directorio de salida (documents/ en la raíz del proyecto)
    set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/documents)

    # 2. Configuramos el Doxyfile (la copia se crea en la carpeta build/)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile 
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile 
        @ONLY
    )

    # Directorio donde Doxygen genera los archivos LaTeX
    set(LATEX_DIR ${DOXYGEN_OUTPUT_DIR}/latex)

    # 3. Creamos el objetivo "documents" (para ejecutar "make documents")
    add_custom_target(documents
        # Comando 1: Ejecutar Doxygen para generar HTML y LaTeX
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        
        # Comando 2: Compilar el PDF desde los archivos LaTeX generados
        COMMAND bash -c "if [ -d \"${LATEX_DIR}\" ]; then \
                           echo \"Compilando PDF en ${LATEX_DIR}...\"; \
                           cd \"${LATEX_DIR}\" && make; \
                         else \
                           echo \"Directorio LaTeX no encontrado.\"; \
                         fi"
        
        # Se ejecuta desde la raíz del proyecto
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generando documentación Doxygen (HTML y PDF) en la carpeta /documents"
    )
endif()

# --- OBJETIVO LIMPIEZA TOTAL (make clean_all) ---
# Este es el comando único que realiza la limpieza total.
add_custom_target(clean_all
    # 1. Ejecuta el 'make clean' estándar (limpia archivos de compilación dentro de build/)
    COMMAND ${CMAKE_COMMAND} -E chdir "${CMAKE_CURRENT_BINARY_DIR}" make clean
    
    # 2. Borra la carpeta de resultados (results) por completo
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_CURRENT_SOURCE_DIR}/results"
    
    # 3. Borra la carpeta de documentos (documents) por completo
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_CURRENT_SOURCE_DIR}/documents"
    
    COMMENT "Limpieza total (make clean_all): Borrado contenido de build/, results/ y documents/."
    VERBATIM
)
