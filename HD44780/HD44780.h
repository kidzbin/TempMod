#define DATA 1
#define CMD  0

void HD44780_Init(uint8_t RS,uint8_t E,uint8_t DB4,uint8_t DB5,uint8_t DB6,uint8_t DB7);

void HD44780_Send(uint8_t RS,uint8_t Data8Bit);
void LCD_DisplayString(char *string_ptr);