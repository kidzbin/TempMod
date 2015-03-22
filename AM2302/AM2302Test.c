#include <stdio.h>
#include <stdint.h>
#include <wiringPi.h>
#include "AM2302.h"

#define  PIN    0

int main(void)
{
 	_AM2302Data AM2302Data;

  	if(wiringPiSetup() == -1)
    	return -1;

	InitAM2302(PIN);
 
	do
	{

		if(FetchAM2303(&AM2302Data))
		{
			printf("Room = %.1f , Humi = %.1f\n",(float)AM2302Data.Room/10,(float)AM2302Data.Humidity/10 );
			delay(1000);
			//break;
		}	
		else
		{
			//printf(" Failed\n");
			delay(200);
		}

	}while(1);

}
