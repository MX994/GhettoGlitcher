#include "serial_trigger.hpp"

SerialTrigger::SerialTrigger() {
    this->match_buffer = nullptr;
    this->pattern = nullptr;
    this->pattern_length = 0;
    this->baud = 0;
}

void SerialTrigger::SetPattern(uint8_t *pattern, uint32_t size) {
    // If buffer was used before, wipe and free.
    if (this->match_buffer) {
        memset(this->match_buffer, 0, this->pattern_length);
        free(this->match_buffer);
        this->pattern_length = 0;
    }

    // Setup pattern & matching buffers.
    this->pattern_length = size;

    this->match_buffer = (uint8_t *)malloc(this->pattern_length);
    memset(this->match_buffer, 0, this->pattern_length);

    this->pattern = (uint8_t *)malloc(this->pattern_length);
    memset(this->pattern, 0, this->pattern_length);
}

void SerialTrigger::SetUARTChannel(uint32_t id) { this->uart_id = id; }

void SerialTrigger::SetBaudRate(uint32_t baud) { this->baud = baud; }

void SerialTrigger::SetDataAndStopBits(uint32_t data_bits, uint32_t stop_bits) {
    this->data_bits = data_bits;
    this->stop_bits = stop_bits;
}

void SerialTrigger::SetParity(uint32_t parity) { this->parity = parity; }

// UART ISR.
void SerialTrigger::OnRX() {
    while (uart_is_readable(UART_CTRL_ID)) {
        // Shift all elements left by 1 (discarding buf[0])
        for (int i = 0; i < this->pattern_length - 1; ++i) {
            this->match_buffer[i] = this->match_buffer[i + 1];
        }

        // Add new element to buffer.
        this->match_buffer[this->pattern_length - 1] = uart_getc(UART_CTRL_ID);

        // Check if it matches pattern.
        if (!memcmp(this->match_buffer, this->pattern, this->pattern_length)) {
            // Bring some GPIO pin high.
        }
    }
}
