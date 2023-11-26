#ifndef SYS_UTIL
#define SYS_UTIL

#include <HardwareSerial.h>
#include <Arduino.h>
#include "generic_trigger.hpp"
#include "hardware_map.hpp"

namespace GhettoGlitcha {
    class SystemUtility {
    public:
        static void DelayCycles(uint64_t cycles);
    };
}

#endif
