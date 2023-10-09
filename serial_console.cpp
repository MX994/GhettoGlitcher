// Based on https://github.com/newaetech/chipshouter-picoemp/blob/main/firmware/c/serial.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "serial_console.hpp"
#include "hardware/watchdog.h"

static char serial_buffer[256];

void read_line() {
    memset(serial_buffer, 0, sizeof(serial_buffer));
    while (1) {
        int c = getchar();
        if (c == EOF) {
            return;
        }

        putchar(c);

        if (c == '\r') {
            return;
        }
        if (c == '\n') {
            continue;
        }
    }
}

bool handle_command(char *command) {
    if (!strcmp(command, "ping")) {
        multicore_fifo_push_blocking(CMD_PING);
        return true;
    }
    return false;
}

void serial_console() {
    multicore_fifo_drain();
    printf("PicoWhisperer by Ibrahima Keita (Tensor497)\n");
    for (;;) {
        read_line();
        printf("\n");
        if(!handle_command(serial_buffer)) {
            printf("Unknown command.");
        }
        printf("\n");
        printf(" > ");
    }
}