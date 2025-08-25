#include <stdio.h>
#include "accelerometer.h"

int main()
{
    unsigned char rxdata[MAX_IPC_TOTAL_SIZE];

	AccelData data;

	accel_start_acquisition();

	char have_data=1;

	while(have_data)
	{
		i2c_registers_read(ICM_42670_INT_STATUS_DRDY_REG, 1,  rxdata);
		
		have_data = (rxdata[0] & ICM_42670_INT_STATUS_DRDY_BITMASK);
		printf("Status %X\n", have_data);

		if (have_data)
		{
			i2c_registers_read(ICM_42670_ACCEL_DATA_REGS, 12,  rxdata);
			data.accel.x = floatFromAccelValues(&rxdata[0], 2.0);
			data.accel.y = floatFromAccelValues(&rxdata[2], 2.0);
			data.accel.z = floatFromAccelValues(&rxdata[4], 2.0);
			data.gyro.x = floatFromAccelValues(&rxdata[6], 2.0);
			data.gyro.y = floatFromAccelValues(&rxdata[8], 2.0);
			data.gyro.z = floatFromAccelValues(&rxdata[10], 2.0);
			
			printf ("%f %f %f\n", data.accel.x, data.accel.y, data.accel.z);
		}
		
	}
	
	accel_stop_acquisition();
    printf("Hello World!\n");
    return 0;
}
