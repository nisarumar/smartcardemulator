#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "IO.h"
#include "IoStream.h"
#include "Fifo.h"
#include "Dev.h"
#include "Art.h"
#include "Atr.h"
#include "AES.h"
#include "Seed.h"


extern uint8_t stateText[];
extern struct fifo_t* WDTBUFF;

void main (void)
{
/*	struct art_config tty;
	struct dev_config dev0;*/
	IoStream_LinkStream();
	uint8_t data = 0;
	DDRA = 0xFF;
	PORTA = MCUSR;
	MCUSR = 0;
	printf("r\n");
	//MK_FIFO(WDTBUFF,5);
/*    DDRA = 0xFF;
	MK_FIFO(tx_fifo,5);
	MK_FIFO(rx_fifo,5);
	tty.etu = 372;
	tty.txFifo = tx_fifo;
	tty.rxFifo = rx_fifo;
	tty.statusReg=0;
	dev0.timerValue = & tty.etu;
	dev0.rxBuff = & tty.rxBuffer;
	dev0.txBuff = & tty.txBuffer;
	dev0.statusReg = & tty.statusReg;
	dev0.artPtr = &tty;
	dev0.rxByteCount = 4;
	Dev_init(&dev0);
	Art_duplexMode(&tty,TRANSMITTER);*/
	sei();
	Seed_init();
/*	Atr_main(&tty);
	while(0!=GET_BIT(tty.statusReg,ART_STATUS_REG_TX));*/
	while(1)
	{
/*		Apdu_decryptKey(&tty);
		aes_dec_128(stateText, roundkeyarr);
		Apdu_getResponse(&tty);*/
		_delay_ms(2500);
		printf("L: %x\n",Fifo_fillLevel(WDTBUFF));
		while(FIFO_OK==Fifo_read(WDTBUFF,&data))
		{
			printf("v: %x\n",data);
		}
	}
}
