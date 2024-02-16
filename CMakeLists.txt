cmake_minimum_required(VERSION 3.13)
set(PICO_BOARD pico)
include(pico_sdk_import.cmake)
project(messung_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()
add_executable(messung
messung.c
)

add_subdirectory(./st7735)

target_link_libraries(messung
		pico_stdlib
		hardware_i2c
		st7735)
