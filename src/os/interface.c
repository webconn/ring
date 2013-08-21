#include <os/interface.h>

void TInterface_standby(void);
void TInterface_menu(void);

void TInterface_set_time(void);
void TInterface_set_brightness(void);

void TInterface_init(void)
{
    iflags.timer = (eeprom_read_byte((uint8_t *) 511) & 0x80) >> 7; // read Timer flag from memory
    iflags.brightness = eeprom_read_byte((uint8_t *) 511) & 0x0F;
    display_setbrgh((uint8_t) iflags.brightness);
    // add Standby screen to work
    TScreen_set_handler(&TInterface_standby);
    if(iflags.timer)
        led_on();
}

void toggle_timer(void)
{
    iflags.timer = !iflags.timer;
    uint8_t temp = eeprom_read_byte((uint8_t *) 511);
    if(iflags.timer)
    {
        temp |= 1<<7;
        led_on();
    }
    else
    { 
        temp &= ~(1<<7);
        led_off();
    }
    eeprom_write_byte((uint8_t *) 511, temp);
    iflags.clear = 0;
}

void goto_menu(void)
{
    iflags.clear = 0;
    TScreen_unset_cursor();
    TScreen_set_handler(&TInterface_menu);
}

void TInterface_standby(void)
{
    if(!iflags.clear)
    {
        // set buttons handlers
        TButtons_add_handler(BUT_OK, &goto_menu);
        TButtons_add_handler(BUT_NO, &toggle_timer);
        iflags.clear = 1;
    }

    uint8_t data;
    // combine strings
    // 1st string is 'as is' in every mode
    data = rtc_day();
    
    line1[0] = ' ';
    line1[1] = ' ';

    line1[2] = str_days[data][0];
    line1[3] = str_days[data][1];

    line1[4] = ' ';

    data = rtc_read(RTC_DATE);
    line1[5] = RTC_DATE_U(data) + 0x30;
    line1[6] = RTC_DATE_L(data) + 0x30;
    line1[7] = '.';
    
    data = rtc_read(RTC_MONTH);
    line1[8] = RTC_MONTH_U(data) + 0x30;
    line1[9] = RTC_MONTH_L(data) + 0x30;
    line1[10] = '.';

    data = rtc_read(RTC_YEAR);
    line1[11] = RTC_YEAR_U(data) + 0x30;
    line1[12] = RTC_YEAR_L(data) + 0x30;

    line1[13] = ' ';
    line1[14] = ' ';
    line1[15] = ' ';

    // 2nd line should change
    if(iflags.timer) // if timer enabled
    {
        data = rtc_read(RTC_HOUR);
        line2[0] = RTC_HOUR_U(data) + 0x30;
        line2[1] = RTC_HOUR_L(data) + 0x30;
        line2[2] = ':';

        data = rtc_read(RTC_MIN);
        line2[3] = RTC_MIN_U(data) + 0x30;
        line2[4] = RTC_MIN_L(data) + 0x30;
        line2[5] = ':';

        data = rtc_read(RTC_SEC);
        line2[6] = RTC_SEC_U(data) + 0x30;
        line2[7] = RTC_SEC_L(data) + 0x30;
        line2[8] = ' ';

        line2[9] = SYM_BELL;

        line2[10] = ' ';
        if(tt_el[0] != 255)
        {
            line2[11] = (tt_el[0] / 10) + 0x30;
            line2[12] = (tt_el[0] % 10) + 0x30;
            line2[13] = ':';
            line2[14] = (tt_el[1] / 10) + 0x30;
            line2[15] = (tt_el[1] % 10) + 0x30;
        }
        else
        {
            line2[11] = '-';
            line2[12] = '-';
            line2[13] = ':';
            line2[14] = '-';
            line2[15] = '-';
        }
    }
    else // if not
    {
        line2[0] = ' ';
        line2[1] = ' ';
        line2[2] = ' ';
        line2[3] = ' ';

        data = rtc_read(RTC_HOUR);
        line2[4] = RTC_HOUR_U(data) + 0x30;
        line2[5] = RTC_HOUR_L(data) + 0x30;
        line2[6] = ':';
        
        data = rtc_read(RTC_MIN);
        line2[7] = RTC_MIN_U(data) + 0x30;
        line2[8] = RTC_MIN_L(data) + 0x30;
        line2[9] = ':';

        data = rtc_read(RTC_SEC);
        line2[10] = RTC_SEC_U(data) + 0x30;
        line2[11] = RTC_SEC_L(data) + 0x30;

        line2[12] = ' ';
        line2[13] = ' ';
        line2[14] = ' ';
        line2[15] = ' ';
    }
    
}

