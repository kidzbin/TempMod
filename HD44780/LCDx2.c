
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wiringPi.h>
#include <lcd.h>

#define LCD_RS  0               //Register select pin
#define LCD_E   1               //Enable Pin
#define LCD_D4  2               //Data pin 4
#define LCD_D5  3               //Data pin 5
#define LCD_D6  5               //Data pin 6
#define LCD_D7  4               //Data pin 7

int main(void)
{
	int lcd;                //Handle for LCD

  	if(wiringPiSetup() == -1)
    	return -1;

	lcdInit (2, 16,4, LCD_RS, LCD_E ,LCD_D4 , LCD_D5, LCD_D6,LCD_D7,0,0,0,0);

	while(1)
	{
	lcdPosition(lcd,0,0);
	lcdPuts(lcd, "Character LCD");
		 delayMicroseconds(500);
	}



	return 0;
}