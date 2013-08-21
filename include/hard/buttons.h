#ifndef BUTTONS_H
#define BUTTONS_H

#include <avr/io.h>
#include <stdint.h>

void buttons_init(void);
uint8_t buttons_read(void);
void buttons_set_handler(uint8_t num, void * func);

#endif
