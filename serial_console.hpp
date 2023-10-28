#ifndef SERIAL_CONSOLE
#define SERIAL_CONSOLE

#define CMD_PING 0
#define CMD_SET_PATTERN 1
#define CMD_SET_BAUD_RATE 2
#define CMD_SET_FORMAT 3
#define CMD_ARM 4
#define CMD_DISARM 5

void serial_console();

#endif