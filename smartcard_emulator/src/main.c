#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include "IO.h"
#include "IoStream.h"
#include "Fifo.h"

int main(void)
{
// Configure pin 7 on port A as output
	DDRA |= (1 << PINA7); // Debug-LED
// Infinite loop
	IoStream_LinkStream();
	uint8_t data = 0;
	MK_FIFO(my_fifo,5);
	for( int i = 0; i < 6; i++)
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
	}
	
	while(1)
	{
	//	printf("Hello\n");
	// Invert the output of pin 7 on port A
		PORTA ^= (1 << PINA7);
		_delay_ms(10);
	}
}
