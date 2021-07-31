cmake_minimum_required(VERSION 3.13)

project(spacegame)

add_library(matrix matrix.cc matrix.h)
add_executable(spacegame main.cc)
target_link_libraries(spacegame matrix)
