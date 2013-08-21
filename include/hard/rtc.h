#ifndef RTC_H
#define RTC_H

#ifdef CONFIG_RTC_DS1307
    #include <hard/rtc/ds1307.h>
#endif

#ifdef CONFIG_RTC_PCA2129
    #include "rtc/pca2129.h"
#endif

#include <stdint.h>

uint8_t rtc_sec(void);
uint8_t rtc_min(void);
uint8_t rtc_hour(void);
uint8_t rtc_day(void);

uint8_t rtc_read(uint8_t addr);
void rtc_write(uint8_t addr, uint8_t data);

uint8_t getday(void);

#endif
