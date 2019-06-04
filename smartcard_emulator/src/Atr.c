/*
 * Atr.c
 * 
 * Copyright 2019 Umar Nisar <umarnisar@outlook.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include <stdint.h>
#include <stdio.h>
#include "IO.h"
#include "IoStream.h"
#include "Fifo.h"
#include "Art.h"
#include "Atr.h"

extern uint8_t logger[];
extern uint8_t logIdx;

static uint8_t ATR_BYTE[ATR_BYTE_TO_SEND]={ATR_BYTE_0,ATR_BYTE_1,ATR_BYTE_2,ATR_BYTE_3};

uint8_t Atr_main(struct art_config* artPtr)
{
	uint8_t byteSent = 0;
	CLR_BIT(ATR_PORT,ATR_PIN);
	//logger[logIdx++] = 14;
	//logger[logIdx++]==GET_BIT(PINB,PB3);
	while(ATR_LOW == GET_BIT(PINB,ATR_PIN));
	while (byteSent < ATR_BYTE_TO_SEND)
	{
		if(0==GET_BIT(artPtr->statusReg,ART_STATUS_REG_TX))
		{
				artPtr->statusReg = 0;
				Fifo_write(artPtr->txFifo,ATR_BYTE[byteSent]);
				Art_txByteStart(artPtr);
				byteSent++;
		}
	}
	logger[logIdx++] = 13;
	return ATR_OK;
}
