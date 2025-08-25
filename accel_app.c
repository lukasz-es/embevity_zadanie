#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "freefall.h"
#include "argshelper.h"

int main()
{
	AccelData data;
	FreeFallDataCache cache;
	
	int freefall_reported = 0;
	int samples_counter = 0;
	
	freefall_init(&cache, 5, 0.1);
	
	accel_start_acquisition();

	while(accel_get_data(&data) == ACCEL_SUCCESS)
	{
		if (freefall_check(&cache, &data) && (!freefall_reported))
		{
			printf("Free fall detected at sample %d\n", samples_counter);
			freefall_reported = 1;
		}
		else
		{
			samples_counter++;
		}
	}
	
	accel_stop_acquisition();
    printf("App finished\n");
    return 0;
}
