#include "aes_shuffling.h"
#include <stdlib.h>

uint8_t shuffling_array[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};


void generate_new_shuffling_array(void) {

	uint8_t i = 15; // array end
	uint8_t j = 0;  // init value
	do
	{	
		j = ((uint8_t)rand())%16; 
		
		//swap the last element with element at random index
		uint8_t temp = shuffling_array[i];
		shuffling_array[i] = shuffling_array[j];
		shuffling_array[j] = temp;
		--i;
	}while(i!=0);
}
