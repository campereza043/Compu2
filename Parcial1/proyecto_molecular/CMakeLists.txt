cmake_minimum_required(VERSION 3.10)
project(SimulacionMolecular)

# Configuración C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configuración de compilación
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Wextra")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native")
endif()

# Ejecutable principal
add_executable(simulador 
    main.cpp 
    src/Caja.cpp 
    src/Bola.cpp 
    src/Sistema.cpp
)

# Directorios de inclusión
target_include_directories(simulador PRIVATE include)

# Configuración Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in 
                   ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generando documentación API con Doxygen"
        VERBATIM
    )
endif()

# Copiar scripts al directorio de build
add_custom_target(copiar_scripts ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/scripts
        ${CMAKE_CURRENT_BINARY_DIR}/scripts
    COMMENT "Copiando scripts de visualización"
)

# Crear directorio de resultados
add_custom_target(crear_directorios ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory results
    COMMENT "Creando directorio de resultados"
)
