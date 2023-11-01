#include "serial_console.hpp"
#include "soc/rtc_wdt.h"

namespace GhettoGlitcha {
    Console::Console() {
        g_Trigger = new GhettoGlitcha::SerialTrigger();
        g_TaskGuard = xSemaphoreCreateMutex();
    }
    void Console::Execute() {
        char command_buffer[0x100];
        memset(command_buffer, 0, 0x100);
        for (;;) {
            Serial.print("> ");
            Console::Read(command_buffer);
            Serial.println("");
            if (!strlen(command_buffer) || !this->Handle(command_buffer)) {
                Serial.println("GhettoGlitcher Commands:");
                Serial.println("- ping: pong");
                Serial.println("- arm: wait for a pattern to show up over UART");
                Serial.println("- disarm: stop waiting for a pattern");
                Serial.println("- pattern <pattern>: Set the pattern to look for.");
                Serial.println("- baud <baud_rate>: Set the baud rate of the communication channel.");
            }
        }
    }

    void Console::Read(char *command_buffer) {
        memset(command_buffer, 0, sizeof(command_buffer));
        for (;;) {
            while (Serial.available() == 0);
            char c = Serial.read();

            // Check if character is EOF; if so return.
            if (c == EOF) {
                return;
            }

            // Push character to the serial buffer.
            Serial.print(c);

            if (c == '\r' || c == '\n') {
                // End of line; we're done.
                return;
            }
            
            // Store in the buffer.
            command_buffer[strlen(command_buffer)] = (char)c;
        }
    }

    bool Console::Handle(char *command_buffer) {
        // Start tokenizing the input.
        char internal_command_buffer[0x100];
        memset(internal_command_buffer, 0, 0x100);
        memcpy(internal_command_buffer, command_buffer, sizeof(internal_command_buffer));
        char *token = strtok(internal_command_buffer, " ");

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
        } else if (!strcmp(token, "pattern")) {
            char *pattern = strtok(NULL, " ");
            uint32_t pattern_length = strlen(pattern);
            pattern[pattern_length] = '\0';
            ((SerialTrigger*)g_Trigger)->SetPattern((uint8_t*)pattern, pattern_length);
            return true;
        } else if (!strcmp(token, "baud")) {
            ((SerialTrigger*)g_Trigger)->SetBaudRate(atoi(strtok(NULL, " ")));
            return true;
        }
        return false;
    }

    static void ArmedTriggerWaitPattern(void *p_Trigger) {
        // Pass in an instance of the trigger object.
        GenericTrigger *c_Trigger = (GenericTrigger *)p_Trigger;
        Serial.println("We're still alive.");
        for (;;) {
            if (c_Trigger->Test()) { 
                Serial.println("Hit!");
                break;
            }
        }
        vTaskDelete(NULL);
    }
}