#include <os/screen.h>

#define SHOW_STRINGS
#include <os/strings.h>
#undef SHOW_STRINGS

#include <hard/display.h>
#include <hard/tasks.h>
#include <hard/buzzer.h>

void (* _handler)(void);

// Strings defines

// Code defines

volatile char line1[16] = { SYM_BELL, ' ', '3', 'B', 'O', 'H', 'O', 'K', ' ', 'v', '.', '0', '.', '1', ' ', SYM_BELL };
volatile char line2[16] = { ' ', 'L', 'o', 'a', 'd', 'i', 'n', 'g', ' ', ' ', 'O', 'S', '.', '.', '.', ' ' };

uint8_t cur_addr = 0x51;

void TScreen_init(void)
{
    // add task
    TScreen_task();
    buzzer(200, 10);
}

void TScreen_task(void)
{
    uint8_t i;

    if(_handler)
        _handler();

    // output
    display_setaddr(0x00);
    for(i=0; i!=16; i++)
    {
        display_write(line1[i]);
    }

    display_setaddr(0x40);
    for(i=0; i!=16; i++)
    {
        #ifdef DEBUG_NT
        if(i == 0) display_write(num_tasks + 0x30);
        #endif
        display_write(line2[i]);
    }

    display_setaddr(cur_addr);

    // read task
    task_add(&TScreen_task, 200);
}

void TScreen_set_handler(void * func)
{
    _handler = func;
}

void TScreen_set_cursor(uint8_t addr)
{
    display_set_cursor(CUR_BLINK);
    cur_addr = addr;
}

void TScreen_unset_cursor(void)
{
    display_set_cursor(CUR_OFF);
    cur_addr = 0x51;
}
