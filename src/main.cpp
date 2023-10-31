#include <Arduino.h>
#include "serial_console.hpp"

GhettoGlitcha::Console *g_Console;

void setup() {
  Serial.begin(115200);
  g_Console = new GhettoGlitcha::Console();
}

void loop() {
  g_Console->Execute();
}

