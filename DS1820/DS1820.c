#include <stdio.h>
#include <stdint.h>
#include <wiringPi.h>
#include "DS1820.h"

#define  PIN    0

#if 0
int main(void)
{
	uint16_t wTemp;

  	if(wiringPiSetup() == -1)
    	return -1;

	InitDS1820(PIN);

	do
	{
		// if(ResetDS())
		// {
		// 	printf("Reset Good\n");
		// 	//break;
		// }	
		// else
		// {
		// 	printf("Reset Failed\n");
		// 	delayMicroseconds(10000);
		// }

		if(FetchDS(&wTemp))
		{
			printf("wTemp = %d , Temp = %.2f\n",wTemp,(float)wTemp*0.0625);
			delay(1000);
			//break;
		}	
		else
		{
			//printf(" Failed\n");
			delayMicroseconds(10000);
		}
		

	}while(1);

  	return 0x00;

}
#endif

uint8_t CRC8(uint8_t crc, uint8_t data)
{
    uint8_t j;
    crc = crc ^data;
    for (j = 0; j < 8; j++)
    {
        if ((crc & 0x01) != 0) crc = (crc >> 1) ^ 0x8c;
        else crc = crc >> 1;
    }
    return crc;
}

void InitDS1820(uint8_t ReadPin_DS1820)
{
  bReadPin_DS = ReadPin_DS1820;
}

uint8_t ResetDS(void)
{
  uint16_t wTimeOut = 2000;

  //Pull High wait for Voltage Stable
  pinMode(bReadPin_DS, OUTPUT);

  digitalWrite(bReadPin_DS, HIGH);
  delayMicroseconds(100);
  digitalWrite(bReadPin_DS, LOW);
  delayMicroseconds(500);
 
  pinMode(bReadPin_DS, INPUT);

  delayMicroseconds(60);
  do
  {

  	if( digitalRead(bReadPin_DS) == 0x00 )
  	{
  		delayMicroseconds(300); //Wait Resistor Pull High
  		return 1;
  	}

  	delayMicroseconds(1);
 
  }while(wTimeOut--);

  return 0x00;
}

void WriteDS(uint8_t bData)
{
	uint8_t i;

	pinMode(bReadPin_DS, OUTPUT);

	for(i=0;i<8;i++)
	{
		digitalWrite(bReadPin_DS, HIGH);
		delayMicroseconds(5);
		digitalWrite(bReadPin_DS, LOW);
		delayMicroseconds(10);
		
		if( bData&(0x01<<i) )
		{
			digitalWrite(bReadPin_DS, HIGH);
			delayMicroseconds(50);
		}
		else
		{
			delayMicroseconds(50);
		}
	}
}

uint8_t ReadDS(void)
{
	uint8_t i;
	uint8_t x;
	uint8_t ReadData=0x00;

	for(i=0;i<8;i++)
	{
		pinMode(bReadPin_DS, OUTPUT);

		digitalWrite(bReadPin_DS, LOW);
		delayMicroseconds(2);
		digitalWrite(bReadPin_DS, HIGH);

		pinMode(bReadPin_DS, INPUT);
		delayMicroseconds(15);

		x = digitalRead(bReadPin_DS);

		ReadData |= x<<i;

		delayMicroseconds(50);

	}
	return ReadData;
}


int FetchDS(uint16_t *uT)
{
	uint8_t w[8];
	uint8_t i;
	uint8_t crc=0;

	if( !ResetDS() ) return 0;

	WriteDS(0xCC);
	WriteDS(0x44);

	if( !ResetDS() ) return 0;

	WriteDS(0xCC);
	WriteDS(0xBE);	

	for(i=0;i<9;i++)
	{
		w[i] = ReadDS();
		//printf("%02x ",w[i]);
   	}
   	//printf("\n");
  	
  	for(i=0;i<8;i++)
  	{
    	crc=CRC8(crc,w[i]);
  	}
 
	//printf("CRC = %d\n",crc);

	*uT = (uint16_t)(w[1]<<8) | w[0] ;

	if( crc == w[8])
		return 1;
	else
		return 0;


	// if(   (*uT == 0 ) ||
	// 	  (w[4]!=127) ||
	// 	  (*uT >= 1370 )

	//   )
	// 	
	// else
	// 	return 1;
}

