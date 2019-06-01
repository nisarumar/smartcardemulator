#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "IO.h"
#include "IoStream.h"
#include "Fifo.h"
#include "Dev.h"
#include "Art.h"

int main(void)
{
	struct art_config tty;
	struct dev_config dev0;
	IoStream_LinkStream();
	PORTA = MCUSR;
	MCUSR=0;
	_delay_ms(10);
    DDRA = 0xFF;
	uint8_t txData = 0x90;
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
	Dev_init(&dev0);
	sei();
	printf("st\n");
	Art_duplexMode(&tty,TRANSMITTER);
	while(1)
	{
		if(0==GET_BIT(tty.statusReg,ART_STATUS_REG_TX))
		{
			tty.statusReg = 0;
			Fifo_write(tty.txFifo,txData++);
			Art_txByteStart(&tty);
			txData%=255;
		}
	}
}
