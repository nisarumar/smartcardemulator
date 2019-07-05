#ifndef RNG_H_
#define RNG_H_

#define MASKING

#define RNG_SEED_SIZE		UINT8_C(32)

#if defined(SHUFFLING) && defined(MASKING)
	#define RNG_MAX				UINT8_C(26)
#else
	#define RNG_MAX				UINT8_C(16)
#endif

#define RNG_MAX_POOL		UINT16_C(1024)

#define RNG_OK				UINT8_C(1)

#define RNG_NOK				UINT8_C(0)

#if defined(SHUFFLING) && defined(MASKING)
	#define RNG_NUMBER			UINT8_C(26)
#elif defined(MASKING)  && !defined(SHUFFLING)
	#define RNG_NUMBER			UINT8_C(10)
#elif defined(SHUFFLING)  && !defined(MASKING)
	#define RNG_NUMBER			UINT8_C(16)
#endif

#ifdef DUMMY
	#define RNG_NUMBER			UINT8_C(10)
#endif
#ifdef NORMAL
	#define RNG_NUMBER			UINT8_C(0)
#endif

void Rng_init(void);
void Rng_reseed(void);
uint8_t Rng_fill(uint8_t );

#endif
