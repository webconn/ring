#ifndef DDTIME_H
#define DDTIME_H

#include <hard/rtc.h>
#include <avr/io.h>
#include <stdint.h>

uint16_t time_createTimestamp(uint16_t d, uint8_t m, uint8_t y);

void time_saveCalTimestamp(uint16_t val);
uint16_t time_getCalTimestamp(void);

void time_saveMoveTimestamp(uint16_t val);
uint16_t time_getMoveTimestamp(void);

void time_saveCalibration(uint8_t val);
uint8_t time_getCalibration(void);

#endif
