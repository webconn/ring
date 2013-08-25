#include <hard/ds1307.h>

#include <avr/io.h>
#include <util/delay.h>

#include <hard/led.h>
#include <hard/rtc.h>

void I2CInit(void)
{
	//Set up TWI Module
	TWBR = 20;
	
    //Enable the TWI Module
	TWCR|=(1<<TWEN);
}

void I2CClose(void)
{
	//Disable the module
	TWCR&=(~(1<<TWEN));
}

void I2CStart(void)
{
	//Put Start Condition on Bus
	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTA);
	//Poll Till Done
	while(!(TWCR & (1<<TWINT)));
}

void I2CStop(void)
{
	//Put Stop Condition on bus
	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO);

	//Wait for STOP to finish
	while(TWCR & (1<<TWSTO));
}

uint8_t I2CWriteByte(uint8_t data)
{
	TWDR=data;	
	
    //Initiate Transfer
	TWCR=(1<<TWEN)|(1<<TWINT);

	//Poll Till Done
	while(!(TWCR & (1<<TWINT)));

	//Check Status
	if((TWSR & 0xF8) == 0x18 || (TWSR & 0xF8) == 0x28 || (TWSR & 0xF8) == 0x40)
	{
		//SLA+W Transmitted and ACK received
		//or
		//SLA+R Transmitted and ACK received
		//or
		//DATA Transmitted and ACK recived

		return TRUE;
	}
	else{
		return FALSE;	//Error
	}
}

uint8_t I2CReadByte(uint8_t *data,uint8_t ack)
{
	//Set up ACK
	if(ack)
	{
		//return ACK after reception
		TWCR|=(1<<TWEA);
	}
	else
	{
		//return NACK after reception
		//Signals slave to stop giving more data
		//usually used for last byte read.
		TWCR&=(~(1<<TWEA));
	}

	//Now enable Reception of data by clearing TWINT
	TWCR|=(1<<TWINT);
	
    //Wait till done
	while(!(TWCR & (1<<TWINT)));

	//Check status
	if((TWSR & 0xF8) == 0x58 || (TWSR & 0xF8) == 0x50)
	{
		//Data received and ACK returned
		//	or
		//Data received and NACK returned

		//Read the data

		*data=TWDR;
		return TRUE;
	}
	else
		return FALSE;	//Error
}

uint8_t DS1307Read(uint8_t address,uint8_t *data)
{
	uint8_t res;	//result

	//Start
	I2CStart();

	//SLA+W (for dummy write to set register pointer)
	res=I2CWriteByte(0b11010000);	//DS1307 address + W

	//Error
	if(!res)	return FALSE;

	//Now send the address of required register
	res=I2CWriteByte(address);

	//Error
	if(!res)	return FALSE;

	//Repeat Start
	I2CStart();

	//SLA + R
	res=I2CWriteByte(0b11010001);	//DS1307 Address + R

	//Error
	if(!res)	return FALSE;

	//Now read the value with NACK
	res=I2CReadByte(data,0);

	//Error
	if(!res)	return FALSE;

	I2CStop();

	return TRUE;
}

/***************************************************



Function To Write Internal Registers of DS1307

---------------------------------------------



address : Address of the register

data: value to write.





Returns:

0= Failure

1= Success

***************************************************/

uint8_t DS1307Write(uint8_t address,uint8_t data)
{
	uint8_t res;	//result

	//Start
	I2CStart();

	//SLA+W
	res=I2CWriteByte(0b11010000);	//DS1307 address + W

	//Error
	if(!res)	return FALSE;

	//Now send the address of required register
	res=I2CWriteByte(address);

	//Error
	if(!res)	return FALSE;

	//Now write the value
	res=I2CWriteByte(data);

	//Error
	if(!res)	return FALSE;

	//STOP
	I2CStop();

	return TRUE;
}

uint8_t rtc_sec(void)
{
    uint8_t sec;
    DS1307Read(0, &sec);

    return (sec & 0x0F) + ((sec & 0x70) >> 4) * 10;
}

uint8_t rtc_min(void)
{
    uint8_t min;
    DS1307Read(1, &min);

    return (min & 0x0F) + ((min & 0x70) >> 4) * 10;
}

uint8_t rtc_hour(void)
{
    uint8_t hour;
    DS1307Read(2, &hour);

    return (hour & 0x0F) + ((hour & 0x30) >> 4) * 10;
}

uint8_t rtc_day(void)
{
    uint8_t day;
    DS1307Read(3, &day);

    return day - 1;
}

uint8_t rtc_read(uint8_t addr)
{
    uint8_t ret;
    DS1307Read(addr, &ret);
    return ret;
}

void rtc_write(uint8_t addr, uint8_t data)
{
    DS1307Write(addr, data);
}

uint8_t getday(void)
{
    uint8_t month, day;
    uint16_t year, y, a, m;
    
    month = rtc_read(RTC_MONTH);
    month = RTC_MONTH_U(month) * 10 + RTC_MONTH_L(month);

    day = rtc_read(RTC_DATE);
    day = RTC_DATE_U(day) * 10 + RTC_DATE_L(day);

    year = (uint16_t) rtc_read(RTC_YEAR);
    year = RTC_YEAR_U(year) * 10 + RTC_YEAR_L(year);

    a = (14 - month) / 12;
    y = year - a;
    m = month + 12 * a - 2;

    return (uint8_t) (day + y + (y>>2) - (y/100) + (y/400) + (31 * m) / 12) % 7;
}
