#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>

volatile char line1[16];
volatile char line2[16];

const char str_days[7][2];
const char str_sync[16];
const char str_config[16];
const char str_brightness[16];
const char str_time[16];
const char str_timer_e[16];
const char str_timer_d[16];
const char str_reboot[16];

void TScreen_init(void);
void TScreen_task(void);
void TScreen_set_handler(void * func);
void TScreen_set_cursor(uint8_t addr);
void TScreen_unset_cursor(void);

#endif
