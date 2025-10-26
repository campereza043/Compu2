cmake_minimum_required(VERSION 3.10)
project(VanDerPolCoupled CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories(include)
file(GLOB SOURCES "src/*.cpp")
add_executable(vanderpol ${SOURCES})


# install target (opcional)
install(TARGETS vanderpol DESTINATION bin)
