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

static void Art_txByteDeInit(struct art_config* artPtr)
{
	Dev_denTxTimer();
	CLR_BIT(artPtr->statusReg,ART_STATUS_REG_RX);
	artPtr->txBitIdx = 0;
	if (FIFO_OK != Fifo_write(artPtr->txFifo,artPtr->txBuffer))
	{
		SET_BIT(artPtr->statusReg, \
										ART_STATUS_REG_TXFIFO_FULL);
	}
	Dev_raiseTxComplInt();
}

static void Art_rxByteDeInit(struct art_config* artPtr)
{
	Dev_denRxTimer();
	CLR_BIT(artPtr->statusReg,ART_STATUS_REG_TX);
	artPtr->rxBitIdx = 0;
	if (FIFO_OK != Fifo_write(artPtr->txFifo,artPtr->txBuffer))
	{
		SET_BIT(artPtr->statusReg, \
										ART_STATUS_REG_RXFIFO_FULL);
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
	if(ART_LOW == Dev_getPinAvg())
	{
		SET_BIT(artPtr->statusReg,ART_STATUS_REG_RX);
		Dev_enRxTimer();

		Dev_updateRxTimerValue(artPtr->etu/* some arithmetic */);

		Dev_denGpioInterrupt();
	}
}

void Art_rxTimerInterrupt(struct art_config* artPtr)
{
	if(artPtr-> rxBitIdx > ART_BYTE+2) /* 12.5 etu */
	{
		Art_rxByteDeInit(artPtr);
	}
	if(ART_BYTE+2 == artPtr-> rxBitIdx) /* 11.5 etu */
	{
		Dev_setGpioIn();
	}
	if(ART_BYTE+1 == artPtr-> rxBitIdx)	/* 10.5 etu */
	{
		if (ART_LOW!= GET_BIT(artPtr->statusReg, \
										ART_STATUS_REG_PARITY_ERR))
		{
			Dev_setGpioOut();
			Dev_setPin(ART_LOW);
		}
	}
	if(ART_BYTE+2 == artPtr->rxBitIdx)
	{
		artPtr->statusReg |= Dev_calcParity()<< \
										ART_STATUS_REG_PARITY_ERR;
	}
	if(artPtr->rxBitIdx < ART_BYTE)
	{
		artPtr->rxBuffer |= (Dev_getPinAvg() & 0x01) << \
													artPtr->rxBitIdx;
	}
	artPtr->rxBitIdx++;
}

void Art_txTimerInterrupt(struct art_config* artPtr)
{
	if(artPtr->txBitIdx > ART_BYTE+2)
	{
		Art_txByteDeInit(artPtr);
	}
	if(ART_BYTE+2 == artPtr->txBitIdx)
	{
		Dev_setGpioIn();
		if(ART_LOW == (Dev_getPinAvg() & 0x01))
		{
			SET_BIT(artPtr->statusReg, \
										ART_STATUS_REG_PARITY_ERR);
		}
	}
	if(ART_BYTE == artPtr->txBitIdx)
	{
		Dev_sendParity();
	}
	if(artPtr->txBitIdx < ART_BYTE)
	{
		Dev_setPin((artPtr->txBuffer >> artPtr->txBitIdx) & 0x01);
	}
	artPtr->txBitIdx++;
	printf("TxBit: %o\n",artPtr->txBitIdx);
}

uint8_t Art_txByteStart(struct art_config* artPtr)
{
	uint8_t ret = ART_OK;
	if (ART_STATUS_TX_READY == artPtr->statusReg)
	{
		/*printf("Status: %o\n",artPtr->statusReg);*/
		SET_BIT(artPtr->statusReg,ART_STATUS_REG_TX);
		Fifo_read(artPtr->txFifo, &(artPtr->txBuffer));
		Dev_setPin(ART_LOW);
		Dev_enTxTimer();
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
		artPtr->rxBuffer = 0;
		Dev_enGpioInterrupt();
	}
	else
	{
		ret = ART_NOK;
	}
	return ret;
}
