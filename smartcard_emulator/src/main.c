#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include "IO.h"
#include "IoStream.h"
#include "Fifo.h"
#include "Dev.h"
#include "Art.h"
int main(void)
{
	struct art_config tty;
	struct dev_config dev0;
// Configure pin 7 on port A as output
	DDRA |= (1 << PINA7); // Debug-LED
// Infinite loop
	IoStream_LinkStream();
	uint8_t data = 0;
	MK_FIFO(tx_fifo,5);
	MK_FIFO(rx_fifo,5);
/*	for( int i = 0; i < 6; i++)
	{
		printf("Add: %o\n",Fifo_write(my_fifo,i));
		printf("fill: %o\n",Fifo_fillLevel(my_fifo));
	}
	
	for( int i = 0; i < 6; i++)
	{
		printf("Read: %o\n",Fifo_read(my_fifo,&data));
		printf("fill: %o\n",Fifo_fillLevel(my_fifo));
		printf("Data: %o\n",data);
		if ( i ==0 )
		{
			printf("Add: %o\n",Fifo_write(my_fifo,i));
			printf("fill: %o\n",Fifo_fillLevel(my_fifo));
		}
	}*/
	Fifo_write(tx_fifo,0xAA);
	tty.etu = 372;
	tty.txFifo = tx_fifo;
	tty.rxFifo = rx_fifo;
	dev0.timerValue = & tty.etu;
	dev0.rxBuff = & tty.rxBuffer;
	dev0.txBuff = & tty.txBuffer;
	dev0.statusReg = & tty.statusReg;
	dev0.artPtr = &tty;
	Dev_init(&dev0);
	Art_duplexMode(&tty,TRANSMITTER);
	if (ART_NOK == Art_txByteStart(&tty))
	{
		printf("ART_NOK\n");
	}
	else
	{
		/*printf("ARTOK\n");*/
	}
	while(1)
	{
	//	printf("Hello\n");
	// Invert the output of pin 7 on port A
		PORTA ^= (1 << PINA7);
		_delay_ms(10);
	}
}
