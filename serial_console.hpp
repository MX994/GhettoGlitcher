#ifndef SERIAL_CONSOLE
#define SERIAL_CONSOLE

#define CMD_PING 0
#define CMD_SET_PATTERN 1
#define CMD_SET_UART_CHANNEL 2
#define CMD_SET_BAUD_RATE 3
#define CMD_SET_DATA_STOP_BITS 4
#define CMD_SET_PARITY 5

void serial_console();

#endif