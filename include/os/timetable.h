#ifndef TIMETABLE_H
#define TIMETABLE_H

#include <stdint.h>

volatile uint8_t tt_el[2];

void TTimetable_init(void);
void TTimetable_task(void);

#endif
