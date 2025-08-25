#include "imui2c.h"

IMUI2C::IMUI2C()
{
    opcnt=5;
	haveDataToSend=true;
	i2c_initialize_slave();
	i2CLoop();
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
	
	while (haveDataToSend)
	{
		read_queue_receive(&rxoptype, &rxdatalen, rxdata);
		
		switch (rxoptype)
		{
			case QUEUE_IPC_MSG_TYPE_WRITE:
				break;
				
			case QUEUE_IPC_MSG_TYPE_READ:
				if (--opcnt <= 0)
				{
					haveDataToSend = false;
				}
				if (haveDataToSend)
				{
					write_queue_send(QUEUE_IPC_MSG_TYPE_READ, 17, (unsigned char *)"Text from second");
				}
				else
				{
					write_queue_send(QUEUE_IPC_MSG_TYPE_DONE, 0, rxdata);
				}
				break;
				
			default:
				break;
		}
	}
}
