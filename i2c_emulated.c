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
    if ( (queue_init(&write_queue_handle, QUEUE_PROJ_ID_WRITE)) || (queue_init(&read_queue_handle, QUEUE_PROJ_ID_READ)) )
    {
        return I2C_CREATE_FAILED;
    }
    return I2C_SUCCESS;
}

static int queue_init(IPCMessageQueueHandles *handles, int queue_proj_id)
{
    /* TODO: error checks */
    handles->queue_key = ftok(QUEUE_ID_FILE, queue_proj_id);
    handles->queue_id = msgget(handles->queue_key, (QUEUE_ACCESS_MOD | IPC_CREAT));
    handles->queue_initialized=1;
    return I2C_SUCCESS;
}

static int write_queue_send(unsigned char opType, unsigned char *data)
{
    return I2C_NOT_IMPLEMENTED_YET;
}

static int read_queue_init(void)
{
    return I2C_NOT_IMPLEMENTED_YET;
}

static int read_queue_receive(unsigned char opType, unsigned char *data)
{
    return I2C_NOT_IMPLEMENTED_YET;
}
