#ifndef SERIAL_CONSOLE
#define SERIAL_CONSOLE

#include "generic_trigger.hpp"
#include "serial_trigger.hpp"
#include "signal_trigger.hpp"
#include "hardware_map.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "FastLED.h"

#define CMD_PING 0
#define CMD_SET_PATTERN 1
#define CMD_SET_UART_CHANNEL 2
#define CMD_SET_BAUD_RATE 3
#define CMD_SET_DATA_STOP_BITS 4
#define CMD_SET_PARITY 5

#define LED_CNT 25

namespace GhettoGlitcha {
    enum TriggerMode {
        TRIGGER_SINGLE,
        TRIGGER_NORMAL
    };

    class Console {
        public:
            Console();
            void Execute();
            int Read(char *command_buffer);
            bool Handle(char *command_buffer);
        private:
            TaskHandle_t g_TriggerTaskHandle;
            GenericTrigger *g_Trigger;
    };
    static SemaphoreHandle_t g_TaskGuard;
    static void ArmedTriggerWaitPattern(void *p_Trigger);
    static TickType_t g_SemaphoreTicks = (TickType_t)100;
    static CRGB g_LEDs[LED_CNT];
    static TriggerMode g_TriggerMode = TRIGGER_NORMAL;
};

#endif