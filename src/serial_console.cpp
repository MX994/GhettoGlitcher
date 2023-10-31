#include "serial_console.hpp"

#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace GhettoGlitcha {
    void Console::Execute() {
        char command_buffer[0x100];
        memset(command_buffer, 0, 0x100);
        for (;;) {
            Serial.print(" > ");
            Console::Read(command_buffer);
            Serial.println("");
            if (!this->Handle(command_buffer)) {
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
        memcpy(internal_command_buffer, command_buffer, sizeof(internal_command_buffer));
        char *token = strtok(internal_command_buffer, " ");

        // Check the command and handle parameters varadically.
        if (!strcmp(token, "ping")) {
            Serial.println("PONG!");
            return true;
        } else if (!strcmp(token, "arm")) {
            xTaskCreatePinnedToCore(ArmedTriggerWaitPattern, "ArmedTrigger", 0x400, NULL, 1, &g_TriggerTaskHandle, 0);
            return true;
        } else if (!strcmp(token, "disarm")) {
            return true;
        } else if (!strcmp(token, "pattern")) {
            char *pattern = strtok(NULL, " ");
            uint32_t pattern_length = strlen(pattern);
            return true;
        } else if (!strcmp(token, "baud")) {
            uint32_t new_baud = atoi(strtok(NULL, " "));
            return true;
        }
        return false;
    }

    void Console::ArmedTriggerWaitPattern(void *p_Parameters){
        for (;;) {

        }
    }
}