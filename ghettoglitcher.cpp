#include "ghettoglitcher.hpp"

int main() {
    SerialTrigger *g_Trigger = new SerialTrigger();
    stdio_init_all();
    cyw43_arch_init();
    multicore_launch_core1(serial_console);
    while (true) {
        printf("%d\n", g_Trigger->IsArmed());
        if (g_Trigger->IsArmed()) {
            g_Trigger->Test();
        }
        while (multicore_fifo_rvalid()) {
            uint32_t c_Command = multicore_fifo_pop_blocking();
            switch (c_Command) {
                case CMD_PING: {
                    // Test command.
                    printf("PONG!");
                    break;
                }
                case CMD_SET_PATTERN: {
                    // Set the pattern that we're looking for.
                    uint32_t c_PatternLength = multicore_fifo_pop_blocking();
                    uint8_t c_PatternBuffer[c_PatternLength];
                    memset(c_PatternBuffer, 0, c_PatternLength);
                    for (int i = 0; i < c_PatternLength; ++i) {
                        c_PatternBuffer[i] = (char)multicore_fifo_pop_blocking();
                    }
                    c_PatternBuffer[c_PatternLength] = '\0';
                    g_Trigger->SetPattern(c_PatternBuffer, c_PatternLength);
                    printf("Pattern is %s (length %d)\n.", c_PatternBuffer, c_PatternLength);
                    break;
                }                
                case CMD_SET_BAUD_RATE: {
                    // Set the baud rate.
                    uint32_t c_BaudRate = multicore_fifo_pop_blocking();
                    g_Trigger->SetBaudRate(c_BaudRate);
                    printf("Set baud rate to %d\n.", c_BaudRate);
                    break;
                }                
                case CMD_SET_FORMAT: {
                    // Change the data format.
                    // uint32_t c_BaudRate = multicore_fifo_pop_blocking();
                    // g_Trigger->SetBaudRate(c_BaudRate);
                    break;
                }
                case CMD_ARM: {
                    g_Trigger->Arm();
                    break;
                }
                case CMD_DISARM: {
                    g_Trigger->Disarm();
                    break;
                }
            }
        }
    }
    return 0;
}