#ifndef RNG_H_
#define RNG_H_

#define RNG_SEED_SIZE		UINT8_C(32)

#define RNG_MAX				UINT8_C(16)

#define RNG_MAX_POOL		UINT16_C(1024)

#define RNG_OK				UINT8_C(1)

#define RNG_NOK				UINT8_C(0)

#define RNG_NUMBER			UINT8_C(6)


void Rng_init(void);
void Rng_reseed(void);
uint8_t Rng_fill(uint8_t );

#endif
