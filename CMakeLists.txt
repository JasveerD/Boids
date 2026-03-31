cmake_minimum_required(VERSION 3.20)
project(boids CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(raylib REQUIRED)
add_executable(boids main.cpp)
target_link_libraries(boids PRIVATE raylib)
