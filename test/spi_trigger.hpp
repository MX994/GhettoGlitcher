#ifndef SERIAL_TRIGGER
#define SERIAL_TRIGGER

#include <HardwareSerial.h>
#include <Arduino.h>
#include "generic_trigger.hpp"
#include "hardware_map.hpp"

namespace GhettoGlitcha {
    class SPITrigger : public GenericTrigger {
    public:
        SPITrigger();
        bool Arm();
        void Disarm();
        void SetTriggerType()
        void SetPattern(uint8_t *pattern, uint32_t size);
        void SetBaudRate(uint32_t baud);
        bool Test();
        bool IsArmed();
        void SetExtOffset(uint64_t cycles);
        void SetRepeat(uint64_t cycles);

    private:
        uint8_t *m_MatchBuffer;
        uint8_t *m_Pattern;
        uint32_t m_PatternLength;
        uint32_t m_Baud;
        HardwareSerial *m_SerialPort;
    };
}

#endif
