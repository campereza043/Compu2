cmake_minimum_required(VERSION 3.10)
project(Billar)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

# Esta es una forma más limpia de agregar los fuentes
add_executable(simulacion
    main.cpp
    src/Bola.cpp
    src/Caja.cpp
    src/Sistema.cpp
)
