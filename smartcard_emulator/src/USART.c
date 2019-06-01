/*
 * Usart.c
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
#include "USART.h"

/*
 *
 * name: USART_Init
 * @param void
 * @return void
 * @brief: set baudrate and initialized usart for tx
 */

void USART_Init(void)
{
	/* Set baud rate */
	UBRR0L = BAUD_VALUE_L;
	UBRR0H = BAUD_VALUE_H; 
	/* Enable transmitter */
	SET_BIT(UCSR0B,TXEN0);
	/* Set frame format: 8 data bit */
	SET_BIT(UCSR0C,UCSZ01);
	SET_BIT(UCSR0C,UCSZ00);
	/* USART DEBUG */
	SET_BIT(DDRA,PINA0);
}

/*
 *
 * name: USART_PutChar
 * @param character, FILE to link stream
 * @return int16_t	OK = 0, NOK = !0
 * @brief: tranmits character over USART0
 */

int16_t USART_PutChar(char c, FILE* stream)
{
	uint8_t count = 0;
	int16_t ret	= 0;
	if (c == '\n')
	{
        ret = USART_PutChar('\r', stream);
    }
	while(0 == GET_BIT(UCSR0A,UDRE0))
	{
		/* wait for buffer to be free */
		//count++;
		if (UINT8_MAX == count)
		{
			break;
		}
	}

	if (UINT8_MAX == count)
	{
		TGL_BIT(PORTA,PINA0);
		ret |= INT16_C(1);
	}
	else
	{
		UDR0 = c;
		ret |= INT16_C(0);
	}
	return ret;
}
