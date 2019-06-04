/*
 * Art.c
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
#include "Dev.h"

/* @todo make dev struct member of artPtr struct*/
uint8_t logger[255]={0};
uint8_t logIdx = 0;

static void Art_txByteDeInit(struct art_config* artPtr)
{
	Dev_denTxTimer();
	CLR_BIT(artPtr->statusReg,ART_STATUS_REG_RX);
	printf("di\n");
	artPtr->txBitIdx = 0;
	if (FIFO_OK != Fifo_read(artPtr->txFifo,& artPtr->txBuffer))
	{
		SET_BIT(artPtr->statusReg, ART_STATUS_REG_TXFIFO_FULL/* empty */);
	}
	Dev_raiseTxComplInt();
}

static void Art_rxByteDeInit(struct art_config* artPtr)
{
	Dev_denRxTimer();
	CLR_BIT(artPtr->statusReg,ART_STATUS_REG_RX);
	artPtr->rxBitIdx = 0;
	if (FIFO_OK != Fifo_write(artPtr->txFifo,artPtr->txBuffer))
	{
		SET_BIT(artPtr->statusReg, ART_STATUS_REG_RXFIFO_FULL);
	}
	Dev_raiseRxComplInt();
}

void Art_duplexMode(struct art_config* artPtr,art_mod mode)
{
	if (RECEIVER == mode)
	{
		Dev_setGpioIn();
		SET_BIT(artPtr->statusReg,ART_STATUS_REG_MODE);
	}
	if (TRANSMITTER == mode)
	{
		Dev_setGpioOut();
		CLR_BIT(artPtr->statusReg,ART_STATUS_REG_MODE);
	}
}

void Art_gpioInterrupt(struct art_config* artPtr)
{
	//logger[logIdx++] = 3;
	if(ART_LOW == Dev_getPinAvg())
	{
		Dev_enRxTimer();
		//logger[logIdx++] = 4;
		SET_BIT(artPtr->statusReg,ART_STATUS_REG_RX);
		Dev_updateRxTimerValue(350);
		Dev_denGpioInterrupt();
	}
}

void Art_rxTimerInterrupt(struct art_config* artPtr)
{
	if(0==artPtr->rxBitIdx)
	{
		logger[logIdx++] = 6;
		Dev_updateRxTimerValue(artPtr->etu);
	}
	if(ART_BYTE+1 == artPtr-> rxBitIdx)	/* 10.5 etu */
	{
		//logger[logIdx++] = 9;
		if (ART_LOW!= GET_BIT(artPtr->statusReg, \
										ART_STATUS_REG_PARITY_ERR))
		{
			//logger[logIdx++] = 10;
			Dev_setGpioOut();
			Dev_setPin(ART_LOW);
		}
	}
	if(ART_BYTE == artPtr->rxBitIdx)
	{
		//logger[logIdx++] = 8;
		artPtr->statusReg |= Dev_calcParity()<< \
										ART_STATUS_REG_PARITY_ERR;
	}
	if(artPtr->rxBitIdx < ART_BYTE)
	{
		//logger[logIdx++] = 7;
		artPtr->rxBuffer |= (Dev_getPinAvg() & 0x01) << \
													artPtr->rxBitIdx;
	}
	//logger[logIdx++] = 5;
	artPtr->rxBitIdx++;
	if(ART_BYTE+3 == artPtr-> rxBitIdx) /* 11.5 etu */
	{
		Dev_setGpioIn();
		Dev_enGpioInterrupt();
		Dev_denRxTimer();
		logger[logIdx++] = 11;
		logger[logIdx++] = artPtr->rxBuffer;
		artPtr->rxBuffer = 0;
		artPtr->rxBitIdx = 0;
		CLR_BIT(artPtr->statusReg,ART_STATUS_REG_RX);
		//Dev_raiseRxComplInt();
	}
}

void Art_txTimerInterrupt(struct art_config* artPtr)
{
	if(0==artPtr->rxBitIdx)
	{
		//logger[logIdx++] = 6;
		Dev_updateTxTimerValue(artPtr->etu);
	}
	if(ART_BYTE+1 == artPtr->txBitIdx)
	{
		Dev_setPin(ART_HIGH);
		Dev_setGpioIn();
		if(ART_LOW == (Dev_getPinAvg() & 0x01))
		{
			SET_BIT(artPtr->statusReg, \
										ART_STATUS_REG_PARITY_ERR);
		}
		Dev_setGpioOut();
	}
	if(ART_BYTE == artPtr->txBitIdx)
	{
		Dev_sendParity();
	}
	if(artPtr->txBitIdx < ART_BYTE)
	{
		Dev_setPin(UINT8_C((artPtr->txBuffer >> artPtr->txBitIdx) & 0x01));
	}
	artPtr->txBitIdx++;
	if(artPtr->txBitIdx > ART_BYTE+1)
	{
		Dev_denTxTimer();
		CLR_BIT(artPtr->statusReg,ART_STATUS_REG_TX);
		artPtr->txBitIdx = 0;
	}
}

uint8_t Art_txByteStart(struct art_config* artPtr)
{
	uint8_t ret = ART_OK;
	if (ART_STATUS_TX_READY == artPtr->statusReg)
	{
		SET_BIT(artPtr->statusReg,ART_STATUS_REG_TX);
		Fifo_read(artPtr->txFifo, &(artPtr->txBuffer));
		artPtr->txBitIdx = 0;
		Dev_enTxTimer();
		Dev_setPin(ART_LOW);
		Dev_updateTxTimerValue(350);
	}
	else
	{
		ret = ART_NOK;
	}
	return ret;
}

uint8_t Art_rxByteStart(struct art_config* artPtr)
{
	uint8_t ret = ART_OK;
	if (ART_STATUS_RX_READY == artPtr->statusReg)
	{
		//logger[logIdx++] = 1;
		artPtr->rxBuffer = 0;
		artPtr->rxBitIdx = 0;
		Dev_enGpioInterrupt();
	}
	else
	{
		ret = ART_NOK;
	}
	return ret;
}
