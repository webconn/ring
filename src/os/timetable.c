#include <os/timetable.h>

#include <avr/io.h>
#include <stdint.h>
#include <avr/eeprom.h>

#include <hard/tasks.h>
#include <hard/rtc.h>

#include <hard/led.h>

#include <os/interface.h>

volatile uint8_t tt_signals[7][24][3];
volatile uint8_t tt_programs[4][12];
volatile uint8_t tt_time[3];
volatile uint8_t tt_el[2];

volatile uint8_t day; // index of current day of the week
volatile uint8_t signal = 22; // index of current signal in timetable

void run_signal(void);

void TTimetable_init(void)
{
    uint8_t tmp = rtc_read(0);
    tmp &= ~(1<<7);
    rtc_write(0, tmp);

    tmp = rtc_read(RTC_HOUR);
    tmp &= ~(1<<6);
    rtc_write(RTC_HOUR, tmp);

    DDRD |= 1<<6;
    // 1. Load timetable from EEPROM
    uint8_t a, b, c;
    for(a=0; a!=7; a++)
    {
        for(b=0; b!=22; b++)
        {
            for(c=0; c!=3; c++)
            {
                eeprom_busy_wait();
                tt_signals[a][b][c] = eeprom_read_byte((uint8_t *) (a*66+b*3+c));
            }
        }
    }

    // 2. Load signals from EEPROM
    for(a=0; a!=4; a++)
    {
        for(b=0; b!=12; b++)
        {
            eeprom_busy_wait();
            tt_programs[a][b] = eeprom_read_byte((uint8_t *) (462+a*12+b));
        }
    }

    // 3. Read time
    tt_time[0] = rtc_hour();
    tt_time[1] = rtc_min();
    tt_time[2] = rtc_sec();

    day = rtc_day();
   
    // 4. Look for available signals
    for(a=0; a!=22; a++)
    {
        if(tt_signals[day][a][0] == 255) break; // stop scan if there are no signals

        if((tt_signals[day][a][0] > tt_time[0]) || (tt_signals[day][a][0] == tt_time[0] && tt_signals[day][a][1] > tt_time[1]) || (tt_signals[day][a][0] == tt_time[0] && tt_signals[day][a][1] == tt_time[1] && (tt_signals[day][a][2] & 0x3F) > tt_time[2])) // all conditions about time
        {
            signal = a;
            break;
        }
    }

    TTimetable_task();
}

uint8_t _lday;

void TTimetable_task(void)
{
    // 1. Check current time
    tt_time[0] = rtc_hour();
    tt_time[1] = rtc_min();
    tt_time[2] = rtc_sec();

    day = rtc_day();
   
    // 2. Compare it with timetable
    if(signal != 22 && tt_signals[day][signal][0] != 255)
    {
        if(tt_signals[day][signal][0] == tt_time[0] && tt_signals[day][signal][1] == tt_time[1] && (tt_signals[day][signal][2] & 0x3F) == tt_time[2])
        {
            run_signal();
            signal++;
        }
        tt_el[0] = tt_signals[day][signal][0];
        tt_el[1] = tt_signals[day][signal][1];
    }
    else
    {
        if(_lday != day)
            signal = 0;
        else
            _lday = day;

        tt_el[0] = 255;
        tt_el[1] = 255;
    }

    task_add(&TTimetable_task, 200);
}

uint8_t _prog_index = 0;
uint8_t _prog_num = 0;

void _signal(void)
{
    if(tt_programs[_prog_num][_prog_index] & 0x80)
    {
        DDRD |= 1<<6;
        PORTD |= 1<<6;
    }
    else
    {
        DDRD &= ~(1<<6);
        PORTD &= ~(1<<6);
    }

    if(_prog_index != 11) task_add(&_signal, (tt_programs[_prog_num][_prog_index++] & 0x7F) * 100);
    else
    {
        DDRD &= ~(1<<6);
        PORTD &= ~(1<<6);
    }

}

void run_signal(void)
{
    _prog_index = 0;
    if(iflags.timer)
    {
        _prog_num = ((tt_signals[day][signal][2] & 0xC0) >> 6);
        task_add(&_signal, 0);
    }
}
