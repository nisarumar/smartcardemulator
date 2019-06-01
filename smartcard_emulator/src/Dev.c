/* @todo : make generic (applicable to all file) */
#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <util/parity.h>
#include "IO.h"
#include "IoStream.h"
#include "Art.h"
#include "Dev.h"

struct dev_config* devPtr;

void Dev_timerInit(void)
{
	SET_BIT(TCCR1B, WGM12); /* CTC */
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		TCNT1 = 0;
		OCR1A = *(devPtr-> timerValue);
	}
	SET_BIT(TIMSK1, OCIE1A);
}

void Dev_setGpioIn(void)
{
	CLR_BIT(DDRB, PB6);
	SET_BIT(PORTB, PB6);
}

void Dev_setGpioOut(void)
{
	SET_BIT(DDRB, PB6);
	SET_BIT(PORTB, PB6);
}

void Dev_enRxTimer(void)
{
	TCNT1 = 0;
	SET_BIT(TCCR1B, CS10);
}

void Dev_enTxTimer(void)
{
	TCNT1 = 0;
	SET_BIT(TCCR1B, CS10);
}

void Dev_updateRxTimerValue(uint16_t etu)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		OCR1A = etu;
	}
}

void Dev_denGpioInterrupt(void)
{
	CLR_BIT(PCICR,PCIE1);
	CLR_BIT(PCMSK1,PCINT14);
}

void Dev_setPin(uint8_t pinValue)
{
	if (ART_LOW == pinValue)
	{
		CLR_BIT(PORTB, PB6);
	}
	if (ART_HIGH == pinValue)
	{
		SET_BIT(PORTB, PB6);
	}
}
uint8_t Dev_getPinAvg(void)
{
	int8_t r = 0;
	uint8_t ret;
	r += (GET_BIT(PORTB, PB6))?1:-1;
	r += (GET_BIT(PORTB, PB6))?1:-1;
	r += (GET_BIT(PORTB, PB6))?1:-1;
	r += (GET_BIT(PORTB, PB6))?1:-1;
	r += (GET_BIT(PORTB, PB6))?1:-1;
	ret = (r>0)?1:0;
	return ret;
}

uint8_t Dev_calcParity(void)
{
	if (Dev_getPinAvg() == parity_even_bit(*(devPtr-> rxBuff)))
	{
		return ART_OK;
	}
	else
	{
		return ART_NOK;
	}
}

void Dev_sendParity(void)
{
	Dev_setPin(parity_even_bit(*(devPtr-> txBuff)));
}

void Dev_denTxTimer(void)
{
	CLR_BIT(TCCR1B, CS10);
}

void Dev_denRxTimer(void)
{
	CLR_BIT(TCCR1B, CS10);
}

void Dev_enGpioInterrupt(void)
{
	SET_BIT(PCICR,PCIE1);
	SET_BIT(PCMSK1,PCINT14);
}

/*@todo : introduce abstraction in macro's instead of if else*/
ISR(TIMER1_COMPA_vect)
{
	if(ART_HIGH == GET_BIT(*(devPtr->statusReg),ART_STATUS_REG_RX))
	{
		Art_rxTimerInterrupt(devPtr-> artPtr);
	}
	if(ART_HIGH == GET_BIT(*(devPtr->statusReg),ART_STATUS_REG_TX))
	{
		Art_txTimerInterrupt(devPtr-> artPtr);
	}
}

ISR(PCINT1_vect)
{
	Art_gpioInterrupt(devPtr-> artPtr);
}

void Dev_init(struct dev_config * devSt)
{
	devPtr = devSt;
	Dev_timerInit();
}

void Dev_raiseTxComplInt(void)
{
	/*@todo:*/
}

void Dev_raiseRxComplInt(void)
{
	/* @todo: */
}
