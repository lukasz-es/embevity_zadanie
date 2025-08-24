#ifndef I2C_EMULATED_H
#define I2C_EMULATED_H

/* C++ header guard */
#ifdef __cplusplus
extern "C" {
#endif

#include <sys/ipc.h> /* For RPC message queue */
#include <sys/msg.h> /* For RPC message queue */
#include <string.h> /* For memcpy */

/* Error definitions */
#define I2C_ERROR_BASE -10
#define I2C_SUCCESS 0
#define I2C_WRITE_FAILED (I2C_ERROR_BASE-0)
#define I2C_READ_FAILED (I2C_ERROR_BASE-1)
#define I2C_CREATE_FAILED (I2C_ERROR_BASE-2)
#define I2C_DESTROY_FAILED (I2C_ERROR_BASE-3)
#define I2C_NOT_IMPLEMENTED_YET (I2C_ERROR_BASE-4)

int i2c_initialize_master(void);
int i2c_initialize_slave(void);
int i2c_close(void);
int i2c_registers_read(unsigned char registerAddr, unsigned char registersNo, unsigned char *data);
int i2c_registers_write(unsigned char registerAddr, unsigned char registersNo, unsigned char *data);

/* IPC queues definitions */
#define QUEUE_IPC_MSG_TYPE_READ 1
#define QUEUE_IPC_MSG_TYPE_WRITE 2
#define QUEUE_IPC_MSG_TYPE 1

#define QUEUE_ID_FILE "queueidfile"
#define QUEUE_ACCESS_MOD 0666
#define QUEUE_IPC_FLAGS 0
#define QUEUE_IPC_RECEIVE_FLAGS 0
#define QUEUE_PROJ_ID_READ 12
#define QUEUE_PROJ_ID_WRITE 34

#define QUEUE_MAX_IPC_DATA_SIZE 100
#define QUEUE_IPC_HEADER_SIZE 2

#define QUEUE_IPC_OP_TYPE_POS 0
#define QUEUE_IPC_OP_RES_POS 0
#define QUEUE_IPC_DATA_LEN_POS 1
#define QUEUE_IPC_PAYLOAD_START_POS 2

#define MAX_IPC_TOTAL_SIZE (QUEUE_MAX_IPC_DATA_SIZE + QUEUE_IPC_HEADER_SIZE)

/* Internal data types */

typedef struct
{
    long messageType;
    unsigned char data[MAX_IPC_TOTAL_SIZE];
} IPCMessage;

typedef struct
{
    key_t queue_key;
    int queue_id;
    int queue_initialized;
} IPCMessageQueueHandles;

/* Internal emulator functions definitions */

int queue_init(IPCMessageQueueHandles *handles, int queue_proj_id);
int queue_destroy(IPCMessageQueueHandles *handles);
int write_queue_send(unsigned char opType, int dataLen, unsigned char *data);
int read_queue_receive(unsigned char *opType, int *dataLen, unsigned char *data);

/* End C++ header guard */
#ifdef __cplusplus
}
#endif

#endif // I2C_EMULATED_H
