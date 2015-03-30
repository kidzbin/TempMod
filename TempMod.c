#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>     /* strtof */
#include <wiringPi.h>
#include <time.h>
#include <sqlite3.h>
#include "DS1820/DS1820.h"
#include "AM2302/AM2302.h"
#include "Max6675/Max6675.h"

#define AM_PIN 0
#define MAX_CE 1
#define TOGGLE 7

#define PCD8544_CLK 2
#define PCD8544_DIN 3
#define PCD8544_DC  4
#define PCD8544_CE  5
#define PCD8544_RST 6

float Last30Temp=0.0;

typedef int (*sqlite3_callback)(
	void*,    /* Data provided in the 4th argument of sqlite3_exec() */
	int,      /* The number of columns in row */
	char**,   /* An array of strings representing fields in the row */
	char**    /* An array of strings representing column names */
);

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
   int i;
   char cStr[10];
   float a;
   for(i=1; i<argc; i++)
   {
   		sprintf(cStr,"%s",argv[i]);
   		//printf("%s ",cStr);
   		Last30Temp = strtof(cStr,NULL);
     	//printf("%s = %s(%.2f)\n", azColName[i], argv[i] ? argv[i] : "NULL",Last30Temp);
   }
   //printf("\n");
   return 0;
}

// #create table roast( Roasting INTEGER,
// #                    TimeStamp INTEGER DEFAULT (datetime('now','localtime')),
// #		     		Max6675 REAL ,
// # 	             	AM_Humi REAL,
// # 	             	AM_Room REAL);

int main(void)
{
	_AM2302Data AM2302Data;
	float wRoastTemp;
 	time_t now,t1,t2;
 	struct tm *local;
 	sqlite3 *db;
 	int rc;
 	char *errMsg = NULL;
 	char InsertSQL[200];
 	char LCDStr[12],TempStatus;
 	char TimeStr[14];
 	float DeltaT;
 	uint8_t TimeFlag=0,hh,mm;
 	const char* data = "Callback function called";

 	rc = sqlite3_open("roastlog.db", &db);

 	if(rc) 
 	{
 		printf("Open DB Error\n");
 		return 0;
 	}

  	if(wiringPiSetup() == -1)
    	return -1;

	InitAM2302(AM_PIN);
	InitMax6675(MAX_CE,1,100000);
  	pinMode(TOGGLE, INPUT);
  	
  	time(&now);
	local = localtime(&now);	
  	
  	sprintf(TimeStr,"%4d%02d%02d%02d%02d%02d\n",local->tm_year+1900,local->tm_mon+1,local->tm_mday,local->tm_hour,local->tm_min,local->tm_sec);
	sprintf(InsertSQL,"CREATE TABLE roast_%s(Roasting INTEGER,TimeStamp INTEGER DEFAULT (datetime('now','localtime')),Max6675 REAL ,AM_Humi REAL,AM_Room REAL);",TimeStr);
	rc = sqlite3_exec(db, InsertSQL, 0, 0, &errMsg);

	LCDInit(PCD8544_CLK,PCD8544_DIN,PCD8544_DC,PCD8544_CE ,PCD8544_RST);

	do
	{
		if(digitalRead(TOGGLE)==0x00 && TimeFlag==0x00)
		{
			//start count
			time(&t1);
			TimeFlag |= 0x01;
		}
		time(&now);
		local = localtime(&now);

		if(FetchAM2303(&AM2302Data))
			TempStatus |= 0x03;

		wRoastTemp = GetThermalCouple();

		if(TempStatus == 0x03)
		{
			printf("%d Roast = %.2f RT=%.2fC,Humi=%.2f%c %2d:%2d:%2d " ,digitalRead(TOGGLE), wRoastTemp ,(float)AM2302Data.Room/10 ,(float)AM2302Data.Humidity/10,37,local->tm_hour,local->tm_min,local->tm_sec );
			sprintf(InsertSQL,"INSERT INTO roast_%s(Roasting,Max6675,AM_Humi,AM_Room) VALUES(%d,%.2f,%.2f,%.2f)",TimeStr,digitalRead(TOGGLE),wRoastTemp,(float)AM2302Data.Humidity/10,(float)AM2302Data.Room/10);
			rc = sqlite3_exec(db, InsertSQL, 0, 0, &errMsg);
			if(rc) printf("SQL Error\n");
			//printf("%s\n",InsertSQL);
			LCDClear();

       		gotoXY(0,0);
       		sprintf(LCDStr,"   %02d:%02d:%02d ",local->tm_hour,local->tm_min,local->tm_sec);
       		LcdString(LCDStr);

       		if(TimeFlag)
       		{
       			time(&t2);
       			t2 = t2-t1;
	       		gotoXY(0,1);

	       		hh = t2/60;
	       		mm = t2%60;

    	   		sprintf(LCDStr,"Timer:%02d:%02d",hh,mm);
       			LcdString(LCDStr);
       		}

       		gotoXY(0,2);
       		sprintf(LCDStr,"Roast:%3.1fC",wRoastTemp);
       		LcdString(LCDStr);

       		gotoXY(0,3);
       		sprintf(LCDStr,"RT   :%.1fC",(float)AM2302Data.Room/10);
       		LcdString(LCDStr);      		

       		gotoXY(0,4);
       		sprintf(LCDStr,"H    :%.1f%c  ",(float)AM2302Data.Humidity/10,0x25);
       		LcdString(LCDStr);  

       		sprintf(InsertSQL,"select TimeStamp,Max6675 from roast_%s where strftime('%%s',TimeStamp) > strftime('%%s', 'now','localtime','-30 seconds') limit 1",TimeStr);
			//printf("%s\n",InsertSQL);
			rc = sqlite3_exec(db, InsertSQL, callback, (void*)data, &errMsg);
			if(rc) printf("SQL Error\n");

			DeltaT = wRoastTemp - Last30Temp;

			printf(" Delta T = %.2f ",DeltaT*2);
			if(TimeFlag) printf(" Timer:%02d:%02d",hh,mm);

			printf("\n");

			delay(500);
		}
		else
		{
			delay(200);
		}
	}while(1);

	sqlite3_close(db); 

  	return 0x00;
}