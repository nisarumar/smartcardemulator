/*
 * Copyright 2019 Chander Kumar <chander@chanderkumar>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */
#include <stdio.h>
#include <string.h>
#include <AES.h>
#include <stdlib.h>
#include <time.h>
#include <Rng.h>
#include "IO.h"
#define SHUFFLING
//#define DUMMY
//#define NORMAL
//#define MASKING
/*
 *This MACRO is understood and taken from https://github.com/kokke/tiny-AES-c/blob/master/aes.c
 */
extern uint8_t Rng_rand[];
# define gf256mul(x,y)      					\
		(((y & 1) * x) ^						\
        ((y>>1 & 1) * xtime(x)) ^	          	\
        ((y>>2 & 1) * xtime(xtime(x))) ^		\
        ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^	\
        ((y>>4 & 1) * xtime(xtime(xtime(xtime(x)))))) 

const uint8_t aes_invsbox[256] = {
 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
 0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
 0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
 0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
 0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
 0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
 0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
 0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
 0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
 0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
 0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
 0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};
const uint8_t roundkeyarr [176] ={
0xb5, 0x2e, 0x33, 0xb1, 0x2a, 0x71, 0x1d, 0xcb, 0xf9, 0xa7, 0x8a, 0xd7, 0x39, 0xd8, 0x82, 0x08,
0xd5, 0x3d, 0x03, 0xa3, 0xff, 0x4c, 0x1e, 0x68, 0x06, 0xeb, 0x94, 0xbf, 0x3f, 0x33, 0x16, 0xb7,
0x14, 0x7a, 0xaa, 0xd6, 0xeb, 0x36, 0xb4, 0xbe, 0xed, 0xdd, 0x20, 0x01, 0xd2, 0xee, 0x36, 0xb6,
0x38, 0x7f, 0xe4, 0x63, 0xd3, 0x49, 0x50, 0xdd, 0x3e, 0x94, 0x70, 0xdc, 0xec, 0x7a, 0x46, 0x6a,
0xea, 0x25, 0xe6, 0xad, 0x39, 0x6c, 0xb6, 0x70, 0x07, 0xf8, 0xc6, 0xac, 0xeb, 0x82, 0x80, 0xc6,
0xe9, 0xe8, 0x52, 0x44, 0xd0, 0x84, 0xe4, 0x34, 0xd7, 0x7c, 0x22, 0x98, 0x3c, 0xfe, 0xa2, 0x5e,
0x72, 0xd2, 0x0a, 0xaf, 0xa2, 0x56, 0xee, 0x9b, 0x75, 0x2a, 0xcc, 0x03, 0x49, 0xd4, 0x6e, 0x5d,
0x7a, 0x4d, 0x46, 0x94, 0xd8, 0x1b, 0xa8, 0x0f, 0xad, 0x31, 0x64, 0x0c, 0xe4, 0xe5, 0x0a, 0x51,
0x23, 0x2a, 0x97, 0xfd, 0xfb, 0x31, 0x3f, 0xf2, 0x56, 0x00, 0x5b, 0xfe, 0xb2, 0xe5, 0x51, 0xaf,
0xe1, 0xfb, 0xee, 0xca, 0x1a, 0xca, 0xd1, 0x38, 0x4c, 0xca, 0x8a, 0xc6, 0xfe, 0x2f, 0xdb, 0x69,
0xc2, 0x42, 0x17, 0x71, 0xd8, 0x88, 0xc6, 0x49, 0x94, 0x42, 0x4c, 0x8f, 0x6a, 0x6d, 0x97, 0xe6
};

