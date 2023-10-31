#ifndef SERIAL_CONSOLE
#define SERIAL_CONSOLE

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define CMD_PING 0
#define CMD_SET_PATTERN 1
#define CMD_SET_UART_CHANNEL 2
#define CMD_SET_BAUD_RATE 3
#define CMD_SET_DATA_STOP_BITS 4
#define CMD_SET_PARITY 5

namespace GhettoGlitcha {
    class Console {
        public:
            void Execute();
            void Read(char *command_buffer);
            bool Handle(char *command_buffer);
        private:
            TaskHandle_t g_TriggerTaskHandle;
            static void ArmedTriggerWaitPattern(void *p_Parameters);
            
    };
};

#endif