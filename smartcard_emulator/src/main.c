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

#define SHUFFLING
#define DUMMY
#define MASKING
extern uint8_t stateText[];

void main (void){
	IoStream_LinkStream();
	/*
	struct art_config tty;
	struct dev_config dev0;
	
    DDRA = 0xFF;
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
	Art_duplexMode(&tty,TRANSMITTER);
	sei();
	Atr_main(&tty);
	while(0!=GET_BIT(tty.statusReg,ART_STATUS_REG_TX))*/;
	//while(1)/{	
		for (uint8_t i=0; i<16; i++){
			printf("%x ",stateText[i]);	
		}
		printf("\n");
		//Apdu_decryptKey(&tty);
		aes_dec_128(stateText);
		//Apdu_getResponse(&tty);
		
		for (uint8_t i=0; i<16; i++){
			printf("%x ",stateText[i]);
			
		}
	//}
}
