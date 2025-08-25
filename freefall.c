#include "freefall.h"

void freefall_init(FreeFallDataCache *cache, int min_samples, float threshold)
{
	cache->already_triggered = 0;
	cache->min_samples_no = min_samples;
	cache->samples_buffer = 0;
	cache->samples_buffer_mask = frontFill(min_samples);
	cache->threshold = threshold;
}

int freefall_check(FreeFallDataCache *cache, AccelData *data)
{
	/* Keep state if fall is previously detected */
	if (cache->already_triggered)
	{
		return 1;
	}
	
	cache->samples_buffer <<= 1;
	
	if ( (fabs(data->accel.x )< cache->threshold) && (fabs(data->accel.y) < cache->threshold) && (fabs(data->accel.z) < cache->threshold) )
	{
		cache->samples_buffer |= 1;
	}
	else
	{
		cache->samples_buffer &= (~1);
	}
	
	unsigned long int temp = ~(cache->samples_buffer | cache->samples_buffer_mask);
	
	if (temp == 0)
	{
		cache->already_triggered = 1;
		return 1;
	}
	
	return 0;
}

unsigned long int frontFill(int bits)
{
	unsigned long int result=1;
	int i;

    for (i=0; i<(bits-1); i++)
    {
		result <<=1;
        result|=1;
    }

    result=~result;
	
	return result;
}
