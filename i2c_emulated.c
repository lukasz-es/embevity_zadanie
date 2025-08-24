#include "i2c_emulated.h"

static IPCMessageQueueHandles write_queue_handle, read_queue_handle;

int i2c_registers_read(unsigned char registerAddr, unsigned char registersNo, unsigned char *data)
{
    return I2C_NOT_IMPLEMENTED_YET;
}

int i2c_registers_write(unsigned char registerAddr, unsigned char registersNo, unsigned char *data)
{
    return I2C_NOT_IMPLEMENTED_YET;
}

int i2c_initialize(void)
{
    if ( (queue_init(&write_queue_handle, QUEUE_PROJ_ID_WRITE) != I2C_SUCCESS) || (queue_init(&read_queue_handle, QUEUE_PROJ_ID_READ) != I2C_SUCCESS) )
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

static inline int queue_init(IPCMessageQueueHandles *handles, int queue_proj_id)
{
    /* TODO: error checks */
    handles->queue_key = ftok(QUEUE_ID_FILE, queue_proj_id);
    handles->queue_id = msgget(handles->queue_key, (QUEUE_ACCESS_MOD | IPC_CREAT));
    handles->queue_initialized=1;
    return I2C_SUCCESS;
}

static inline int queue_destroy(IPCMessageQueueHandles *handles)
{
    msgctl(handles->queue_id, IPC_RMID, NULL);
    handles->queue_initialized = 0;
    return I2C_SUCCESS;
}

static inline int write_queue_send(unsigned char opType, int dataLen, unsigned char *data)
{
    if (!write_queue_handle.queue_initialized)
    {
        return I2C_WRITE_FAILED;
    }

    if (dataLen > MAX_IPC_DATA_SIZE)
    {
        return I2C_WRITE_FAILED;
    }

    IPCMessage msg;
    msg.messageType = QUEUE_IPC_MSG_TYPE;
    msg.data[0] = opType;
    msg.data[1] = dataLen;
    memcpy(&msg.data[2], data, dataLen);

    if (msgsnd(write_queue_handle.queue_id, &msg, sizeof(msg), QUEUE_IPC_FLAGS) != 0)
    {
        return I2C_WRITE_FAILED;
    }

    return I2C_SUCCESS;
}

static inline int read_queue_receive(unsigned char *opType, int *dataLen, unsigned char *data)
{
    if (!read_queue_handle.queue_initialized)
    {
        return I2C_READ_FAILED;
    }

    IPCMessage msg;

    if (msgrcv(read_queue_handle.queue_id, &msg, sizeof(msg), QUEUE_IPC_MSG_TYPE, IPC_RECEIVE_FLAGS) < 0)
    {
        return I2C_READ_FAILED;
    }

    if (msg.data[1] > MAX_IPC_DATA_SIZE)
    {
        return I2C_READ_FAILED;
    }

    *opType=msg.data[0];
    *dataLen=msg.data[1];
    memcpy(data, &data[2], *dataLen);

    return I2C_SUCCESS;
}
