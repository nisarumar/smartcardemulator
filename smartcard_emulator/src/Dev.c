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

extern uint8_t logger[];
extern uint8_t logIdx;

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

static void Dev_auxTimerInit(void)
{
	SET_BIT(TCCR0A, WGM01);
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		TCNT0 = 0;
		OCR0A = (uint8_t) 88;
	}
	SET_BIT(TIMSK0, OCIE0A);
}

static void Dev_configRxCompInterrupt(void)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		SET_BIT(DDRD,PD2); /* PD2 (INT0) as Out */
		SET_BIT(EICRA,ISC00); /* Any edge trigger for INT0 */
		SET_BIT(EIMSK,INT0);
	}
	
}

void Dev_setGpioIn(void)
{
	CLR_BIT(DDRB, PB6);
	SET_BIT(PORTB, PB6);
}

void Dev_setGpioOut(void)
{
	SET_BIT(PORTB, PB6);
	SET_BIT(DDRB, PB6);
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

void Dev_updateTxTimerValue(uint16_t etu)
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
	//logger[logIdx] = GET_BIT(PORTB, PB6);
	r += (GET_BIT(PINB, PB6))?1:-1;
	r += (GET_BIT(PINB, PB6))?1:-1;
	r += (GET_BIT(PINB, PB6))?1:-1;
	r += (GET_BIT(PINB, PB6))?1:-1;
	r += (GET_BIT(PINB, PB6))?1:-1;
	ret = (r>0)?1:0;
	//logger[logIdx++] = ret;
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
ISR(TIMER0_COMPA_vect)
{
	//Dev_raiseRxComplInt();
}
ISR(PCINT1_vect)
{
	//logger[logIdx++] = 2;
	//logger[logIdx++] = GET_BIT(PINB,PB6);
	Art_gpioInterrupt(devPtr-> artPtr);
}

ISR(INT0_vect)
{
	devPtr->artPtr->statusReg = 0;
	Fifo_write(devPtr->artPtr->txBuffer, 239);
	Art_duplexMode(devPtr->artPtr,TRANSMITTER);
	Art_txByteStart(devPtr->artPtr);
}

void Dev_init(struct dev_config * devSt)
{
	devPtr = devSt;
	Dev_timerInit();
	Dev_auxTimerInit();
	Dev_configRxCompInterrupt();
}

void Dev_raiseTxComplInt(void)
{
	/*@todo:*/
}

void Dev_raiseRxComplInt(void)
{
	static uint8_t count =0;
	logger[logIdx] == 144;
	if (3 == count)
	{
		TGL_BIT(PORTD,PD2);
		count = 0;
	}
	else
	{
		count++;
	}
}

void Dev_enAuxTimer(void)
{
	SET_BIT(TCCR0B,CS02);
}

void Dev_denAuxTimer(void)
{
	CLR_BIT(TCCR0B,CS02);
}
