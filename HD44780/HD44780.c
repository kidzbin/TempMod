
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wiringPi.h>
#include "HD44780.h"


typedef struct 
{
	uint8_t RS;
	uint8_t E;
	uint8_t DB[4];
}_HD44780;

_HD44780 HD44780;

void HD44780_Init(uint8_t RS,uint8_t E,uint8_t DB4,uint8_t DB5,uint8_t DB6,uint8_t DB7)
{
	HD44780.RS = RS;
	HD44780.E  = E;
	HD44780.DB[0] = DB4;
	HD44780.DB[1] = DB5;
	HD44780.DB[2] = DB6;
	HD44780.DB[3] = DB7;

	pinMode(HD44780.RS    , OUTPUT);
	pinMode(HD44780.E     , OUTPUT);
	pinMode(HD44780.DB[0] , OUTPUT);
	pinMode(HD44780.DB[1] , OUTPUT);
	pinMode(HD44780.DB[2] , OUTPUT);
	pinMode(HD44780.DB[3] , OUTPUT);

	delayMicroseconds(6000);

	HD44780_Send(CMD,0x02);
	
	HD44780_Send(CMD,0x28);
	HD44780_Send(CMD,0x0E);

	HD44780_Send(CMD,0x01);
	HD44780_Send(CMD,0x80);

	digitalWrite(HD44780.E     , LOW);


}

void HD44780_Send(uint8_t RS,uint8_t Data8Bit)
{
	uint8_t i,j;
	uint8_t nibble[2];

	nibble[0] = Data8Bit>>4;
	nibble[1] = Data8Bit&0x0F;

	for(j=0;j<2;j++)
	{
		digitalWrite(HD44780.RS    , RS);
		for(i=0;i<4;i++)
			digitalWrite(HD44780.DB[i] , (nibble[j]>>i)&0x01 );
		digitalWrite(HD44780.E     , HIGH);
		delayMicroseconds(15);
		digitalWrite(HD44780.E     , LOW);
		delayMicroseconds(50);
	}
	delayMicroseconds(1000);
}

void LCD_DisplayString(char *string_ptr)
{
   while(*string_ptr)
    HD44780_Send(DATA,*string_ptr++);
}