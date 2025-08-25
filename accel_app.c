#include <stdio.h>
#include "accelerometer.h"

int main()
{
    unsigned char opType=4, rxoptype;
    int dataLen=20, rxdatalen;
    unsigned char data[100]="Text from app", rxdata[100];

	accel_start_acquisition();

	char have_data=1;

	while(have_data)
	{
		i2c_registers_read(ICM_42670_INT_STATUS_DRDY_REG, 1,  rxdata);
		
		have_data = (rxdata[0] & ICM_42670_INT_STATUS_DRDY_BITMASK);
		printf("Status %X\n", have_data);

		if (have_data)
		{
			i2c_registers_read(ICM_42670_ACCEL_DATA_REGS, 1,  rxdata);
		}
		
	}
	
	accel_stop_acquisition();
    printf("Hello World!\n");
    return 0;
}