uint8_t point = 0;

void goto_standby(void)
{
    iflags.clear = 0;
    TScreen_set_handler(&TInterface_standby);
}

void menu_decr(void)
{
    if(point != 0) point--;
}

void menu_incr(void)
{
    if(point != 2) point++;
}

void goto_app(void)
{
    iflags.clear = 0;
    if(point == 0)
        TScreen_set_handler(&TInterface_set_time);
    else if(point == 1)
        TScreen_set_handler(&TInterface_set_brightness);
    else if(point == 2)
        sys_reset();
}

void TInterface_menu(void)
{
    if(!iflags.clear)
    {
        TButtons_add_handler(BUT_BACK, &menu_decr);
        TButtons_add_handler(BUT_FWD, &menu_incr);
        TButtons_add_handler(BUT_OK, &goto_app);
        TButtons_add_handler(BUT_NO, &goto_standby);
    }

    uint8_t i;
    for(i=0; i!=13; i++)
    {
        line1[i] = str_config[i];
    }

    line1[13] = point + 0x31;
    line1[14] = '/';
    line1[15] = 3 + 0x30;

    if(point == 0) // 1st point: set time
    {
        for(i=0; i!=16; i++)
            line2[i] = str_time[i];
    }
    else if(point == 1) // 2nd point: set brightness
    {
        for(i=0; i!=16; i++)
            line2[i] = str_brightness[i];
    }
    else if(point == 2) // 3rd point: reboot
    {
        for(i=0; i!=16; i++)
            line2[i] = str_reboot[i];
    }
}

uint8_t addr = 0;
uint8_t time[10];

void addr_decr(void)
{
    if(addr) addr--;
}

void addr_incr(void)
{
    if(addr != 9) addr++;
}

void digit_incr(void)
{
    if(addr & 1 || addr == 4)
    {
        if(time[addr] == 9) time[addr] = 0;
        else time[addr]++;
    }
    else if(addr == 0)
    {
        if(time[addr] == 3) time[addr] = 0;
        else time[addr]++;
    }
    else if(addr == 2)
    {
        if(time[addr] == 1) time[addr] = 0;
        else time[addr] = 1;
    }
    else if(addr == 6)
    {
        if(time[addr] == 2) time[addr] = 0;
        else time[addr]++;
    }
    else if(addr == 8)
    {
        if(time[addr] == 5) time[addr] = 0;
        else time[addr]++;
    }
        
}

void set_time(void)
{
    uint8_t temp;

    // 0. recalibrate clock if possible
    uint16_t old_timestamp = time_getCalTimestamp();
    uint16_t current_timestamp = time_createTimestamp(time[0] * 10 + time[1], time[2] * 10 + time[3], time[4] * 10 + time[5]);
    if(old_timestamp != 0 && current_timestamp != old_timestamp)
    {
        // 1. Get time movement
        // TODO: compare RTC and required date, calibrate only in case when they are equal
        
    }
    
    // save calibration timestamp - only when date doesn't change
    time_saveCalTimestamp(current_timestamp);

    // 1. set date
    temp = time[1] + (time[0] << 4);
    rtc_write(RTC_DATE, temp);

    // 2. set month
    temp = time[3] + (time[2] << 4);
    rtc_write(RTC_MONTH, temp);

    // 3. set year
    temp = time[5] + (time[4] << 4);
    rtc_write(RTC_YEAR, temp);

    // 4. set hours
    temp = time[7] + (time[6] << 4);
    rtc_write(RTC_HOUR, temp);

    // 5. set minutes
    temp = time[9] + (time[8] << 4);
    rtc_write(RTC_MIN, temp);

    // 6. clear seconds
    rtc_write(RTC_SEC, 0);

    // 7. calculate day
    rtc_write(RTC_DAY, getday());

    TScreen_unset_cursor();
    iflags.clear = 0;
    sys_reset();
    //TScreen_set_handler(&TInterface_menu);
}

