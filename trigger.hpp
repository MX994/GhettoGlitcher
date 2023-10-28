#ifndef TRIGGER
#define TRIGGER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

class Trigger {
    protected:
        bool m_Armed;
};


#endif