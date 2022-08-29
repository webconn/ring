#include <os/timetable.h>

#include <avr/io.h>
#include <stdint.h>
#include <avr/eeprom.h>

#include <hard/tasks.h>
#include <hard/rtc.h>

#include <hard/led.h>

#include <os/interface.h>

#define DAYS 6
#define ENTRIES 24
#define ENTRY_SIZE 3

#define PROGRAMS 4
#define PROGRAM_SIZE 12

volatile uint8_t tt_signals[DAYS][ENTRIES][ENTRY_SIZE];
volatile uint8_t tt_programs[PROGRAMS][PROGRAM_SIZE];
volatile uint8_t tt_time[3];
volatile uint8_t tt_el[2];

volatile uint8_t day; // index of current day of the week
volatile uint8_t signal = ENTRIES; // index of current signal in timetable

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

    for(a=0; a!=DAYS; a++)
    {
        for(b=0; b!=ENTRIES; b++)
        {
            for(c=0; c!=ENTRY_SIZE; c++)
            {
                eeprom_busy_wait();
                tt_signals[a][b][c] = eeprom_read_byte((uint8_t *) (a*ENTRY_SIZE*ENTRIES+b*ENTRY_SIZE+c));
            }
        }
    }

    // 2. Load signals from EEPROM
    for(a=0; a!=PROGRAMS; a++)
    {
        for(b=0; b!=PROGRAM_SIZE; b++)
        {
            eeprom_busy_wait();
            tt_programs[a][b] = eeprom_read_byte((uint8_t *) (462+a*PROGRAM_SIZE+b));
        }
    }

    // 3. Read time
    tt_time[0] = rtc_hour();
    tt_time[1] = rtc_min();
    tt_time[2] = rtc_sec();

    day = rtc_day();

    // skip if sunday
    if (day != 0)
    {
        uint8_t tt_day = day - 1;

        // 4. Look for available signals
        for(a=0; a!=ENTRIES; a++)
        {
            if(tt_signals[tt_day][a][0] == 255) break; // stop scan if there are no signals

            if((tt_signals[tt_day][a][0] > tt_time[0]) || (tt_signals[tt_day][a][0] == tt_time[0] && tt_signals[tt_day][a][1] > tt_time[1]) || (tt_signals[tt_day][a][0] == tt_time[0] && tt_signals[tt_day][a][1] == tt_time[1] && (tt_signals[tt_day][a][2] & 0x3F) > tt_time[2])) // all conditions about time
            {
                signal = a;
                break;
            }
        }
    }

    TTimetable_task();
}

uint8_t _lday = 255;

void TTimetable_task(void)
{
    // 1. Check current time
    tt_time[0] = rtc_hour();
    tt_time[1] = rtc_min();
    tt_time[2] = rtc_sec();

    day = rtc_day();
    if (_lday == 255)
    {
        _lday = day;
    }

    // 2. Compare it with timetable (and skip sunday)
    if(day != 0 && signal != ENTRIES && tt_signals[day - 1][signal][0] != 255)
    {
        uint8_t tt_day = day - 1;
        if(tt_signals[tt_day][signal][0] == tt_time[0] && tt_signals[tt_day][signal][1] == tt_time[1] && (tt_signals[tt_day][signal][2] & 0x3F) == tt_time[2])
        {
            run_signal();
            signal++;
        }
        tt_el[0] = tt_signals[tt_day][signal][0];
        tt_el[1] = tt_signals[tt_day][signal][1];
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
        _prog_num = ((tt_signals[day-1][signal][2] & 0xC0) >> 6);
        task_add(&_signal, 0);
    }
}
