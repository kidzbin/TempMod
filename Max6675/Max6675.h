
typedef struct
{
	uint8_t CE;
	int     chan;
	int     speed;
}_Max6675;

void InitMax6675(uint8_t CE,int chan,int speed);
float GetThermalCouple(void);
