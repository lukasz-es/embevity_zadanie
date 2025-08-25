#include <stdio.h>
#include "accelerometer.h"

int main()
{
    unsigned char opType=4, rxoptype;
    int dataLen=20, rxdatalen;
    unsigned char data[100]="Text from app", rxdata[100];
    
    //write_queue_send(opType, dataLen, data);
    //read_queue_receive(&rxoptype, &rxdatalen, rxdata);
	data[0] = 0xDE;
	data[1] = 0xBE;
	i2c_registers_write(0x55, 2, data);
	//i2c_registers_read(0x55, 6,  data);
	accel_start_acquisition();
	
    while ( i2c_registers_read(ICM_42670_INT_STATUS_DRDY_REG, 1,  data) != I2C_FINISH_SIM)
	{
		printf("%X", data[0]);
	}
	
	accel_stop_acquisition();
    printf("Hello World!\n");
    return 0;
}
