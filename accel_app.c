#include <stdio.h>
#include "accelerometer.h"

int main()
{
	AccelData data;

	accel_start_acquisition();

	while(accel_get_data(&data) == ACCEL_SUCCESS)
	{
	}
	
	accel_stop_acquisition();
    printf("Hello World!\n");
    return 0;
}
