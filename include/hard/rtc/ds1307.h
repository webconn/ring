#ifndef RTC_DS1307_H
#define RTC_DS1307_H

#define RTC_SEC 0
#define RTC_MIN 1
#define RTC_HOUR 2
#define RTC_DAY 3
#define RTC_DATE 4
#define RTC_MONTH 5
#define RTC_YEAR 6

#define RTC_SEC_U(data) ((data >> 4) & 0x07)
#define RTC_SEC_L(data) (data & 0x0F)
#define RTC_MIN_U(data) ((data >> 4) & 0x07)
#define RTC_MIN_L(data) (data & 0x0F)
#define RTC_HOUR_U(data) ((data >> 4) & 0x03)
#define RTC_HOUR_L(data) (data & 0x0F)
#define RTC_DAY_L(data) (data & 0x07)
#define RTC_DATE_U(data) ((data >> 4) & 0x03)
#define RTC_DATE_L(data) (data & 0x0F)
#define RTC_MONTH_U(data) ((data >> 4) & 0x01)
#define RTC_MONTH_L(data) (data & 0x0F)
#define RTC_YEAR_U(data) ((data >> 4) & 0x0F)
#define RTC_YEAR_L(data) (data & 0x0F)

#endif
