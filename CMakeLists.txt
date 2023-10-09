cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(test_project C CXX ASM)
set(CMAKE_C_STANDARD 11)

set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(picowhisperer
    picowhisperer.cpp
)

pico_enable_stdio_uart(picowhisperer 1)

pico_add_extra_outputs(picowhisperer)

target_link_libraries(picowhisperer pico_stdlib)