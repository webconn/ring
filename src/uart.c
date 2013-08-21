#include <hard/uart.h>

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include <hard/tasks.h>

#define BDRT (F_CPU/16/CONFIG_UART_BAUDRATE - 1)

void * handler = 0; // rx handler

void uart_init(void)
{
    // Calculating baud rate
    UBRRH = (uint8_t) BDRT >> 8;
    UBRRL = (uint8_t) BDRT;
    // Configuring lines
    UCSRB = (1<<RXEN)|(1<<TXEN); // enable Rx,Tx; Rx interrupt
    UCSRB |= (1<<RXCIE);
    // Configuring interface
    UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0); // 8-bit, 1 stop bit, no parity
}

void uart_send_byte(char byte)
{
    while(!(UCSRA & (1<<UDRE))); // waiting for UART ready
    UDR = byte; // send byte
}

void uart_send_string(char * string)
{
    uint8_t i = 0;
    while(string[i] != '\0')
    {
        uart_send_byte(string[i]);
        i++;
    }
}

char uart_receive(void)
{
    while(!(UCSRA & (1<<RXC)));
    return (char) UDR;
}

void uart_set_rx_handler(void * func)
{
    handler = func;
}

ISR(USART_RXC_vect)
{
    sys_reset();
}
