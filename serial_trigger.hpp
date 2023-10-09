#ifndef SERIAL_TRIGGER
#define SERIAL_TRIGGER

#include "<ctypes.h>"

// UART pins.
#define UART_TX 0
#define UART_RX 1

// UART Controller.
#define UART_CTRL_ID 0

namespace PicoWhisperer {
    class SerialTrigger {
        public:
            SerialTrigger();
            void Init(uint8_t *pattern, uint32_t baud);
        private:
            uint8_t *MatchBuffer;
    };
}

#endif