//const uint8_t rcon [10] ={0x01, 0x02,  0x04, 0x08,  0x10,  0x20,  0x40,  0x80,  0x1b,  0x36};
uint8_t	stateText [16] = {0x79,	0x9F, 0xFD,	0x33, 0x7C,	0x8E, 0x7D, 0x9A, 0xCC, 0xD0, 0xCA, 0xC5, 0x19, 0x16, 0x33, 0x4D};
//uint8_t key[16]={0xB5, 0x2E, 0x33, 0xB1, 0x2A, 0x71, 0x1D, 0xCB, 0xF9, 0xA7, 0x8A, 0xD7, 0x39, 0xD8, 0x82, 0x08 };
#ifdef SHUFFLING
extern uint8_t shuffling_array[];
void generate_new_shuffling_array(void);
#endif
#ifdef DUMMY
void add_dummy_operations(uint8_t round);
#endif

#ifdef MASKING
uint8_t mask_in ;
uint8_t mask_out;
uint8_t m_in_inv_col [4]= {0x00,0x00,0x00,0x00};
uint8_t m_out_inv_col[4] ={0x00,0x00,0x00,0x00};
uint8_t masked_round_key[176]={0};
uint8_t masked_aes_invsbox[256];
void init_masking(void);
void generate_output_masks(void);
void remask_state(uint8_t *stateText);
void mask_state(uint8_t *stateText);
void gen_masked_roundkey_array(void);
#endif


#ifdef MASKING
void gen_masked_roundkey_array(void){
	for (uint8_t round=0; round <11; round++){
		if(round ==10){
			for(uint8_t row=0; row<4 ; row++){
			masked_round_key[(round<<4)+row + 0] = roundkeyarr[(round<<4)+row + 0]^m_out_inv_col[row]^mask_in;
			masked_round_key[(round<<4)+row + 4] = roundkeyarr[(round<<4)+row + 4]^m_out_inv_col[row]^mask_in;
			masked_round_key[(round<<4)+row + 8] = roundkeyarr[(round<<4)+row + 8]^m_out_inv_col[row]^mask_in;
			masked_round_key[(round<<4)+row+ 12] = roundkeyarr[(round<<4)+row + 12]^m_out_inv_col[row]^mask_in;
			}
		}
		else if (round>=1 && round<10){
			for(uint8_t row=0; row<4 ; row++){
			masked_round_key[(round<<4)+row + 0] = roundkeyarr[(round<<4)+row + 0]^m_in_inv_col[row]^mask_out;
			masked_round_key[(round<<4)+row + 4] = roundkeyarr[(round<<4)+row + 4]^m_in_inv_col[row]^mask_out;
			masked_round_key[(round<<4)+row + 8] = roundkeyarr[(round<<4)+row + 8]^m_in_inv_col[row]^mask_out;
			masked_round_key[(round<<4)+row+ 12] = roundkeyarr[(round<<4)+row + 12]^m_in_inv_col[row]^mask_out;
			}
		}
		else{
			for(uint8_t row=0; row<4 ; row++){
			masked_round_key[(round<<4)+row + 0] = roundkeyarr[(round<<4)+row + 0]^mask_in;
			masked_round_key[(round<<4)+row + 4] = roundkeyarr[(round<<4)+row + 4]^mask_in;
			masked_round_key[(round<<4)+row + 8] = roundkeyarr[(round<<4)+row + 8]^mask_in;
			masked_round_key[(round<<4)+row+ 12] = roundkeyarr[(round<<4)+row + 12]^mask_in;
			}	
		}
	}
}
void mask_state(uint8_t *stateText){
	for(uint8_t i=0 ; i<4 ; i++){
		stateText[i+0] ^= m_out_inv_col[i];  		
		stateText[i+4] ^= m_out_inv_col[i];
		stateText[i+8] ^= m_out_inv_col[i];
		stateText[i+12] ^= m_out_inv_col[i];
	}
}


void remask_state(uint8_t *stateText){
	
	for (uint8_t i =0 ; i<4 ;i++){
		stateText[i+0] ^=m_out_inv_col[i]^mask_in; 
		stateText[i+4] ^=m_out_inv_col[i]^mask_in; 
		stateText[i+8] ^=m_out_inv_col[i]^mask_in; 
		stateText[i+12] ^=m_out_inv_col[i]^mask_in; 
	}
}


