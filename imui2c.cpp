#include "imui2c.h"
#include <stdio.h>

IMUI2C::IMUI2C()
{
    opcnt=8;
	haveDataToSend=true;
	unsigned char tempArray[2];
	float val;

	i2c_initialize_slave();
}

IMUI2C::~IMUI2C()
{
    i2c_close();
}

void IMUI2C::i2CLoop()
{
    unsigned char rxoptype;
    int rxdatalen;
    unsigned char rxdata[MAX_IPC_TOTAL_SIZE];
	
	unsigned char txoptype;
    int txdatalen;
    unsigned char txdata[MAX_IPC_TOTAL_SIZE];
	
	const int registerAddrPos = 0;

	while(needToWork)
	{
		read_queue_receive(&rxoptype, &rxdatalen, rxdata);
		
		switch (rxoptype)
		{
			case QUEUE_IPC_MSG_TYPE_WRITE:
			switch(rxdata[registerAddrPos])
			{
				case ICM_42670_GYRO_CONFIG0_REG:
				case ICM_42670_ACCEL_CONFIG0_REG:
				case ICM_42670_PWR_MGMT0_REG:
				case ICM_42670_INT_STATUS_DRDY_REG:
					std::cout << "Config received \n";
					break;
			}
				break;
				
			case QUEUE_IPC_MSG_TYPE_READ:

				switch(rxdata[0])
				{
					case ICM_42670_ACCEL_DATA_REGS:
					case ICM_42670_GYRO_DATA_REGS:
					std::cout << "Send accel\n";
					if (--opcnt <= 0)
					{
						haveDataToSend = false;
					}
					write_queue_send(QUEUE_IPC_MSG_TYPE_READ, 17, (unsigned char *)"Text from second");
					break;
					
					case ICM_42670_INT_STATUS_DRDY_REG:
					std::cout << "Send status\n";
					if (haveDataToSend)
					{
						txdata[0] = 1;
					}
					else
					{
						txdata[0] = 0;
						needToWork=false;
					}
					write_queue_send(QUEUE_IPC_MSG_TYPE_READ, 1, txdata);
					break;
					
					default:
					break;
				}
				
				break;
				
			default:
				break;
		}
	}
}

void IMUI2C::floatToAccelValues(unsigned char *array, const float val, const float range) const
{
	const unsigned int halfRangeValue = 32767;
	const unsigned int splitPoint = 256;
	unsigned int tmp = (unsigned int)(( (val / range) + 1 ) * halfRangeValue);
	
	// Byte order is: MSB LSB
	array[0] = tmp / splitPoint;
	array[1] = tmp % splitPoint;
}

