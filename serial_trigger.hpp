#ifndef SERIAL_TRIGGER
#define SERIAL_TRIGGER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pico/stdlib.h"

// UART pins.
#define UART_TX 0
#define UART_RX 1

// UART Controller.
#define UART_CTRL_ID 0

class SerialTrigger {
   public:
    SerialTrigger();
    void SetPattern(uint8_t *pattern, uint32_t size);
    void SetUARTChannel(uint32_t id);
    void SetBaudRate(uint32_t baud);
    void SetDataAndStopBits(uint32_t data_bits, uint32_t stop_bits);
    void SetParity(uint32_t parity);
    void OnRX();

   private:
    uint8_t *match_buffer;
    uint8_t *pattern;
    uint32_t pattern_length;
    uint32_t uart_id, baud, data_bits, stop_bits, parity;
};

#endif
