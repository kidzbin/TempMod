
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>  
#include <wiringPi.h>
#include <time.h>
#include <sqlite3.h>
#include "PCD8544/PCD8544.h"
#include "AM2302/AM2302.h"

#define PCD8544_CLK 0
#define PCD8544_DIN 1
#define PCD8544_DC  2
#define PCD8544_CE  3
#define PCD8544_RST 4

#define AM_PIN      5

// #create table HomeLogger( TimeStamp INTEGER DEFAULT (datetime('now','localtime')),
// # 	             	AM_Humi REAL,
// # 	             	AM_Room REAL);

int main(void)
{
	_AM2302Data AM2302Data[10];
	time_t now;
 	struct tm *local;
 	char TimeStr[14];
 	char LCDStr[12];
 	uint8_t bCnt,i;
 	uint16_t Room,Humi;
 	unsigned int start,end; //miliis

 	sqlite3 *db;
 	int rc;
 	char *errMsg = NULL;
 	char InsertSQL[200];

  	if(wiringPiSetup() == -1)
    	return -1;

    InitAM2302(AM_PIN);
	LCDInit(PCD8544_CLK,PCD8544_DIN,PCD8544_DC,PCD8544_CE ,PCD8544_RST);
	LCDClear();

	rc = sqlite3_open("/home/pi/TempMod/HomeLogger.db", &db);

	while(1)
	{
		start = millis();
		bCnt = 0x00;
		while( ((millis()-start) < 1000) && (bCnt<10) )
		{
			while( FetchAM2303(&AM2302Data[bCnt]) )
			{
				bCnt++;
				//printf("RT=%.2fC,Humi=%.2f%c %2d:%2d:%2d " ,(float)AM2302Data[bCnt].Room/10 ,(float)AM2302Data[bCnt].Humidity/10,37,local->tm_hour,local->tm_min,local->tm_sec );
			}
		}

		if(bCnt==0) continue;

		time(&now);
		local = localtime(&now);
		//printf("RT=%.2fC,Humi=%.2f%c %2d:%2d:%2d " ,(float)AM2302Data.Room/10 ,(float)AM2302Data.Humidity/10,37,local->tm_hour,local->tm_min,local->tm_sec );
		gotoXY(0,0);

       	sprintf(LCDStr,"   %02d:%02d:%02d ",local->tm_hour,local->tm_min,local->tm_sec);
       	LcdString(LCDStr);

		gotoXY(0,2);

		Room=0;
		Humi=0;

		for(i=0;i<bCnt;i++)
		{
			Room+=AM2302Data[i].Room;
			Humi+=AM2302Data[i].Humidity;
		}

       	sprintf(LCDStr," RT  =%.1fC",(float)(Room)/10/bCnt);
       	LcdString(LCDStr);

		gotoXY(0,3);
    			
       	sprintf(LCDStr," Humi=%.1f%c",(float)(Humi)/10/bCnt,37);
       	LcdString(LCDStr);

       	sprintf(InsertSQL,"INSERT INTO HomeLogger(AM_Room,AM_Humi) VALUES(%.1f,%.1f)",(float)(Room)/10/bCnt,(float)(Humi)/10/bCnt);
       	rc = sqlite3_exec(db, InsertSQL, 0, 0, &errMsg);
	}

	sqlite3_close(db); 

	return 0;
}