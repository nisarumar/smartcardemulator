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

int main(void)
{

	struct art_config tty;
	struct dev_config dev0;
	IoStream_LinkStream();
    DDRA = 0xFF;
	uint8_t cnt = 0x00;
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
	while(0!=GET_BIT(tty.statusReg,ART_STATUS_REG_TX));
	_delay_ms(100);
	Apdu_getResponse(&tty);
	while(1)
	{
	}
}
