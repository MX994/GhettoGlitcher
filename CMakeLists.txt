cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(picowhisperer C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_BOARD pico_w)

pico_sdk_init()

add_executable(ghettoglitcher
    ghettoglitcher.cpp
    serial_console.cpp
    serial_trigger.cpp
)

pico_add_extra_outputs(ghettoglitcher)

target_link_libraries(ghettoglitcher 
    pico_stdlib 
    pico_cyw43_arch_none
    hardware_pwm 
    hardware_pio
    pico_multicore
)

pico_enable_stdio_usb(ghettoglitcher 1)
# pico_enable_stdio_uart(picowhisperer 1)