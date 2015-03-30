
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wiringPi.h>
#include <wiringShift.h>
#include "PCD8544.h"

#define PCD8544_CLK 0
#define PCD8544_DIN 1
#define PCD8544_DC  2
#define PCD8544_CE  3
#define PCD8544_RST 4

int main(void)
{

  	if(wiringPiSetup() == -1)
    	return -1;

	LCDInit(PCD8544_CLK,PCD8544_DIN,PCD8544_DC,PCD8544_CE ,PCD8544_RST);

	//while(1)
{	LCDClear();

    //gotoXY(0,0);
    LCDSample();
}
	return 0;
}