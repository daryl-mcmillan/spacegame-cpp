cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_FETCH_FROM_GIT on)
include(pico_sdk_import.cmake)

project(spacegame)

pico_sdk_init()

add_library(matrix matrix.cc matrix.h)

add_executable(spacegame main.cc display.cc display.h controls.cc controls.h game/planet.cc game/planet.h game/spaceship.h game/spaceship.cc game/stats.h game/stats.cc)
pico_add_extra_outputs(spacegame)
target_link_libraries(spacegame matrix pico_stdlib pico_multicore hardware_spi hardware_dma)
