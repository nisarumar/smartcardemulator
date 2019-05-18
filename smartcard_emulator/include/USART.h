#ifndef USART_H_
#define USART_H_

#define BAUD_VALUE_L	(uint8_t)	30
#define BAUD_VALUE_H	(uint8_t)	0x00
void USART_Init(void);
int16_t USART_PutChar(char , FILE* );
#endif
