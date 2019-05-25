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

const uint8_t aes_sbox[256] = {
 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
 0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

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

const uint8_t rcon [10] ={0x01, 0x02,  0x04, 0x08,  0x10,  0x20,  0x40,  0x80,  0x1b,  0x36}; 
uint8_t	stateText [16] ={0x39, 0x25, 0x84, 0x1d, 0x02, 0xdc, 0x09, 0xfb, 0xdc, 0x11, 0x85, 0x97, 0x19, 0x6a, 0x0b, 0x32};
uint8_t roundkeyarr[176];
uint8_t key[16]={0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};


void add_round_key(uint8_t* stateText, uint8_t* roundKey, uint8_t roundnum){
	 for (uint8_t i =0; i<16; i++){
		 stateText[i] ^= roundKey[16*roundnum + i];
	}
	//printf("\nAfter add round %d is \n",roundnum);
	//for (uint8_t i=0; i<16; i++){
	//		printf("%x ",stateText[i]);
	//}
}
void inverse_subbytes(uint8_t* stateText,  const uint8_t* invsbox){
	for(uint8_t i =0; i<16; i++){
		stateText[i] = invsbox[stateText[i]];
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
uint8_t gf256mul(uint8_t in1, uint8_t in2){
    uint8_t prod = 0;
    for (; in2; in2 >>= 1) {
        if (in2 & 1)
            prod ^= in1;
        if (in1 & 0x80)
            in1 = (in1 << 1) ^ 0x1b;
        else
            in1 <<= 1;
    }
    return prod;
}

 void inverse_mix_coloumns(uint8_t* s){
	 for (uint8_t i=0 ; i<4 ; i++){
		 uint8_t index = 4*i; 
		 uint8_t temp0 = s[index];
		 uint8_t temp1 = s[index+1];
		 uint8_t temp2 = s[index+2];
	s[index] =  gf256mul(0x0e , s[index]) ^ gf256mul(0x0b,s[index+1])^gf256mul(0x0d, s[index+2])^gf256mul(0x09 , s[index+3]);
	 s[index+1] =  gf256mul(0x09 , temp0) ^ gf256mul(0x0e,s[index+1])^gf256mul(0x0b, s[index+2])^gf256mul(0x0d , s[index+3]);
	 s[index+2] =  gf256mul(0x0d , temp0) ^ gf256mul(0x09,temp1)^gf256mul(0x0e, temp2)^gf256mul(0x0b , s[index+3]);
	 s[index+3] =  gf256mul(0x0b , temp0) ^ gf256mul(0x0d,temp1)^gf256mul(0x09, temp2)^gf256mul(0x0e , s[index+3]);
	
//	printf("\nafter inverse mix coloumns is \n");
//	for (uint8_t i=0; i<16; i++){
//			printf("%x ",s[i]);
//			}

	 }
	 
}

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
	//printf("\n");
	//for (uint8_t i=0; i<176; i+=16){
	//		printf("Generated key %d is %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x \n",i/			16,k[i],k[i+1],k[i+2],k[i+3],k[i+4],k[i+5],k[i+6],k[i+7],k[i+8],k[i+9],k[i+10],k[i+11],k[i+12],k[i+13],k[i+14],k[i+15]);
	//		}
}

void aes_dec_128(uint8_t* state ,uint8_t* roundkeyarray ){
	  uint8_t roundCount=10;
	  //add round key
	  
	   add_round_key(stateText,roundkeyarr, 10);
	   roundCount--;
	  // first 9 rounds
	   for (; roundCount > 0 ; roundCount--){
			inverse_shift_rows(stateText);
			inverse_subbytes(stateText,aes_invsbox);
			add_round_key(stateText,roundkeyarr,roundCount);
			inverse_mix_coloumns(stateText);
		}
		
		//final round
		inverse_shift_rows(stateText);
		inverse_subbytes(stateText,aes_invsbox);
		add_round_key(stateText,roundkeyarr,roundCount);
		
		
		
}
