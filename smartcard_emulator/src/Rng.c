#include <stdint.h>
#include <stdio.h>
#include "fkein.h"
#include "Seed.h"
#include "Rng.h"


extern volatile threefish_param_t* tparam;
uint8_t Rng_seed[RNG_SEED_SIZE] = {0};
uint8_t Rng_rand[RNG_MAX] = {0};
uint16_t Rng_randPoolIdx = 0;
void Rng_init(void)
{
	skein_256_256_default_iv();
	Seed_readEEprom();
	skein_rand_seed(tparam, (char*) Rng_seed);
}


void Rng_reseed(void)
{
	if(Rng_randPoolIdx == RNG_MAX_POOL)
	{
		Seed_readEEprom();
		skein_rand_reseed(tparam, (char*) Rng_seed);	
		Rng_randPoolIdx = 0;
	}
}

uint8_t Rng_fill(uint8_t count)
{
	uint8_t ret = RNG_OK;
	for (uint8_t idx = 0; idx<count;idx++)
	{
		if(Rng_randPoolIdx < RNG_MAX_POOL)
		{
			Rng_rand[idx] = skein_rand(tparam);
			Rng_randPoolIdx++;
		}
		else
		{
			ret = RNG_NOK;
		}
	}
	return ret;
}
