#ifndef OS_INTERFACE_H
#define OS_INTERFACE_H

#include <os/screen.h>
#include <os/strings.h>
#include <os/buttons.h>
#include <os/timetable.h>
#include <hard/rtc.h>
#include <hard/led.h>
#include <hard/display.h>
#include <hard/tasks.h>
#include <hard/time.h>

#include <avr/eeprom.h>
#include <stdint.h>

struct interface_flags {
    unsigned clear : 1;
    unsigned timer : 1;
    unsigned brightness : 4;
} iflags;

void TInterface_init(void);

#endif
