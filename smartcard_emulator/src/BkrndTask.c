#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "IO.h"
#include "IoStream.h"
#include "Fifo.h"
#include "Art.h"
#include "Atr.h"
#include "Seed.h"
#include "Rng.h"

void BkrndTask(void)
{
	static uint8_t val=0;

/*	if (val == 3)
	{
		Rng_reseed();
	}*/
	if (val == 2)
	{
		Seed_seedReInit();
		val++;
	}
	if (val == 1)
	{
		Seed_writeEeprom();
		val++;
	}
	if (val == 0)
	{
		Seed_whitening();
		val++;
	}
	if (val > 2/*3*/)
	{
		val = 0;
	}
}
