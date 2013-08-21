#include <os/buttons.h>
#include <avr/io.h>

#include <hard/buzzer.h>

void (* handlers[4])(void);

uint8_t buttons, old_buttons, cache_buttons;
uint8_t pos;


void TButtons_init(void)
{
    // 1. set inputs on PD2-PD5
    DDRD &= ~((1<<2)|(1<<3)|(1<<4)|(1<<5));
    PORTD |= (1<<2)|(1<<3)|(1<<4)|(1<<5);

    // 2. add task
    TButtons_task();
}

void TButtons_task(void)
{
    // 1. check all buttons
    buttons = (~PIND & 0x3C) >> 2;
    cache_buttons = buttons;

    pos = 0;
    // 2. scan buttons
    while(buttons)
    {
        if((buttons & 1) && handlers[pos & 0x7F] && !(old_buttons & 1))
        {
            handlers[pos]();
            pos |= 0x80;
        }
        pos++;
        buttons = buttons >> 1;
        old_buttons = old_buttons >> 1;
    }

    old_buttons = cache_buttons;

    if(pos & 0x80)
        buzzer(200, 10);
    

    // reset task
    task_add(&TButtons_task, 40);
}

void TButtons_add_handler(uint8_t button, void * handler)
{
    handlers[button] = handler;
}
