#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include "IO.h"
#include "IoStream.h"

int main(void)
{
// Configure pin 7 on port A as output
	DDRA |= (1 << PINA7); // Debug-LED
// Infinite loop
	IoStream_LinkStream();
	while(1)
	{
		printf("Hello\n");
	// Invert the output of pin 7 on port A
	PORTA ^= (1 << PINA7);
	_delay_ms(1000);
	}
}