void generate_output_masks(void)
{
    //Creating output masks starts here.
    m_out_inv_col[0] = gf256mul(m_in_inv_col[0],0x0e)^gf256mul(m_in_inv_col[1],0x0b)^gf256mul(m_in_inv_col[2],0x0d)^gf256mul(m_in_inv_col[3],0x09);
						
    m_out_inv_col[1] = gf256mul(m_in_inv_col[0],0x09)^gf256mul(m_in_inv_col[1],0x0e)^gf256mul(m_in_inv_col[2],0x0b)^gf256mul(m_in_inv_col[3],0x0d);
						
    m_out_inv_col[2] = gf256mul(m_in_inv_col[0],0x0d)^gf256mul(m_in_inv_col[1],0x09)^gf256mul(m_in_inv_col[2],0x0e)^gf256mul(m_in_inv_col[3],0x0b);
						
    m_out_inv_col[3] = gf256mul(m_in_inv_col[0],0x0b)^gf256mul(m_in_inv_col[1],0x0d)^gf256mul(m_in_inv_col[2],0x09)^gf256mul(m_in_inv_col[3],0x0e);
    //Creating output masks ends here.
}
void init_masking(void){
	
	mask_in = Rng_rand[4];
	mask_out= mask_in;
	
	//repalce rand() by TRNG
	m_in_inv_col[0]= Rng_rand[0];
	m_in_inv_col[1]= Rng_rand[1];
	m_in_inv_col[2]= Rng_rand[2];
	m_in_inv_col[3]= Rng_rand[3];
	
	/*
	mask_i_1 = Rng_rand[4];
	mask_out_1= mask_in_1;
	
	mask_i_2 = Rng_rand[5];
	mask_out_2= mask_in_2;
	
	mask_i_3 = Rng_rand[6];
	mask_out_3= mask_in_3;
	
	mask_i_4 = Rng_rand[7];
	mask_out_4= mask_in_4;
	*/
	
	// compute output masks for each coloumn in inverse sub bytes  from input masks
	generate_output_masks();
	
	for(uint16_t i =0 ; i<256 ; i++){
		masked_aes_invsbox[i^mask_out] = aes_invsbox[i]^mask_in;
	}
}
#endif
        
