/*
 * Fifo.c
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
#include "Fifo.h"

uint8_t Fifo_fillLevel(struct fifo_t* handl)
{
	return handl->fl;
}

uint8_t Fifo_write(struct fifo_t* handl, uint8_t data)
{
	uint8_t ret = FIFO_OK;
	if(handl->fl < handl->sz)
	{
		handl->buff[handl->wr] = data;
		handl->wr = (handl->wr+1) % handl->sz;
		handl->fl++;
	}
	else
	{
		ret = FIFO_NOK; 	/* FIFO_FULL */
	}
	return ret;
}
uint8_t Fifo_read(struct fifo_t* handl, uint8_t * data)
{
	uint8_t ret = FIFO_OK;
	if(handl->fl > 0)
	{
		*data = handl->buff[handl->rd];
		handl->rd = (handl->rd+1) % handl->sz;
		handl->fl--;
	}
	else
	{
		ret = FIFO_NOK; 	/* FIFO_EMPTY */
	}
	return ret;
}
