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

# Configuración para documentación
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile 
                   ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generando documentación Doxygen"
    )
endif()
