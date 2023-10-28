#ifndef SERIAL_TRIGGER
#define SERIAL_TRIGGER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pico/stdlib.h"
#include "trigger.hpp"

class SerialTrigger : Trigger {
   public:
    SerialTrigger();
    void Arm();
    void Disarm();
    void SetPattern(uint8_t *pattern, uint32_t size);
    void SetBaudRate(uint32_t baud);
    void SetFormat(uint32_t data_bits, uint32_t stop_bits, uart_parity_t parity);
    void Test();
    bool IsArmed();

   private:
    uint8_t *m_MatchBuffer;
    uint8_t *m_Pattern;
    uint32_t m_PatternLength;
    uint32_t m_Baud, m_DataBits, m_StopBits, m_Parity;
};

#endif
