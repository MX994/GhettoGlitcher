#ifndef SIGNAL_TRIGGER
#define SIGNAL_TRIGGER

#include <HardwareSerial.h>
#include <Arduino.h>
#include "generic_trigger.hpp"
#include "hardware_map.hpp"

namespace GhettoGlitcha {
    class SignalTrigger : public GenericTrigger {
    public:
        SignalTrigger();
        bool Arm();
        void Disarm();
        bool Test();
        bool IsArmed();
        void SetPin(uint32_t pin);
        const char *TriggerType();
        void SetExtOffset(uint64_t cycles);
        void SetRepeat(uint64_t cycles);

    private:
        uint32_t g_Pin;
    };
}

#endif
