#include "ghettoglitcher.hpp"

int main() {
    SerialTrigger Trigger;
    stdio_init_all();
    cyw43_arch_init();
    multicore_launch_core1(serial_console);
    while (true) {
        while (multicore_fifo_rvalid()) {
            uint32_t command = multicore_fifo_pop_blocking();
            switch (command) {
                case CMD_PING:
                    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
                    printf("PONG!");
                    break;
                case CMD_SET_PATTERN:
                    uint32_t pattern_length = multicore_fifo_pop_blocking();
                    uint8_t pattern_buffer[pattern_length];
                    memset(pattern_buffer, 0, pattern_length);
                    for (int i = 0; i < pattern_length; ++i) {
                        pattern_buffer[i] = (char)multicore_fifo_pop_blocking();
                    }
                    Trigger.SetPattern(pattern_buffer, pattern_length);
                    printf((char *)pattern_buffer);
                    break;
            }
        }
    }
    return 0;
}