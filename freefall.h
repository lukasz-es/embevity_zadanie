#ifndef FREEFALL_H
#define FREEFALL_H

#include "accelerometer.h"
#include <math.h>

typedef struct
{
	int already_triggered;
	int min_samples_no;
	unsigned long int samples_buffer;
	unsigned long int samples_buffer_mask;
	float threshold;
} FreeFallDataCache;

void freefall_init(FreeFallDataCache *cache, int min_samples, float threshold);
int freefall_check(FreeFallDataCache *cache, AccelData *data);
unsigned long int frontFill(int bits);

#endif // FREEFALL_H
