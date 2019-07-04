#ifndef SEED_H_
#define SEED_H_


void Seed_init(void);
void Seed_deInit(void);
void Seed_whitening(void);
void Seed_renewSeed(void);
void Seed_resume(void);
void Seed_pause(void);
void Seed_writeEeprom(void);
void Seed_readEEprom(void);
void Seed_seedReInit(void);
/* 2^n bytes to whiten 32 bits of whitened entropy */
#define SEED_RAW_TO_WHITE_BYTES		UINT8_C(5)
/* size of array to hold Raw entropy */
#define SEED_RAW_BUFFER_SIZE		UINT8_C(1)<<SEED_RAW_TO_WHITE_BYTES
/* pool size of entropy */
#define SEED_BUFFER_SIZE 			UINT8_C(4)

#define LOW_ENTROPY_RAND_SIZE		UINT8_C(16)
											
/* control signals for seed */

/* control signal to seed srandom function of C*/

#define SEED_CONTROL_SRAND			5

/* control signal to renew seed */

#define SEED_CONTROL_RENEW_SEED		6


/* status signals for seed */

/* indicate if raw entropy buffer is full */
#define SEED_STATUS_FIFO_FULL	 	0

/* indicate if entropy pool is complete */
#define SEED_STATUS_WHITE_DONE		1

#define SEED_STATUS_WRITE_DONE		2
extern uint8_t randNotReady;

#endif