//Adding the round key with shuffling
void add_round_key(uint8_t* stateText,uint8_t roundnum){
	 for (uint8_t i =0; i<16; i++){
		#ifdef SHUFFLING
			stateText[shuffling_array[i]] ^= roundkeyarr[16*roundnum + shuffling_array[i]];
		#endif
		
		#ifdef MASKING
			stateText[i] ^= masked_round_key[16*roundnum + i];
		#endif
		
		#if defined(MASKING) && defined(SHUFFLING)
			stateText[shuffling_array[i]] ^= masked_round_key[16*roundnum + shuffling_array[i]];
		#endif
		
		#ifdef NORMAL
			stateText[i] ^= roundkeyarr[16*roundnum + i];
		#endif
	}
	//printf("\nAfter add round %d is \n",roundnum);
	//for (uint8_t i=0; i<16; i++){
	//		printf("%x ",stateText[i]);
	//}
}
//Inverse subbytes 
void inverse_subbytes(uint8_t* stateText, uint8_t roundNum){
		#ifdef DUMMY
			uint8_t max_ops =80;
			static uint8_t completed_ops = 0;
			uint8_t current_ops_count =0;
			uint8_t number_of_ops =0;
			uint8_t dummy_output =0;

				if(roundNum==0){number_of_ops = max_ops - completed_ops;}
				else{	srand(roundNum);
					number_of_ops =Rng_rand[roundNum]%4;}

				while(completed_ops < max_ops && current_ops_count<number_of_ops){
					dummy_output ^= aes_invsbox[dummy_output];
					completed_ops++;
					current_ops_count++;
			}
		#endif
	
	for(uint8_t i =0; i<16; i++){	
		#ifdef SHUFFLING
			stateText[shuffling_array[i]] = aes_invsbox[stateText[shuffling_array[i]]];
		#endif
		
		#if defined(MASKING) && defined(SHUFFLING)
			stateText[shuffling_array[i]] = masked_aes_invsbox[stateText[shuffling_array[i]]];
		#endif
		
		#ifdef MASKING 
			stateText[i] = masked_aes_invsbox[stateText[i]];
		#endif
		#ifdef NORMAL 
			stateText[i] = aes_invsbox[stateText[i]];
		#endif
		
		#ifdef DUMMY
				while(completed_ops < max_ops && current_ops_count<number_of_ops){
					dummy_output ^= aes_invsbox[dummy_output];
					completed_ops++;
					current_ops_count++;
			}
		#endif

	}
	//printf("\nAfter inverse subytes is \n");
	//for (uint8_t i=0; i<16; i++){
	//	printf("%x ",stateText[i]);
	//}
}
void inverse_shift_rows(uint8_t* stateText){
		uint8_t temp0 = stateText[13];
		uint8_t temp1 = stateText[14];
		uint8_t temp2 = stateText[10];
		uint8_t temp3= stateText[3];

			stateText[13]=stateText[9];
			stateText[9]=stateText[5];
			stateText[5]=stateText[1];
			stateText[1]=temp0;

			stateText[14]=stateText[6];
			stateText[10]=stateText[2];
			stateText[6]=temp1;
			stateText[2]=temp2;

			stateText[3]=stateText[7];
			stateText[7]=stateText[11];
			stateText[11]=stateText[15];
			stateText[15]=temp3;

		//	printf("\nInverse Shift bytes is \n");
		//	for (uint8_t i=0; i<16; i++){
		//	printf("%x ",stateText[i]);
		//	}

}
/*
 *  This Xtime funcationlity is read and taken from https://www.emsec.ruhr-uni-bochum.de/media/crypto/attachments/files/2010/04/ms_hoheisel.pdf and 
 * https://github.com/kokke/tiny-AES-c/blob/master/aes.c
 */
uint8_t xtime(uint8_t x)
{
  return ((x<<1) ^ (((x>>7) & 1) * 0x1b)); 
}

