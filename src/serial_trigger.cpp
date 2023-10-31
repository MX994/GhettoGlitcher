#include "serial_trigger.hpp"
#include "hardware_map.hpp"

namespace GhettoGlitcha {
    SerialTrigger::SerialTrigger() {
        // Initialize all variables.
        this->m_MatchBuffer = nullptr;
        this->m_Pattern = nullptr;
        this->m_PatternLength = 0;
        this->m_Baud = 0;
        this->m_Armed = false;
        m_SerialPort = new HardwareSerial(UART1);
        pinMode(EMP_TRIGGER_PIN, OUTPUT);
    }

    void SerialTrigger::SetPattern(uint8_t *pattern, uint32_t size) {
        // If buffer was used before, wipe and free.
        if (this->m_MatchBuffer) {
            memset(this->m_MatchBuffer, 0, this->m_PatternLength);
            free(this->m_MatchBuffer);
            this->m_PatternLength = 0;
        }

        // Setup pattern & matching buffers.
        this->m_PatternLength = size;

        this->m_MatchBuffer = (uint8_t *)malloc(this->m_PatternLength);
        memset(this->m_MatchBuffer, 0, this->m_PatternLength);

        this->m_Pattern = (uint8_t *)malloc(this->m_PatternLength);
        memset(this->m_Pattern, 0, this->m_PatternLength);
    }

    void SerialTrigger::SetBaudRate(uint32_t baud) { 
        m_Baud = baud;
    }

    void SerialTrigger::Arm() {
        if (!this->m_MatchBuffer) {
            Serial.println("Invalid pattern; aborting.");
            return;
        }
        Serial.println("Waiting for pattern to show up in traffic!");
        m_SerialPort->begin(m_Baud, SERIAL_8N1, UART_RX, UART_TX);
        this->m_Armed = true;
    }

    void SerialTrigger::Disarm() {
        Serial.println("Stopped waiting for pattern.");
        this->m_Armed = false;
    }

    bool SerialTrigger::IsArmed() {
        return m_Armed;
    }

    // UART ISR.
    void SerialTrigger::Test() {
        // Only parse if data && is armed.
        if (this->m_Armed) {
            // Shift all elements left by 1 (discarding buf[0])
            for (int i = 0; i < this->m_PatternLength - 1; ++i) {
                this->m_MatchBuffer[i] = this->m_MatchBuffer[i + 1];
            }
            Serial.println((char *)this->m_MatchBuffer);
            // Add new element to buffer.
            while (Serial.available() == 0);
            this->m_MatchBuffer[this->m_PatternLength - 1] = Serial1.read();

            // Check if it matches pattern.
            if (!memcmp(this->m_MatchBuffer, this->m_Pattern, this->m_PatternLength)) {
                // Time to trigger.
                digitalWrite(EMP_TRIGGER_PIN, HIGH);
                delayMicroseconds(200);
                digitalWrite(EMP_TRIGGER_PIN, LOW);

                // Disable this to prevent multiple reports.
                this->Disarm();
            }
        }
    }
}