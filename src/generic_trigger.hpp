#ifndef GENERIC_TRIGGER
#define GENERIC_TRIGGER
#include <Arduino.h>
#include "hardware_map.hpp"

class GenericTrigger {
    protected:
        bool m_Armed;
    public:
        virtual bool Test();
        virtual bool IsArmed();
        virtual bool Arm();
        virtual void Disarm();
        virtual const char *TriggerType();
        // virtual void SetExtOffset(uint64_t cycles);
        // virtual void SetRepeat(uint64_t cycles);
        void Wait(uint64_t cycles) {
            for (int i = 0; i < cycles; ++i) {
                asm("nop");
            }
        }
        void Glitch() {
            Wait(this->c_ExtOffsetCycles);
            digitalWrite(EMP_TRIGGER_PIN, HIGH);
            Wait(this->c_RepeatCycles);
            digitalWrite(EMP_TRIGGER_PIN, LOW);
        }
        void SetExtOffset(uint64_t cycles) {
            this->c_ExtOffsetCycles = cycles;
        }
        void SetRepeat(uint64_t cycles) {
            this->c_RepeatCycles = cycles;
        }
    private:
        uint64_t c_RepeatCycles = 0, c_ExtOffsetCycles = 0;

};

#endif