#include <hard/time.h>

uint16_t time_createTimestamp(uint16_t d, uint8_t m, uint8_t y)
{
    // Count years
    d += (365 * y) + (y >> 2) + 1;

    // If current year is leap, we need to add one in case of last february
    if(!(y & 3) && m < 3)
        d--;

    // Count months
    while(--m > 0)
    {
        if(m == 2) // feb
            d += 28;
        else if(m < 8)
        {
            if(m & 1)
                d += 31;
            else
                d += 30;
        }
        else
        {
            if(m & 1)
                d += 30;
            else
                d += 31;
        }
    }
    
    return d;
}

void time_saveCalTimestamp(uint16_t val)
{
    rtc_write(0x10, val >> 8);
    rtc_write(0x11, val & 0xFF);
}

uint16_t time_getCalTimestamp(void)
{   
    return (uint16_t) (rtc_read(0x10) << 8) | rtc_read(0x11);
}

void time_saveCalibration(uint8_t val)
{
    rtc_write(0x12, val);
}

uint8_t time_getCalibration(void)
{
    return rtc_read(0x12);
}

void time_saveMoveTimestamp(uint16_t val)
{
    rtc_write(0x13, val >> 8);
    rtc_write(0x14, val & 0xFF);
}

uint16_t time_getMoveTimestamp(void)
{
    return (uint16_t) (rtc_read(0x13) << 8) | rtc_read(0x14);
}
