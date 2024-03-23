#include "signal_trigger.hpp"

namespace GhettoGlitcha {
    SignalTrigger::SignalTrigger() {
        // Initialize all variables.
        this->m_Armed = false;
        this->g_Pin = false;
    }

    void SignalTrigger::SetPin(uint32_t pin) {
        this->g_Pin = pin;
        pinMode(this->g_Pin, INPUT);
    }

    bool SignalTrigger::Arm() {
        Serial.println("Waiting...");
        this->m_Armed = true;
        return true;
    }

    void SignalTrigger::Disarm() {
        Serial.println("Stopped waiting.");
        this->m_Armed = false;
    }

    bool SignalTrigger::IsArmed() {
        return m_Armed;
    }

    // UART ISR.
    bool SignalTrigger::Test() {
        if (this->m_Armed) {
            while (!digitalRead(g_Pin));
            this->Glitch();
            // this->Disarm();
            return true;
        }
        return false;
    }

    const char *SignalTrigger::TriggerType() {
        return "Signal";
    }
}