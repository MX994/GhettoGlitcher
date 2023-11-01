#ifndef SERIAL_TRIGGER
#define SERIAL_TRIGGER

#include <HardwareSerial.h>
#include <Arduino.h>
#include "generic_trigger.hpp"
#include "hardware_map.hpp"

namespace GhettoGlitcha {
    class SerialTrigger : public GenericTrigger {
    public:
        SerialTrigger();
        bool Arm();
        void Disarm();
        void SetPattern(uint8_t *pattern, uint32_t size);
        void SetBaudRate(uint32_t baud);
        bool Test();
        bool IsArmed();

    private:
        uint8_t *m_MatchBuffer;
        uint8_t *m_Pattern;
        uint32_t m_PatternLength;
        uint32_t m_Baud;
        HardwareSerial *m_SerialPort;
    };
}

#endif
