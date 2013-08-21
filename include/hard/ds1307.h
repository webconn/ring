#ifndef DS1307_H
#define DS1307_H

#include <stdint.h>

#define TRUE 1
#define FALSE 0

void I2CInit(void);
uint8_t DS1307Write(uint8_t address, uint8_t data);
uint8_t DS1307Read(uint8_t address, uint8_t * data);

#endif
