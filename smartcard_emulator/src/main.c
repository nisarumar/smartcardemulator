#define I_KNOW_ABOUT_THREEFISH_ALIGNMENT_RESTRICTIONS
#include "fkein.h"
extern threefish_param_t tparam_asm;
volatile threefish_param_t* tparam;
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "IO.h"
#include "IoStream.h"
#include "Fifo.h"
#include "Dev.h"
#include "Art.h"
#include "Atr.h"
#include "AES.h"
#include "Seed.h"
#include "BkrndTask.h"
#include "Rng.h"
void main (void)
{
	tparam = (void*) &tparam_asm;
	struct art_config tty;
	struct dev_config dev0;
	IoStream_LinkStream();
	uint8_t data = 0;
	DDRA = 0xFF;
	MCUSR = 0x00;
	PORTA = MCUSR;

extern uint8_t stateText[];
	MK_FIFO(tx_fifo,5);
	tx_fifo->buff = &fifo_buff_tx_fifo[0];
	tx_fifo->sz = 5 & UINT8_MAX;
	MK_FIFO(rx_fifo,5);
	rx_fifo->buff = &fifo_buff_rx_fifo[0];
	rx_fifo->sz = 5 & UINT8_MAX;
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
	Seed_init();
	Art_duplexMode(&tty,TRANSMITTER);
	TRIGGER_INIT();
	sei();
	Rng_init();
	Atr_main(&tty);
	while(0!=GET_BIT(tty.statusReg,ART_STATUS_REG_TX));
	while(1){		
		Apdu_decryptKey(&tty);
		TRIGGER_SET();
		aes_dec_128(stateText);
		TRIGGER_CLR();
		Rng_reseed();
		Apdu_getResponse(&tty);
		Seed_resume();	
	}
}
