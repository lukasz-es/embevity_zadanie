#include <stdio.h>
#include "accelerometer.h"

int main()
{
    i2c_initialize_master();
    unsigned char opType=4, rxoptype;
    int dataLen=20, rxdatalen;
    unsigned char data[100]="Text from app", rxdata[100];
    
    write_queue_send(opType, dataLen, data);
    read_queue_receive(&rxoptype, &rxdatalen, rxdata);
    
    i2c_close();
    printf("Hello World!\n");
    return 0;
}
