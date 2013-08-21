#ifndef LED_H
#define LED_H

#define led_init() DDRB |= (1<<5) // LED pin is PB5
#define led_on() PORTB |= (1<<5)
#define led_off() PORTB &= ~(1<<5)

#endif
