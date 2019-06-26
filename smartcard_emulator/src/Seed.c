
#include <stdio.h>
#include <stdint.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/atomic.h>
#include "IO.h"
#include "Fifo.h"


MK_FIFO(WDTBUFF,32);

uint8_t Seed_StatusReg = 0x00;

void Seed_init(void)
{
	WDTBUFF->buff = &fifo_buff_WDTBUFF[0];
	WDTBUFF->sz = 32 & UINT8_MAX;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		MCUSR &= ~(1<<WDRF); /* to ensure reset after interrupt */
		WDTCSR |= (1<<WDCE) | (1<<WDE);
		WDTCSR |= (1<<WDIE);
		TCNT2 = 0;
		SET_BIT(TCCR2B, CS20);
	}
}


void Seed_deInit(void)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		CLR_BIT(TCCR2B, CS20);
		MCUSR &= ~(1<<WDRF);
		/* Write logical one to WDCE and WDE */
		/* Keep old prescaler setting to prevent unintentional time-out */
		WDTCSR |= (1<<WDCE) | (1<<WDE);
		/* Turn off WDT */
		WDTCSR = 0x00;
	}
}


ISR(WDT_vect) {
	
	if (0x01 == GET_BIT(Seed_StatusReg,SEED_FIFO_FULL))
	{
		if (FIFO_NOK == Fifo_write(WDTBUFF,TCNT2))
		{
			SET_BIT(Seed_StatusReg,SEED_FIFO_FULL);
		}
	}
	//CLR_BIT(WDTCSR,WDIE); /* ensures that reset dooes't happen */
	WDTCSR |= (1<<WDIE);
}


void Seed_writeToEeprom(void)
{
	
}




