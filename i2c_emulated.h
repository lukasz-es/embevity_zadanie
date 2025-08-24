#ifndef I2C_EMULATED_H
#define I2C_EMULATED_H

#define I2C_ERROR_BASE -10
#define I2C_WRITE_FAILED (I2C_ERROR_BASE-0)
#define I2C_READ_FAILED (I2C_ERROR_BASE-1)
#define I2C_NOT_IMPLEMENTED_YET (I2C_ERROR_BASE-2)

int i2c_registers_read(unsigned char registerAddr, unsigned char registersNo, unsigned char *data);
int i2c_registers_write(unsigned char registerAddr, unsigned char registersNo, unsigned char *data);

#endif // I2C_EMULATED_H
