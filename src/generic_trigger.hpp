#ifndef GENERIC_TRIGGER
#define GENERIC_TRIGGER
#include <Arduino.h>

class GenericTrigger {
    protected:
        bool m_Armed;
    public:
        virtual bool Test();
        virtual bool IsArmed();
        virtual bool Arm();
        virtual void Disarm();
        virtual const char *TriggerType();
        virtual void SetExtOffset(uint64_t cycles);
        virtual void SetRepeat(uint64_t cycles);
};

#endif