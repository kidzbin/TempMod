#define POLY  0x131


uint8_t CRC8(uint8_t crc, uint8_t data);
void InitDS1820(uint8_t ReadPin_DS1820);
uint8_t ResetDS(void);
void WriteDS(uint8_t bData);
uint8_t ReadDS(void);
int FetchDS(uint16_t *uT);

uint8_t bReadPin_DS;