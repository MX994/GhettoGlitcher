#include "serial_console.hpp"
#include "soc/rtc_wdt.h"
#include <FastLED.h>

namespace GhettoGlitcha {
    Console::Console() {
        g_Trigger = new GhettoGlitcha::SerialTrigger();
        g_TaskGuard = xSemaphoreCreateMutex();

        // Turn on LEDs, set them up.
        pinMode(LED_MATRIX_PWR, OUTPUT);
        digitalWrite(LED_MATRIX_PWR, HIGH);
        FastLED.addLeds<NEOPIXEL, LED_MATRIX_DATA>(g_LEDs, LED_CNT);
    }
    void Console::Execute() {
        char command_buffer[0x100];
        uint64_t c_ExtCycleCount = 0, c_RepeatCycleCount = 0;
        for (;;) {
            Serial.print("> ");
            memset(command_buffer, 0, 0x100);
            if (Console::Read(command_buffer)) {
                Serial.println("");
                if (!this->Handle(command_buffer)) {
                    Serial.println("GhettoGlitcher Commands:");
                    Serial.println("- ping: pong");
                    Serial.println("- arm: wait for trigger.");
                    Serial.println("- disarm: stop waiting for trigger.");
                    Serial.println("- ext_offset: how many cycles to wait before pulsing.");
                    Serial.println("- repeat: how many cycles to pulse for.");

                    const char *trigger_type = g_Trigger->TriggerType();
                    Serial.printf("Trigger Mode: %s\n", trigger_type);
                    if (!strcmp(trigger_type, "Serial")) {
                        Serial.println("- pattern <pattern>: Set the pattern to look for.");
                        Serial.println("- baud <baud_rate>: Set the baud rate of the communication channel.");
                    } 
                    if (!strcmp(trigger_type, "Signal")) {
                    }
                }
            }
        }
    }

    int Console::Read(char *command_buffer) {
        int read = 0;
        memset(command_buffer, 0, sizeof(command_buffer));
        for (;;) {
            while (Serial.available() == 0);
            char c = Serial.read();

            // Check if character is EOF; if so return.
            if (c == EOF) {
                return read;
            }
            // Push character to the serial buffer.
            Serial.print(c);

            if (c == '\r' || c == '\n' || c == '\0') {
                // End of line; we're done.
                command_buffer[strlen(command_buffer)] = '\0';
                return read;
            }

            read++;

            // Store in the buffer.
            command_buffer[strlen(command_buffer)] = c;
        }
    }

    bool Console::Handle(char *command_buffer) {
        // Start tokenizing the input.
        char internal_command_buffer[0x100];
        memset(internal_command_buffer, 0, 0x100);
        memcpy(internal_command_buffer, command_buffer, sizeof(internal_command_buffer));
        char *token = strtok(internal_command_buffer, " ");
        Serial.println(token);
        // Check the command and handle parameters varadically.
        if (!strcmp(token, "ping")) {
            Serial.println("PONG!");
            return true;
        } else if (!strcmp(token, "arm")) {
            // Check if armed already.
            if (g_Trigger->IsArmed()) {
                Serial.println("Already waiting for the target. Aborting...");
            } else if (g_Trigger->Arm()) {
                xTaskCreatePinnedToCore(ArmedTriggerWaitPattern, "ArmedTrigger", 0x400, (void *)this->g_Trigger, 1, &g_TriggerTaskHandle, 0);
            }
            return true;
        } else if (!strcmp(token, "disarm")) {
            g_Trigger->Disarm();
            return true;
        } else if (!strcmp(token, "ext_offset")) {
            char *offset = strtok(NULL, " ");
            uint32_t length = strlen(offset);
            offset[length] = '\0';
            g_Trigger->SetExtOffset(atoi(offset));
            return true;
        } else if (!strcmp(token, "repeat")) {
            char *offset = strtok(NULL, " ");
            uint32_t length = strlen(offset);
            offset[length] = '\0';
            g_Trigger->SetRepeat(atoi(offset));
            return true;
        } else if (!strcmp(token, "trigger_type")) {
            char *trigger_type = strtok(NULL, " ");
            if (!strcmp(trigger_type, "serial")) {
                g_Trigger = new GhettoGlitcha::SerialTrigger();
            } else if (!strcmp(trigger_type, "signal")) {
                g_Trigger = new GhettoGlitcha::SignalTrigger();
            } else if (!strcmp(trigger_type, "can")) {

            } else if (!strcmp(trigger_type, "spi")) {
                
            } else {
                Serial.println("Invalid serial choice! Choices are serial, signal, can.");
            }
            return true;
        } else if (!strcmp(token, "trigger_type")) {
            char *trigger_type = strtok(NULL, " ");
            if (!strcmp(trigger_type, "serial")) {
                g_Trigger = new GhettoGlitcha::SerialTrigger();
            } else if (!strcmp(trigger_type, "signal")) {
                g_Trigger = new GhettoGlitcha::SignalTrigger();
            } else if (!strcmp(trigger_type, "can")) {

            } else if (!strcmp(trigger_type, "spi")) {
                
            } else {
                Serial.println("Invalid serial choice! Choices are serial, signal, can.");
            }
            return true;
        } else if (!strcmp(token, "trigger_mode")) {
            char *mode = strtok(NULL, " ");
            if (!strcmp(mode, "single")) {
                g_TriggerMode = TRIGGER_SINGLE;
            } else if (!strcmp(mode, "normal")) {
                g_TriggerMode = TRIGGER_NORMAL;
            } 
            return true;
        }
        const char *trigger_type = g_Trigger->TriggerType();
        Serial.printf("Trigger Type: %s\n", trigger_type);
        if (!strcmp(trigger_type, "Serial")) {
            if (!strcmp(token, "pattern")) {
                char *pattern = strtok(NULL, " ");
                uint32_t pattern_length = strlen(pattern);
                pattern[pattern_length] = '\0';
                ((SerialTrigger*)g_Trigger)->SetPattern((uint8_t*)pattern, pattern_length);
                return true;
            } else if (!strcmp(token, "baud")) {
                ((SerialTrigger*)g_Trigger)->SetBaudRate(atoi(strtok(NULL, " ")));
                return true;
            }
        } 
        if (!strcmp(trigger_type, "Signal")) {
            if (!strcmp(token, "pin")) {
                ((SignalTrigger*)g_Trigger)->SetPin(atoi(strtok(NULL, " ")));
                return true;
            }
        }
        return false;
    }

    static void ArmedTriggerWaitPattern(void *p_Trigger) {
        // Pass in an instance of the trigger object.
        GenericTrigger *c_Trigger = (GenericTrigger *)p_Trigger;

        // Set to red.
        fill_solid(g_LEDs, LED_CNT, 0xFF0000);
        FastLED.show();
        for (;;) {
            bool result = c_Trigger->Test();

            // Show result on LEDs.
            fill_solid(g_LEDs, LED_CNT, result ? 0xFF00 : 0xFF0000);
            FastLED.show();

            if (result && g_TriggerMode == TRIGGER_SINGLE) {
                c_Trigger->Disarm();
                break;
            }
        }
        vTaskDelete(NULL);
    }
}