/*
uint8_t gf256mul(uint8_t a,uint8_t b) {
	//if (a==0)
		//return 0;
	//else
		//return gf_antilog_table[(gf_log_table[a]+ gf_log_table[b])%255];
}*/

 void inverse_mix_coloumns(uint8_t* s){
	for (uint8_t i=0 ; i<4 ; i++){
		 uint8_t index = 4*i;
		 uint8_t temp0 = s[index];
		 uint8_t temp1 = s[index+1];
		 uint8_t temp2 = s[index+2];
		s[index]   =  gf256mul(s[index],0x0e) ^ gf256mul(s[index+1],0x0b) ^ gf256mul(s[index+2],0x0d) ^ gf256mul(s[index+3],0x09);
		s[index+1] =  gf256mul(temp0,0x09) ^ gf256mul(s[index+1],0x0e) ^ gf256mul(s[index+2],0x0b) ^ gf256mul(s[index+3],0x0d);
		s[index+2] =  gf256mul(temp0,0x0d) ^ gf256mul(temp1,0x09) ^ gf256mul(s[index+2],0x0e) ^ gf256mul(s[index+3],0x0b);
		s[index+3] =  gf256mul(temp0,0x0b) ^ gf256mul(temp1,0x0d) ^ gf256mul(temp2,0x09) ^ gf256mul(s[index+3],0x0e);

//	printf("\nafter inverse mix coloumns is \n");
//	for (uint8_t i=0; i<16; i++){
//			printf("%x ",s[i]);
//			}

	 }

}
#ifdef GEN_ROUNDKEY_FUNC
void gen_roundkey(uint8_t* key , uint8_t* k,  const uint8_t* sboxarr,  const uint8_t* rcont){

	memcpy(k,key,16);

	for(uint8_t i=16; i<176; i+=4){
			k[i]=k[i-4];
			k[i+1]=k[i-4+1];
			k[i+2]=k[i-4+2];
			k[i+3]=k[i-4+3];
		if (i % 16 == 0){

			// cyclic shift of first four bytes
			uint8_t temp = k[i];
			k[i] = k[i+1];
			k[i+1] = k[i+2];
			k[i+2] = k[i+3];
			k[i+3] = temp;

			//printf(" After Cyclic Shift %d is %x %x %x %x \n",i,k[i],k[i+1],k[i+2],k[i+3]);

			// subbytes for i.e. is replacing the each bytes from array
			k[i]   = sboxarr[k[i]];
			k[i+1] = sboxarr[k[i+1]];
			k[i+2] = sboxarr[k[i+2]];
			k[i+3] = sboxarr[k[i+3]];

			//printf(" After Subytes %d is %x %x %x %x \n",i,k[i],k[i+1],k[i+2],k[i+3]);


			//multiplication with rcon constant array
			k[i]   ^=rcont[(i/16)-1];
			//printf(" After rcon %d is %x %x %x %x \n",i,k[i],k[i+1],k[i+2],k[i+3]);
		}
			// xor with previous word
			k[i]   ^= k[i-16];
			k[i+1] ^= k[i-16 +1];
			k[i+2] ^=k[i-16 +2];
			k[i+3] ^=k[i-16 +3];
			//printf(" After xor %d is %x %x %x %x \n",i,k[i],k[i+1],k[i+2],k[i+3]);

	}
//	printf("\n");
	for (uint8_t i=0; i<176; i+=16){
//		printf(" %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x \n",k[i],k[i+1],k[i+2],k[i+3],k[i+4],k[i+5],k[i+6],k[i+7],k[i+8],k[i+9],k[i+10],k[i+11],k[i+12],k[i+13],k[i+14],k[i+15]);
		}
}
#endif
//generating the array for shuffling of operations(add round key and inverse sbox)
#ifdef SHUFFLING
uint8_t shuffling_array[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
void generate_new_shuffling_array(void) {

	uint8_t i = 15; // array end
	uint8_t j = 0;  // init value
	do
	{	j = Rng_rand[i]%(i+1); 		
		//swap the last element with element at random index
		uint8_t temp = shuffling_array[i];
		shuffling_array[i] = shuffling_array[j];
		shuffling_array[j] = temp;
		--i;
	}while(i!=0);
}
#endif

void aes_dec_128(uint8_t* state){

	uint8_t roundCount=10;
	
	#ifdef DUMMY
	//	Rng_fill(RNG_NUMBER);
	#endif
	//Generating the shuffling _array[] for randomness in operation
	#ifdef SHUFFLING
		//Rng_fill(RNG_NUMBER);
		generate_new_shuffling_array();
	#endif
	
	#ifdef MASKING
		//Rng_fill(RNG_NUMBER);
		init_masking();
		gen_masked_roundkey_array();
		mask_state(state);
	#endif
	  
	//add round key
	TRIGGER_SET();
	add_round_key(state,10);
	roundCount--;
	  
	// first 9 rounds
	for (; roundCount > 0 ; roundCount--){
		inverse_shift_rows(state);
		inverse_subbytes(state,roundCount);
		add_round_key(state,roundCount);
		inverse_mix_coloumns(state);
		//remasking
		#ifdef MASKING
		remask_state(state);
		#endif
	}

	//final round
	#ifdef SHUFFLING
	//Rng_fill(RNG_NUMBER);
	generate_new_shuffling_array();
	#endif
	inverse_shift_rows(state);
	inverse_subbytes(state,roundCount);
	add_round_key(state,roundCount);
	TRIGGER_CLR();
}
