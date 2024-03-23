#include "serial_trigger.hpp"

namespace GhettoGlitcha {
    SPITrigger::SPITrigger() {
        // Initialize all variables.
        this->m_MatchBuffer = nullptr;
        this->m_Pattern = nullptr;
        this->m_PatternLength = 0;
        this->m_Baud = 9600;
        this->m_Armed = false;
        m_SerialPort = new HardwareSerial(UART1);
        pinMode(EMP_TRIGGER_PIN, OUTPUT);
    }

    void SPITrigger::SetPattern(uint8_t *pattern, uint32_t size) {
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
        this->m_MatchBuffer[this->m_PatternLength] = '\0';

        this->m_Pattern = (uint8_t *)malloc(this->m_PatternLength);
        memset(this->m_Pattern, 0, this->m_PatternLength);
        memcpy(this->m_Pattern, pattern, size);
        this->m_Pattern[this->m_PatternLength] = '\0';
    }

    void SPITrigger::SetBaudRate(uint32_t baud) { 
        m_Baud = baud;
    }

    bool SPITrigger::Arm() {
        if (!this->m_MatchBuffer) {
            Serial.println("Invalid pattern; aborting.");
            return false;
        }
        Serial.println("Waiting for pattern to show up in traffic!");
        Serial.print("Expected pattern:");
        Serial.println((char *)this->m_Pattern);
        m_SerialPort->begin(m_Baud, SERIAL_8N1, UART_RX, UART_TX);
        this->m_Armed = true;
        return true;
    }

    void SPITrigger::Disarm() {
        Serial.println("Stopped waiting for pattern.");
        m_SerialPort->end();
        this->m_Armed = false;
    }

    bool SPITrigger::IsArmed() {
        return m_Armed;
    }

    // UART ISR.
    bool SPITrigger::Test() {
        // Only parse if data && is armed.
        if (this->m_Armed) {
            // Shift all elements left by 1 (discarding buf[0]).
            for (int i = 0; i < this->m_PatternLength - 1; ++i) {
                this->m_MatchBuffer[i] = this->m_MatchBuffer[i + 1];
            }
            // Add new element to buffer.
            while (m_SerialPort->available() == 0);
            this->m_MatchBuffer[this->m_PatternLength - 1] = m_SerialPort->read();
            Serial.println(this->m_MatchBuffer[this->m_PatternLength]);

            // Check if it matches pattern.
            if (!memcmp(this->m_MatchBuffer, this->m_Pattern, this->m_PatternLength)) {
                this->Glitch();
                this->Disarm();
                return true;
            }
        }
        return false;
    }

    const char *SPITrigger::TriggerType() {
        return "SPI";
    }
}