void TInterface_set_time(void)
{
    // 1st string is message
    uint8_t i;
    for(i=0; i!=16; i++)
        line1[i] = str_time[i];

    if(!iflags.clear)
    {
        iflags.clear = 1;
        TButtons_add_handler(BUT_BACK, &addr_decr);
        TButtons_add_handler(BUT_FWD, &addr_incr);
        TButtons_add_handler(BUT_OK, &digit_incr);
        TButtons_add_handler(BUT_NO, &set_time);

        // read time
        time[0] = RTC_DATE_U(rtc_read(RTC_DATE));
        time[1] = RTC_DATE_L(rtc_read(RTC_DATE));
        time[2] = RTC_MONTH_U(rtc_read(RTC_MONTH));
        time[3] = RTC_MONTH_L(rtc_read(RTC_MONTH));
        time[4] = RTC_YEAR_U(rtc_read(RTC_YEAR));
        time[5] = RTC_YEAR_L(rtc_read(RTC_YEAR));
        time[6] = RTC_HOUR_U(rtc_read(RTC_HOUR));
        time[7] = RTC_HOUR_L(rtc_read(RTC_HOUR));
        time[8] = RTC_MIN_U(rtc_read(RTC_MIN));
        time[9] = RTC_MIN_L(rtc_read(RTC_MIN));
    }
    
    line2[0] = ' ';
    line2[1] = time[0] + 0x30;
    line2[2] = time[1] + 0x30;
    line2[3] = '.';
    line2[4] = time[2] + 0x30;
    line2[5] = time[3] + 0x30;
    line2[6] = '.';
    line2[7] = time[4] + 0x30;
    line2[8] = time[5] + 0x30;
    line2[9] = ' ';
    line2[10] = time[6] + 0x30;
    line2[11] = time[7] + 0x30;
    line2[12] = ':';
    line2[13] = time[8] + 0x30;
    line2[14] = time[9] + 0x30;
    line2[15] = ' ';

    TScreen_set_cursor(0x40 + 1 + 3*(addr>>1) + (addr & 1)); // math magic :)

}

void set_brght(void)
{
    uint8_t temp = eeprom_read_byte((uint8_t *) 511);
    temp &= 0xF0;
    temp |= iflags.brightness;

    eeprom_write_byte((uint8_t *) 511, temp);

    iflags.clear = 0;
    TScreen_set_handler(&TInterface_menu);
}

void brgh_decr(void)
{
    if(iflags.brightness) iflags.brightness--;
    display_setbrgh(iflags.brightness);
}

void brgh_incr(void)
{
    if(iflags.brightness != 8) iflags.brightness++;
    display_setbrgh(iflags.brightness);
}

void TInterface_set_brightness(void)
{
    uint8_t i;
    for(i=0; i!=16; i++)
        line1[i] = str_brightness[i];

    if(!iflags.clear)
    {
        iflags.clear = 1;
        TButtons_add_handler(BUT_BACK, &brgh_decr);
        TButtons_add_handler(BUT_FWD, &brgh_incr);
        TButtons_add_handler(BUT_NO, &set_brght);
        TButtons_add_handler(BUT_OK, &set_brght);
    }

    line2[0] = ' ';
    line2[1] = 0xA9;
    line2[2] = 'p';
    line2[3] = ':';
    line2[4] = ' ';

    for(i=0; i!=8; i++)
        line2[5+i] = ' ';

    for(i=0; i!=iflags.brightness; i++)
        line2[5+i] = SYM_FILL;

    line2[13] = ' ';
    line2[14] = iflags.brightness + 0x30;
    line2[15] = ' ';
}
