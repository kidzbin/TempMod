#include <stdio.h>
#include <stdint.h>
#include <wiringPi.h>
#include "AM2302.h"

#define  PIN    1

#if 0
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
#endif

void InitAM2302(uint8_t bReadPin)
{
	bReadPin_AM = bReadPin;
}

uint8_t Waiting(uint8_t Signal)
{
  int bCnt=0x00;

  while( digitalRead(bReadPin_AM) == Signal )
  {
      bCnt++;
      delayMicroseconds(1);
      if(bCnt > 254) break;
  }

  return bCnt;
}

uint8_t FetchAM2303(_AM2302Data * AM2302Data)
{
	uint8_t rawus[40];
	uint8_t Data[5];
 	uint8_t max=0x00,min=0xFF,mid;
 	uint8_t i,j,*pt,sum;

  pinMode(bReadPin_AM, OUTPUT);
  digitalWrite(bReadPin_AM, HIGH);
  delayMicroseconds(10);
  digitalWrite(bReadPin_AM, LOW);
  delayMicroseconds(1000);
  digitalWrite(bReadPin_AM, HIGH);
  delayMicroseconds(30);
  pinMode(bReadPin_AM, INPUT);

	if( Waiting(LOW)  == 255 ) return 0;
	if( Waiting(HIGH) == 255 ) return 0;

  for( i=0 ; i< 40; i++ )
  {
   	Waiting(LOW);
   	rawus[i]=Waiting(HIGH);
 	  if(rawus[i]>max) max = rawus[i] ;
  	if(rawus[i]<min) min = rawus[i] ;    	
  }
 
  mid = ( max + min ) / 2;

  pt = rawus;
	for( i=0 ; i< 5; i++ )
  {
  	Data[i]=0;
  	for(j=0;j<8;j++)
  	{
  		if(*pt>mid)
  			Data[i] |= 0x01<<( 7 - (j&0x07) );		
  		pt++;
  	}
  }

  // for( i=0 ; i< 5; i++ )
  // 	printf("%x ",Data[i]);

  // printf("\n");

  sum=0;
  for(i=0;i<4;i++)
  	sum+=Data[i];

  if( sum != Data[4]) return 0;

	AM2302Data->Humidity =  (uint16_t)(Data[0]<<8 | Data[1]);
	AM2302Data->Room     =  (uint16_t)(Data[2]<<8 | Data[3]);

  if(AM2302Data->Humidity >999     ) return 0;
  if(AM2302Data->Room     >850     ) return 0;

  	return 1;
}