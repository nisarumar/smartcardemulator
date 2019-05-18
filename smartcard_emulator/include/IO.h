#ifndef IO_H_
#define IO_H_

#include <avr/io.h>

#define SET_BIT(PORT, BIT)	PORT |= (uint8_t)(UINT8_C(1)<<BIT)
#define CLR_BIT(PORT, BIT)	PORT &= (uint8_t)~(UINT8_C(1)<<BIT)
#define TGL_BIT(PORT, BIT)	PORT ^= (uint8_t)(UINT8_C(1)<<BIT)
#define GET_BIT(PORT, BIT)	((PORT & (UINT8_C(1)<<BIT))?UINT8_C(1):UINT8_C(0))

#define LED_INIT()		DDRA = 0xFF
#define lED_SET(VAL)	PORTA = (uint8_t)VAL

#define TRIGGER_INIT()	{SET_BIT(DDRB, PB4); CLR_BIT(PORTB, PB4);}
#define TRIGGER_SET()	SET_BIT(PORTB, PB4)
#define TRIGGER_CLR()	CLR_BIT(PORTB, PB4)

#endif /* IO_H_ */
