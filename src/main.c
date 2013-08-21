#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <hard/display.h>
#include <hard/tasks.h>
#include <hard/uart.h>
#include <hard/led.h>
#include <hard/ds1307.h>
#include <hard/buzzer.h>

// include tasks
#include <os/screen.h>
#include <os/buttons.h>
#include <os/timetable.h>
#include <os/interface.h>

int main(void)
{
    // init
    uart_init();
    task_init();
    led_init();
    display_init();
    I2CInit();

    display_setbrgh(8);
    
    sei();

    // init OS tasks
    TScreen_init(); // screen update
    TButtons_init(); // buttons check
    TTimetable_init(); // timetable scan
    TInterface_init(); // interface init

    while(1)
    {
        task_manager();
    }

    return 0;
}
