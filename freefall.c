#include "freefall.h"

void freefall_init(FreeFallDataCache *cache, int min_samples, float threshold)
{
	cache->already_triggered = 0;
	cache->min_samples_no = min_samples;
	cache->samples_buffer = 0;
	cache->samples_buffer_mask = frontFill(sizeof(unsigned long int)-min_samples);
	cache->threshold = threshold;
}

int freefall_check(FreeFallDataCache *cache, AccelData *data)
{
	return 0;
}

unsigned long int frontFill(int bits)
{
	unsigned long int result=1;
	int i;

    for (i=0; i<(bits - 1); i++)
    {
        result <<=1;
        result|=1;
    }

    result=~result;
	
	return result;
}
