#include "i2c_emulated.h"

IPCMessageQueueHandles write_queue_handle, read_queue_handle;

int i2c_registers_read(unsigned char registerAddr, unsigned char registersNo, unsigned char *data)
{
    /* NOTE: I'm skipping device address here as it's only emulation */
	
	unsigned char device_data[2];
	unsigned char optype;
	int regsRead;
	
	device_data[0] = registerAddr;
	device_data[1] = registersNo;
	if (write_queue_send(QUEUE_IPC_MSG_TYPE_READ, 2, device_data) != I2C_SUCCESS)
	{
		return I2C_WRITE_FAILED;
	}
	
	if (read_queue_receive(&optype, &regsRead, data) != I2C_SUCCESS)
	{
		return I2C_READ_FAILED;
	}
	
	if (optype == QUEUE_IPC_MSG_TYPE_DONE)
	{
		return I2C_FINISH_SIM;
	}
	
	if (regsRead != registersNo)
	{
		return I2C_READ_FAILED;
	}
		
	return I2C_SUCCESS;
}

int i2c_registers_write(unsigned char registerAddr, unsigned char registersNo, unsigned char *data)
{
	/* NOTE: I'm skipping device address here as it's only emulation */
	
	unsigned char device_data[MAX_IPC_TOTAL_SIZE]; /* This could be malloced but IMHO const size version is better */
	int i;
	
	if (registersNo > QUEUE_MAX_IPC_DATA_SIZE)
	{
		return I2C_WRITE_FAILED;
	}
	
	device_data[0] = registerAddr;
	memcpy(&device_data[1], data, registersNo);
	
	return write_queue_send(QUEUE_IPC_MSG_TYPE_WRITE, (registersNo+1), device_data);
}

int i2c_initialize_master(void)
{
    if ( (queue_init(&write_queue_handle, QUEUE_PROJ_ID_WRITE) != I2C_SUCCESS) || (queue_init(&read_queue_handle, QUEUE_PROJ_ID_READ) != I2C_SUCCESS) )
    {
        return I2C_CREATE_FAILED;
    }
    return I2C_SUCCESS;
}

int i2c_initialize_slave(void)
{
    if ( (queue_init(&write_queue_handle, QUEUE_PROJ_ID_READ) != I2C_SUCCESS) || (queue_init(&read_queue_handle, QUEUE_PROJ_ID_WRITE) != I2C_SUCCESS) )
    {
        return I2C_CREATE_FAILED;
    }
    return I2C_SUCCESS;
}

int i2c_close(void)
{
    if ( (queue_destroy(&write_queue_handle) != I2C_SUCCESS) || (queue_destroy(&read_queue_handle) != I2C_SUCCESS) )
    {
        return I2C_DESTROY_FAILED;
    }
    return I2C_SUCCESS;
}

int queue_init(IPCMessageQueueHandles *handles, int queue_proj_id)
{
    /* TODO: error checks */
    handles->queue_key = ftok(QUEUE_ID_FILE, queue_proj_id);
    handles->queue_id = msgget(handles->queue_key, (QUEUE_ACCESS_MOD | IPC_CREAT));
    handles->queue_initialized=1;
    return I2C_SUCCESS;
}

int queue_destroy(IPCMessageQueueHandles *handles)
{
    msgctl(handles->queue_id, IPC_RMID, NULL);
    handles->queue_initialized = 0;
    return I2C_SUCCESS;
}

int write_queue_send(unsigned char opType, int dataLen, unsigned char *data)
{
	if (!write_queue_handle.queue_initialized)
    {
        return I2C_WRITE_FAILED;
    }

    if (dataLen > QUEUE_MAX_IPC_DATA_SIZE)
    {
        return I2C_WRITE_FAILED;
    }

    IPCMessage msg;
    msg.messageType = QUEUE_IPC_MSG_TYPE;
    msg.data[QUEUE_IPC_OP_TYPE_POS] = opType;
    msg.data[QUEUE_IPC_DATA_LEN_POS] = dataLen;
    memcpy(&msg.data[QUEUE_IPC_PAYLOAD_START_POS], data, dataLen);

    if (msgsnd(write_queue_handle.queue_id, &msg, sizeof(msg), QUEUE_IPC_FLAGS) != 0)
    {
        return I2C_WRITE_FAILED;
    }

    return I2C_SUCCESS;
}

int read_queue_receive(unsigned char *opType, int *dataLen, unsigned char *data)
{	
	if (!read_queue_handle.queue_initialized)
    {
        return I2C_READ_FAILED;
    }

    IPCMessage msg;

    if (msgrcv(read_queue_handle.queue_id, &msg, sizeof(msg), QUEUE_IPC_MSG_TYPE, QUEUE_IPC_RECEIVE_FLAGS) < 0)
    {
        return I2C_READ_FAILED;
    }

    if (msg.data[QUEUE_IPC_DATA_LEN_POS] > QUEUE_MAX_IPC_DATA_SIZE)
    {
        return I2C_READ_FAILED;
    }

    *opType=msg.data[QUEUE_IPC_OP_TYPE_POS];
    *dataLen=msg.data[QUEUE_IPC_DATA_LEN_POS];
    memcpy(data, &msg.data[QUEUE_IPC_PAYLOAD_START_POS], msg.data[QUEUE_IPC_DATA_LEN_POS]);

    return I2C_SUCCESS;
}
