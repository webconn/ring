#include <hard/buzzer.h>

void buzzer(uint16_t w, uint16_t time)
{
    DDRD |= (1<<7);
    uint16_t iters = time;
    uint8_t i;
    cli();
    do
    {
        for(i=0; i!=10; i++)
        {
            PORTD |= (1<<7);
            _delay_us(w);
            PORTD &= ~(1<<7);
            _delay_us(w);
        }
        iters--;
    }
    while(iters != 0);
    sei();
}
