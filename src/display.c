#include <hard/display.h>
#include <avr/io.h>
#include <util/delay.h>

#define RS 2
#define RW 1
#define E_PIN 0

#define STROBE_DELAY 1

void ws0010_strobe(void)
{
    PORTB |= 1<<E_PIN;
    _delay_us(STROBE_DELAY);
    PORTB &= ~(1<<E_PIN);
    _delay_us(STROBE_DELAY);
}

void ws0010_wait(void)
{
    DDRA = 0x00;
    do
    {
        PORTB &= ~(1<<RS);
        PORTB |= 1<<RW;
        ws0010_strobe();
    }
    while(PORTA & (1<<7));
}

void ws0010_rq(uint8_t rs, uint8_t rw, uint8_t data)
{
    #ifdef CONFIG_DISPLAY_LCD_NOCYR
    _delay_us(50);
    #endif

    PORTB &= ~((1<<RS)|(1<<RW));
    PORTB |= (rs<<RS)|(rw<<RW);
    
    DDRA = 0xFF;
    PORTA = data;

    ws0010_strobe();
}

void display_init(void)
{
    // 1. data directions
    DDRB |= (1<<RS)|(1<<RW)|(1<<E_PIN)|(1<<3);

    #if defined(CONFIG_DISPLAY_OLED)
    // 2. send init codes
    ws0010_rq(0, 0, 0b00111010); // cyrillic chartable
    ws0010_rq(0, 0, 0b00001100); // enable display
    ws0010_rq(0, 0, 0b00000001); // clean display
    ws0010_rq(0, 0, 0b00000110); // increment, no shift
    #elif defined(CONFIG_DISPLAY_LCD_NOCYR)
    _delay_ms(50);
    ws0010_rq(0, 0, 0b00111000); // 2-line, 8-bit
    ws0010_rq(0, 0, 0b00111000); // repeat
    ws0010_rq(0, 0, 0b00001100); // enable display
    ws0010_rq(0, 0, 0b00000001); // clear display
    _delay_ms(2);
    ws0010_rq(0, 0, 0b00000110); // increment, no shift
    #endif

    display_setaddr(0);
    // 3. brightness control
    // set on Timer0
    TCCR0 = (1<<WGM01)|(1<<WGM00)|(1<<COM01)|(1<<COM00)|(1<<CS00);
}

void display_setbrgh(uint8_t brgh)
{
    if(brgh == 0)
        OCR0 = 190;
    else if(brgh == 1)
        OCR0 = 195;
    else if(brgh == 2)
        OCR0 = 200;
    else if(brgh == 3)
        OCR0 = 205;
    else if(brgh == 4)
        OCR0 = 210;
    else if(brgh == 5)
        OCR0 = 215;
    else if(brgh == 6)
        OCR0 = 220;
    else if(brgh == 7)
        OCR0 = 230;
    else if(brgh == 8)
        OCR0 = 255;
}

void display_write(char ch)
{
    ws0010_rq(1, 0, ch);
}

void display_print(char * string)
{
    uint8_t i = 0;
    while(string[i] != '\0')
    {
        display_write(string[i]);
        i++;
    }
}

void display_setaddr(uint8_t addr)
{
    ws0010_rq(0, 0, (addr | (1<<7)));
}

void display_set_cursor(uint8_t mode)
{
    ws0010_rq(0, 0, (mode & 0x03) | 0x0C);
}
