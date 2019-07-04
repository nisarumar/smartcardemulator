
#include <stdio.h>
#include <stdint.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/atomic.h>
#include "IO.h"
#include "Fifo.h"
#include "Rng.h"
#include "Seed.h"

uint8_t WDTBUFF[SEED_RAW_BUFFER_SIZE] = {0};
uint8_t WDTBUFFIdx = 0;
uint8_t Seed_StatusReg = 0x00;
uint32_t Seed_seed[SEED_BUFFER_SIZE];
uint8_t Seed_seedCount = 0;
uint16_t Seed_seedAddr= 0x00;
uint8_t Seed_eepromCount = 0;
extern uint8_t Rng_seed[];
void Seed_init(void)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		Seed_StatusReg = 0x00;
		Seed_seedAddr=(uint16_t)eeprom_read_byte((uint8_t *)0);
		if (0xFF == Seed_seedAddr || 0x00 == Seed_seedAddr)
		{
			Seed_seedAddr = 1;
			SET_BIT(PORTA,PA4);
		}
		MCUSR &= ~(1<<WDRF);
		WDTCSR |= (1<<WDCE) | (1<<WDE);
		WDTCSR |= (1<<WDIE);
		TCNT2 = 0;
		SET_BIT(TCCR2B, CS20);
	}
}


void Seed_deInit(void)
{
	if(0x01 == GET_BIT(Seed_StatusReg,SEED_STATUS_WHITE_DONE))
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			CLR_BIT(TCCR2B, CS20);
			MCUSR &= ~(1<<WDRF);
			WDTCSR |= (1<<WDCE) | (1<<WDE);
			WDTCSR = 0x00;
		}
	}
}


ISR(WDT_vect) {
	
	if (0x00 == GET_BIT(Seed_StatusReg,SEED_STATUS_FIFO_FULL))
	{
		WDTBUFF[WDTBUFFIdx++] = TCNT2;
		if (SEED_RAW_BUFFER_SIZE==WDTBUFFIdx/*FIFO_NOK == Fifo_write(WDTBUFF,TCNT2)*/)
		{
			SET_BIT(Seed_StatusReg,SEED_STATUS_FIFO_FULL);
			WDTBUFFIdx=0;
		}
		//TGL_BIT(PORTA,PA0);
	}
	WDTCSR |= (1<<WDIE);
}

void Seed_whitening(void)
{
	uint8_t dataIdx=0;
	if(0x01 == GET_BIT(Seed_StatusReg,SEED_STATUS_FIFO_FULL) && \
				0x00 == GET_BIT(Seed_StatusReg,SEED_STATUS_WHITE_DONE))
	{
		while(dataIdx<SEED_RAW_BUFFER_SIZE/*FIFO_OK == Fifo_read(WDTBUFF,&data)*/)
		{
			Seed_seed[Seed_seedCount] +=WDTBUFF[dataIdx++];
			Seed_seed[Seed_seedCount] += Seed_seed[Seed_seedCount] <<10;
			Seed_seed[Seed_seedCount] ^= Seed_seed[Seed_seedCount] >>6;
		}
		dataIdx = 0;
		Seed_seed[Seed_seedCount] += Seed_seed[Seed_seedCount] <<3;
		Seed_seed[Seed_seedCount] ^= Seed_seed[Seed_seedCount] >>11;
		Seed_seed[Seed_seedCount] += Seed_seed[Seed_seedCount] <<15;
		//SET_BIT(PORTA,PA1);
		Seed_seedCount++;
		if (Seed_seedCount == SEED_BUFFER_SIZE)
		{
			//SET_BIT(PORTA,PA2);
			SET_BIT(Seed_StatusReg,SEED_STATUS_WHITE_DONE);
		}
		else
		{
			CLR_BIT(Seed_StatusReg,SEED_STATUS_FIFO_FULL);
		}
	}
}

void Seed_pause(void)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		WDTCSR |= (1<<WDCE) | (1<<WDE);
		WDTCSR = 0x00;
	}
}
void Seed_resume(void)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		WDTCSR |= (1<<WDCE) | (1<<WDE);
		WDTCSR |= (1<<WDIE);
	}
}

void Seed_writeEeprom(void)
{
	static uint8_t count=0;
	uint8_t temp=0;
	if (Seed_eepromCount < Seed_seedCount)
	{
		//TGL_BIT(PORTA,PA3);
		temp = Seed_seed[Seed_eepromCount]>>count;
		eeprom_busy_wait();
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			eeprom_write_byte((uint8_t *)Seed_seedAddr++,temp);
		}
		count+=8;
		if (32 == count)
		{
			count = 0;
			Seed_eepromCount++;
		}
	}
}

void Seed_readEEprom(void)
{
	static uint8_t address=1;
	uint8_t seedIdx = 0;
	TGL_BIT(PORTA,PA2);
	for (seedIdx = 0;seedIdx<RNG_SEED_SIZE;seedIdx++)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			eeprom_busy_wait();
			Rng_seed[seedIdx]=eeprom_read_byte((uint8_t *)address);
		}
		address = (address%UINT8_MAX)+1;
	}
/*	else
	{
		if (address<Seed_seedAddr)
		{
			printf("E\n");
			eeprom_busy_wait();
			ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
			{
				tempData=eeprom_read_byte((uint8_t *)address++);
			}
			TGL_BIT(PORTA,PA1);
		}
		else
		{
			printf("W\n");
			while (address <= WDTBUFFIdx);
			tempData = WDTBUFF[addressC++];
			addressC %= SEED_RAW_BUFFER_SIZE;
			TGL_BIT(PORTA,PA0);
		}
	}*/
}

void Seed_seedReInit(void)
{
	if (0x01==GET_BIT(Seed_StatusReg,SEED_STATUS_WHITE_DONE) && Seed_eepromCount==SEED_BUFFER_SIZE)
	{
		Seed_seedCount=0;
		Seed_eepromCount=0;
		CLR_BIT(PORTA,PA4);
		if (255 <= Seed_seedAddr || 0 == Seed_seedAddr)
		{
			Seed_seedAddr = 1;
			CLR_BIT(Seed_StatusReg,SEED_STATUS_WRITE_DONE);
			SET_BIT(Seed_StatusReg,SEED_STATUS_WRITE_DONE);
			SET_BIT(PORTA,PA5);
		}
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			eeprom_write_byte((uint8_t *)0,Seed_seedAddr);
		}
		CLR_BIT(Seed_StatusReg,SEED_STATUS_WHITE_DONE);
		CLR_BIT(Seed_StatusReg,SEED_STATUS_FIFO_FULL);
	}
}
