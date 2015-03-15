
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wiringPi.h>
#include <wiringShift.h>
#include "PCD8544.h"

// COnfiguration for the LCD
#define LCD_C     LOW
#define LCD_D     HIGH
#define LCD_CMD   0

#define  DEBUG 0

// LCD pins      Raspberry Pi
// LCD1 - GND    P06  - GND
// LCD2 - VCC    P01 - 3.3V
// LCD3 - CLK    P11 - GPIO0
// LCD4 - Din    P12 - GPIO1
// LCD5 - D/C    P13 - GPIO2
// LCD6 - CS     P15 - GPIO3
// LCD7 - RST    P16 - GPIO4
// LCD8 - LED    P01 - 3.3V 

//delayMicroseconds(30);

#define LCDWIDTH  48
#define LCDHEIGHT 84

typedef struct 
{
	uint8_t CLK;
	uint8_t DIN;
	uint8_t DC;
	uint8_t CE;
	uint8_t RST;
}_LCDCtrl;


_LCDCtrl LCDCtrl;


void gotoXY(uint8_t x, uint8_t y)
{
  SendLCD( 0, 0x80 | x);  // Column.
  SendLCD( 0, 0x40 | y);  // Row.
}


int LCDInit(void)
{
	LCDCtrl.CLK = 2;
	LCDCtrl.DIN = 3;
	LCDCtrl.DC  = 4;
	LCDCtrl.CE  = 5;
	LCDCtrl.RST = 6;

	pinMode(LCDCtrl.CLK , OUTPUT);
	pinMode(LCDCtrl.DIN , OUTPUT);
	pinMode(LCDCtrl.DC  , OUTPUT);
	pinMode(LCDCtrl.CE  , OUTPUT);
	pinMode(LCDCtrl.RST , OUTPUT);

	digitalWrite(LCDCtrl.CE , LOW);
	digitalWrite(LCDCtrl.RST, LOW);
	digitalWrite(LCDCtrl.RST, HIGH);
	digitalWrite(LCDCtrl.CE , HIGH);

	SendLCD(CMD,0x21);
	SendLCD(CMD,0xB2);
	SendLCD(CMD,0x04);
	SendLCD(CMD,0x14);
	SendLCD(CMD,0x0C);

  	SendLCD(LOW, 0x20);
  	SendLCD(LOW, 0x0C);

  	LCDClear();

	return 0;
}

int LCDClear(void)
{
	uint16_t i;
	for ( i = 0; i < LCDWIDTH*LCDHEIGHT/8 ; i++)
		SendLCD(DATA,0x00);

}

void SendLCD(uint8_t bMode,uint8_t bData)
{
	uint8_t i;

	digitalWrite(LCDCtrl.DC , bMode);
	digitalWrite(LCDCtrl.CE , LOW);
	//shiftOut(LCDCtrl.DIN,LCDCtrl.CLK, MSBFIRST, bData) ;
	
	i=0x08;
	do
	{
		i--;
		// printf("%d ",(bData>>i)&0x01);
		digitalWrite(LCDCtrl.DIN , (bData>>i)&0x01 );
		delayMicroseconds(5);
		digitalWrite(LCDCtrl.CLK , HIGH);
		delayMicroseconds(5);
		digitalWrite(LCDCtrl.CLK , LOW );
		delayMicroseconds(5);

	}while(i);	


	digitalWrite(LCDCtrl.CE , HIGH);

#if DEBUG
	printf("%2x ",bData);

	i=0x08;
	do
	{
		i--;
		printf("%d ",(bData>>i)&0x01);
	}while(i);

	printf("\n");
#endif
}

void LcdCharacter(char character)
{
	uint8_t index;
  	SendLCD(LCD_D, 0x00);
  	for (index = 0; index < 5; index++)
  	{
    	SendLCD(LCD_D, ASCII[character - 0x20][index]);
  	}
  	SendLCD(LCD_D, 0x00);
}

void LcdString(char *characters)
{
  while (*characters)
  {
    LcdCharacter(*characters++);
  }
}

void LCDSample(void)
{
	LCDClear();
 	gotoXY(7,1);
  	LcdString("Nokia 5110");
  	gotoXY(4,2);
  	LcdString("Scroll Demo");

}