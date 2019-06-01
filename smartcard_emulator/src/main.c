#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include "IO.h"
#include "IoStream.h"
#include "Fifo.h"
#include "AES.h"
#include <avr/interrupt.h>
#include <avr/io.h>
uint16_t timerValue;
uint8_t tot_overflow;
void init_timer1(void);
void init_timer1(void){
	TCCR1A =0;
	TCCR1B =0;
	TCCR1B |= (1<<CS10);
	TIMSK1 = (1<<TOIE1);
	sei();
	TCNT1=0;
	timerValue=0;

}
ISR(TIMER1_OVF_vect)
{
    // keep a track of number of overflowstim
    tot_overflow++;
}
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
	 printf("Cipher Text is \n");
	 for (uint8_t i=0; i<16; i++){
			printf("%x",stateText[i]);
			}

        printf("\n");
	
     	//generate the key for every round
     	
		//gen_roundkey (key , roundkeyarr, aes_sbox, rcon);
		
		init_timer1();
		//decrypt
		aes_dec_128(stateText, roundkeyarr);
		timerValue=TCNT1;
		printf("No. of time timer overflowed %u \n",tot_overflow);
		printf("Time take to decrypt is %u \n",timerValue);
	
		printf("Plain Text is \n");

	  for (uint8_t i=0; i<16; i++){
			printf("%x",stateText[i]);
			}
	
	while(1);
	//{
	//	printf("Hello\n");
	// Invert the output of pin 7 on port A
	//	PORTA ^= (1 << PINA7);
	//	_delay_ms(10);
	//}
}
