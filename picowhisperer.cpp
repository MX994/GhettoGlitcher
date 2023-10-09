#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"

#include "serial_console.hpp"
#include "picowhisperer.hpp"

int main() {
    stdio_init_all();
    cyw43_arch_init();
    multicore_launch_core1(serial_console);
    while (true) {
        while (multicore_fifo_rvalid()) {
            uint32_t command = multicore_fifo_pop_blocking();
            switch (command) {
                case CMD_PING:
                    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
                    break;
            }
        }
    }
    return 0;
}