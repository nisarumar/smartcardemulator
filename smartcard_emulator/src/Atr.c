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


static uint8_t ATR_BYTE[ATR_BYTE_TO_SEND]={ATR_BYTE_0,ATR_BYTE_1,ATR_BYTE_2,ATR_BYTE_3};
uint8_t AES_KEY[16]={0};

uint8_t Atr_main(struct art_config* artPtr)
{
	uint8_t byteSent = 0;
	CLR_BIT(ATR_PORT,ATR_PIN);
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
	while(0!=GET_BIT(artPtr->statusReg,ART_STATUS_REG_TX));
	artPtr->statusReg=0;
	byteSent=0;
	Art_duplexMode(artPtr,RECEIVER);
	Art_rxByteStart(artPtr);
	while(byteSent < 5)
	{
		while(0==GET_BIT(artPtr->statusReg,ART_STATUS_REG_RX_COMP));
		CLR_BIT(artPtr->statusReg,ART_STATUS_REG_RX_COMP);
		byteSent++;
	}
	byteSent=0;
	while(byteSent<16)
	{
		artPtr->statusReg = 0;
		Art_duplexMode(artPtr,TRANSMITTER);
		Fifo_write(artPtr->txFifo,239);
		Art_txByteStart(artPtr);
		while(0!=GET_BIT(artPtr->statusReg,ART_STATUS_REG_TX));
		artPtr->statusReg=0;
		Art_duplexMode(artPtr,RECEIVER);
		Art_rxByteStart(artPtr);
		while(0==GET_BIT(artPtr->statusReg,ART_STATUS_REG_RX_COMP));
		CLR_BIT(artPtr->statusReg,ART_STATUS_REG_RX_COMP);
		AES_KEY[byteSent++]=artPtr->rxBuffer;
	}
	return ATR_OK;
}

uint8_t Apdu_getResponse(struct art_config* artPtr)
{
	uint8_t byteSent = 0;
	artPtr->statusReg = 0;
	Art_duplexMode(artPtr,TRANSMITTER);
	Fifo_write(artPtr->txFifo,0x61);
	Art_txByteStart(artPtr);
	while(0!=GET_BIT(artPtr->statusReg,ART_STATUS_REG_TX));
	Fifo_write(artPtr->txFifo,0x10);
	Art_txByteStart(artPtr);
	while(0!=GET_BIT(artPtr->statusReg,ART_STATUS_REG_TX));
	artPtr->statusReg=0;
	byteSent=0;
	Art_duplexMode(artPtr,RECEIVER);
	Art_rxByteStart(artPtr);
	while(byteSent < 5)
	{
		while(0==GET_BIT(artPtr->statusReg,ART_STATUS_REG_RX_COMP));
		CLR_BIT(artPtr->statusReg,ART_STATUS_REG_RX_COMP);
		byteSent++;
	}
	byteSent=0;
	artPtr->statusReg = 0;
	Art_duplexMode(artPtr,TRANSMITTER);
	Fifo_write(artPtr->txFifo,0xC0);
	Art_txByteStart(artPtr);
	while(0!=GET_BIT(artPtr->statusReg,ART_STATUS_REG_TX));
	byteSent=0;
	while (byteSent < 16)
	{
		if(0==GET_BIT(artPtr->statusReg,ART_STATUS_REG_TX))
		{
				artPtr->statusReg = 0;
				Fifo_write(artPtr->txFifo,AES_KEY[byteSent++]);
				Art_txByteStart(artPtr);
		}
	}
	while(0!=GET_BIT(artPtr->statusReg,ART_STATUS_REG_TX));
	artPtr->statusReg = 0;
	Art_duplexMode(artPtr,TRANSMITTER);
	Fifo_write(artPtr->txFifo,0x9D);
	Art_txByteStart(artPtr);
	while(0!=GET_BIT(artPtr->statusReg,ART_STATUS_REG_TX));
	Fifo_write(artPtr->txFifo,0x00);
	Art_txByteStart(artPtr);
	while(0!=GET_BIT(artPtr->statusReg,ART_STATUS_REG_TX));
	
	return ART_OK;
}
