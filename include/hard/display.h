#ifndef DISPLAY_H
#define DISPLAY_H

#include <avr/io.h>
#include <stdint.h>

#define CUR_OFF 0
#define BLINK_ONLY 1
#define CUR_ONLY 2
#define CUR_BLINK 3

void display_init(void);
void display_setaddr(uint8_t addr);
void display_write(char ch);
void display_print(char * string);
void display_setbrgh(uint8_t brgh);
void display_set_cursor(uint8_t mode);

#endif
