#include <stdio.h>
#include <wiringPi.h>
#include <wiringShift.h>
#include <stdint.h>
#include <time.h>
#include "Max6675.h"

_Max6675 Max6675;

void InitMax6675(uint8_t CE,int chan,int speed)
{
	Max6675.CE = CE;
	Max6675.chan = chan;
	Max6675.speed = speed;
	wiringPiSPISetup (Max6675.chan, Max6675.speed) ;
	pinMode(Max6675.CE , OUTPUT);
	digitalWrite(Max6675.CE, HIGH);
}

float GetThermalCouple(void)
{
  uint8_t  buff[2];
  uint16_t wRaw;
  float    wTemp;

  digitalWrite(Max6675.CE, HIGH);
  delay(500);
  digitalWrite(Max6675.CE, LOW);

  wiringPiSPIDataRW(Max6675.chan,buff,2);

  //printf("%d %d\n",buff[0],buff[1]);

  wRaw = ((uint16_t)((buff[0]<<8) | (buff[1]))&0x7FF8)>>3;
  wTemp = (float)(wRaw>>2) + (float)(wRaw&0x03)*(0.25);

  return wTemp;
}

int main()
{
	float wTemp;

	if(wiringPiSetup() == -1)
	    return -1;

	InitMax6675(0,1,100000);

	while(1)
	{
		printf("%.2f\n",GetThermalCouple());
		delay(1000);
	}

	return 0;
}