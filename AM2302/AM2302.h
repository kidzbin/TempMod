#define  LOW    0
#define  HIGH   1


typedef struct 
{
	uint16_t Room;
	uint16_t Humidity;
}_AM2302Data;

uint8_t bReadPin_AM;

void InitAM2302(uint8_t bReadPin);
uint8_t Waiting(uint8_t Signal);
uint8_t FetchAM2303(_AM2302Data * AM2302Data);
// uint8_t FetchAM2303One(_AM2302Data * AM2302Data);