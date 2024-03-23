#include <Arduino.h>

#include "serial_console.hpp"

GhettoGlitcha::Console *g_Console;

void setup() {
    Serial.begin(9600);
    g_Console = new GhettoGlitcha::Console();
    g_Console->Execute();
}

void loop() { }
