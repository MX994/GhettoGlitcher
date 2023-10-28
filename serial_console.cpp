// Based on
// https://github.com/newaetech/chipshouter-picoemp/blob/main/firmware/c/serial.c

#include "serial_console.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hardware/watchdog.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"

static char serial_buffer[256];

void read_from_stdin() {
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

        if (strlen(serial_buffer) >= sizeof(serial_buffer) - 1) {
            return;
        }

        serial_buffer[strlen(serial_buffer)] = (char)c;
    }
}

bool handle_command(char *command) {
    char internal[256];
    memcpy(internal, command, 256);
    char *token = strtok(internal, " ");
    if (!strcmp(token, "ping")) {
        multicore_fifo_push_blocking(CMD_PING);
        return true;
    } else if (!strcmp(token, "arm")) {
        multicore_fifo_push_blocking(CMD_ARM);
        return true;
    } else if (!strcmp(token, "disarm")) {
        multicore_fifo_push_blocking(CMD_DISARM);
        return true;
    } else if (!strcmp(token, "pattern")) {
        multicore_fifo_push_blocking(CMD_SET_PATTERN);
        char *pattern = strtok(NULL, " ");
        uint32_t pattern_length = strlen(pattern);
        multicore_fifo_push_blocking(pattern_length);
        for (int i = 0; i < pattern_length; ++i) {
            multicore_fifo_push_blocking(pattern[i]);
        }
        return true;
    } else if (!strcmp(token, "baud")) {
        multicore_fifo_push_blocking(CMD_SET_BAUD_RATE);
        multicore_fifo_push_blocking(atoi(strtok(NULL, " ")));
        return true;
    } else if (!strcmp(token, "format")) {
        multicore_fifo_push_blocking(CMD_SET_FORMAT);
        multicore_fifo_push_blocking(atoi(strtok(NULL, " ")));
        multicore_fifo_push_blocking(atoi(strtok(NULL, " ")));
        multicore_fifo_push_blocking(atoi(strtok(NULL, " ")));
        return true;
    }
    return false;
}

void serial_console() {
    multicore_fifo_drain();
    printf("GhettoGlitcher by Ibrahima Keita (Tensor497)\n");
    for (;;) {
        printf(" > ");
        read_from_stdin();
        printf("\n");
        if (!handle_command(serial_buffer)) {
            printf("GhettoGlitcher Commands:\n");
            printf("- ping: pong\n");
            printf("- arm: wait for a pattern to show up over UART\n");
            printf("- disarm: stop waiting for a pattern\n");
            printf("- pattern <pattern>: Set the pattern to look for.\n");
            printf("- baud <baud_rate>: Set the baud rate of the communication channel.\n");
            printf("- format <data_bits> <stop_bits> <parity>: Sets the data format (data bits, stop bits, parity bit)\n");
        }
        printf("\n");
    }
}
