#include <stdio.h>
#include <stdint.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <time.h>
#include <sqlite3.h>
#include "AM2302.h"

#define  PIN    5

int main(void)
{
 	_AM2302Data AM2302Data[10];
	time_t now;
 	struct tm *local;
 	unsigned int startus,endus;

 	uint8_t bCnt,bfailedCnt;

  	if(wiringPiSetup() == -1)
    	return -1;

	InitAM2302(PIN);

	startus = millis();

	do
	{
		bCnt = 0x00;
		bfailedCnt = 0x00;
		while( ((millis()-startus)) < 1000 && (bCnt<10) )
		{
			if(FetchAM2303(&AM2302Data[bCnt]))
				bCnt++;
			bfailedCnt++;
		}

		printf("%d %d\n",bCnt,bfailedCnt);
		startus = millis();

	}while(1);
}
