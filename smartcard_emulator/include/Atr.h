#ifndef ATR_H_
#define ATR_H_

#define ATR_LOW				UINT8_C(0)
#define ATR_HIGH			UINT8_C(0)
#define ATR_PORT			DDRB
#define ATR_PIN				PB3

#define ATR_OK				UINT8_C(0)
#define ATR_NOK				UINT8_C(1)

#define ATR_BYTE_TO_SEND	UINT8_C(4)
/* ATR Bytes to Send */
#define ATR_BYTE_0			UINT8_C(0x3B)
#define ATR_BYTE_1			UINT8_C(0x90) 
#define ATR_BYTE_2			UINT8_C(0x11)
#define ATR_BYTE_3			UINT8_C(0x00)

uint8_t Atr_main(struct art_config*);

#endif
