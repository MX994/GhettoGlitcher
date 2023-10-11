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

        if (c == '\r' || c == ' ') {
            return;
        }

        if (c == '\n') {
            continue;
        }

        if (strlen(serial_buffer) >= 255) {
            return;
        }

        serial_buffer[strlen(serial_buffer)] = (char)c;
    }
}

bool handle_command(char *command) {
    if (!strcmp(command, "ping")) {
        multicore_fifo_push_blocking(CMD_PING);
        return true;
    } else if (!strcmp(command, "pattern")) {
        multicore_fifo_push_blocking(CMD_SET_PATTERN);
        read_from_stdin();
        multicore_fifo_push_blocking(strlen(serial_buffer));
        for (int i = 0; i < strlen(serial_buffer); ++i) {
            multicore_fifo_push_blocking(serial_buffer[i]);
        }
        return true;
    } else if (!strcmp(command, "baud")) {
        multicore_fifo_push_blocking(CMD_SET_BAUD_RATE);
        read_from_stdin();
        multicore_fifo_push_blocking(atoi(serial_buffer));
        return true;
    } else if (!strcmp(command, "data_stop_bits")) {
        multicore_fifo_push_blocking(CMD_SET_DATA_STOP_BITS);
        read_from_stdin();
        multicore_fifo_push_blocking(atoi(serial_buffer));
        read_from_stdin();
        multicore_fifo_push_blocking(atoi(serial_buffer));
        return true;
    } else if (!strcmp(command, "parity")) {
        multicore_fifo_push_blocking(CMD_SET_PARITY);
        read_from_stdin();
        multicore_fifo_push_blocking(atoi(serial_buffer));
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
            printf("- pattern <pattern>: Set the pattern to look for.\n");
            printf(
                "- baud <baud_rate>: Set the baud rate of the communication "
                "channel.\n");
            printf(
                "- data_stop_bits <data_bits> <stop_bits>: Set the number of "
                "bits to "
                "use for data and stop.\n");
            printf(
                "- parity <parity>: Set parity used in the communication "
                "channel.\n");
        }
        printf("\n");
    }
}
