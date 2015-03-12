#include <stdio.h>
#include <stdint.h>
#include <wiringPi.h>
#include <time.h>
#include <sqlite3.h>
#include "DS1820/DS1820.h"
#include "AM2302/AM2302.h"

#define DS_PIN 0
#define AM_PIN 1

// #create table Home(ID INTEGER PRIMARY KEY AUTOINCREMENT, 
// #                    TimeStamp INTEGER DEFAULT (datetime('now','localtime')),
// #		     		DS_Temp REAL ,
// # 	             	AM_Humi REAL,
// # 	             	AM_Room REAL);


int main(void)
{
	_AM2302Data AM2302Data;
	uint16_t DSTemp;
 	uint8_t  TempStatus;
 	time_t now;
 	struct tm *local;
 	sqlite3 *db;
 	int rc;
 	char *errMsg = NULL;
 	char InsertSQL[128];

 	rc = sqlite3_open("HomeTemp.db", &db);
 	
 	if(rc) 
 	{
 		printf("Error\n");
 		return 0;
 	}

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
			sprintf(InsertSQL,"INSERT INTO Home (DS_Temp,AM_Humi,AM_Room) VALUES(%.2f,%.1f,%.1f);" , (float)DSTemp*0.0625,(float)AM2302Data.Humidity/10,(float)AM2302Data.Room/10);
			//printf("%s\n",InsertSQL);
			rc = sqlite3_exec(db, InsertSQL, 0, 0, &errMsg);
			delay(3000);
		}
		else
		{
			delay(750);
		}
	}while(1);

	sqlite3_close(db); 

  	return 0x00;
}