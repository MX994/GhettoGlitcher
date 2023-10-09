#include "serial_trigger.hpp"

namespace PicoWhisperer {
    SerialTrigger::SerialTrigger() {
        this->MatchBuffer = nullptr;
        this->PatternSize = 0;
        this->ExpectedPattern = nullptr;
    }

    void Init(uint8_t *pattern, uint32_t size, uint32_t baud) {
        // Store pattern.
        this->ExpectedPattern = (uint8_t *)malloc(size);
        memcpy(this->ExpectedPattern, pattern, size);

        // Create a buffer to check matches.
        this->MatchBuffer = (uint8_t *)malloc(size);
        memset(0, this->MatchBuffer, size);
        this->PatternSize = size;
    }

    // UART ISR.
    void OnRX() {
        while (uart_is_readable(UART_CTRL_ID)) {
            // Shift all elements left by 1 (discarding buf[0])
            for (int i = 0; i < this->PatternSize - 1; ++i) {
                this->MatchBuffer[i] = this->MatchBuffer[i + 1];
            }

            // Add new element to buffer.
            this->MatchBuffer[MatchBufferSize - 1] = uart_getc();

            // Check if it matches pattern.
            if (!memcmp(this->MatchBuffer, this->ExpectedPattern)) {
                // Bring some GPIO pin high.
            }
        }
    }
};