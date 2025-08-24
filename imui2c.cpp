#include "imui2c.h"

IMUI2C::IMUI2C()
{
    i2c_initialize_slave();
}

IMUI2C::~IMUI2C()
{
    i2c_close();
}

void IMUI2C::i2CTest()
{
    unsigned char opType=4, rxoptype;
    int dataLen=20, rxdatalen;
    unsigned char data[100]="Text from second", rxdata[100];
    
    read_queue_receive(&rxoptype, &rxdatalen, rxdata);
    write_queue_send(opType, dataLen, data);
}
