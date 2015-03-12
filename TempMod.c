#include <stdio.h>
#include <stdint.h>
#include <wiringPi.h>
#include <time.h>
#include "DS1820/DS1820.h"
#include "AM2302/AM2302.h"

#define DS_PIN 0
#define AM_PIN 1

int main(void)
{
	_AM2302Data AM2302Data;
	uint16_t DSTemp;
 	uint8_t  TempStatus;
 	time_t now;
 	struct tm *local;

	InitDS1820(DS_PIN);
	InitAM2302(AM_PIN);

  	if(wiringPiSetup() == -1)
    	return -1;

	do
	{
		time(&now);
		local = localtime(&now);
		TempStatus = 0x00;
		if(FetchDS(&DSTemp))
			TempStatus |= 0x01;

		if(FetchAM2303(&AM2302Data))
			TempStatus |= 0x02;

		if(TempStatus == 0x03)
		{
			printf("DS1820 Temp = %.2fC , AM2302 Room=%.2fC,Humi=%.2f%c  %2d:%2d:%2d\n" ,(float)DSTemp*0.0625 , (float)AM2302Data.Room/10 ,(float)AM2302Data.Humidity/10,37,local->tm_hour,local->tm_min,local->tm_sec );
			delay(1000);
		}
		else
		{
			delay(300);
		}

	}while(1);

  	return 0x00;
}