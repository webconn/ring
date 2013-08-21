#ifndef UART_H
#define UART_H

#include <stdint.h>

void uart_init(void);
void uart_send_byte(char byte);
void uart_send_string(char * string);
uint8_t * uart_getrxbuffer(void);
char uart_receive(void);
void uart_set_rx_handler(void * func);

#endif
