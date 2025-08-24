#ifndef I2C_EMULATED_H
#define I2C_EMULATED_H

/* C++ header guard */
#ifdef __cplusplus
extern "C" {
#endif

#include <sys/ipc.h>
#include <sys/msg.h>

/* Error definitions */
#define I2C_ERROR_BASE -10
#define I2C_SUCCESS 0
#define I2C_WRITE_FAILED (I2C_ERROR_BASE-0)
#define I2C_READ_FAILED (I2C_ERROR_BASE-1)
#define I2C_CREATE_FAILED (I2C_ERROR_BASE-2)
#define I2C_NOT_IMPLEMENTED_YET (I2C_ERROR_BASE-3)

int i2c_initialize(void);
int i2c_registers_read(unsigned char registerAddr, unsigned char registersNo, unsigned char *data);
int i2c_registers_write(unsigned char registerAddr, unsigned char registersNo, unsigned char *data);

/* IPC queues definitions */
#define IPC_MSG_TYPE_READ 1
#define IPC_MSG_TYPE_WRITE 2

#define QUEUE_ID_FILE "queueidfile"
#define QUEUE_ACCESS_MOD 0666
#define QUEUE_PROJ_ID_READ 12
#define QUEUE_PROJ_ID_WRITE 34

#define MAX_IPC_DATA_SIZE 100

typedef struct
{
    int messageLen; /* Data buffer length in bytes */
    int messageType;
    unsigned char data[MAX_IPC_DATA_SIZE];
} IPCMessage;

typedef struct
{
    key_t queue_key;
    int queue_id;
    int queue_initialized;
} IPCMessageQueueHandles;

/* Internal emulator functions definitions */
static int queue_init(IPCMessageQueueHandles *handles, int queue_proj_id);
static int write_queue_send(unsigned char opType, unsigned char *data);
static int read_queue_receive(unsigned char opType, unsigned char *data);

/* End C++ header guard */
#ifdef __cplusplus
}
#endif

#endif // I2C_EMULATED_H
