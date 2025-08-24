#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include "ICM_42670_defs.h"
#include "i2c_emulated.h"

#define ACCEL_ERROR_NONE 0

#define ACCEL_ERROR_BASE -1

#define ACCEL_ERROR_I2C_FAIL (ACCEL_ERROR_BASE-0)
#define ACCEL_ERROR_NOT_CONFIGURED (ACCEL_ERROR_BASE-1)
#define ACCEL_ERROR_INVALID_CFG_VALUE (ACCEL_ERROR_BASE-2)
#define ACCEL_ERROR_OTHER (ACCEL_ERROR_BASE-3)
#define ACCEL_NOT_IMPLEMENTED_YET (ACCEL_ERROR_BASE-4)

typedef struct
{
    /* Per axis values */
    float x;
    float y;
    float z;
} AxesData;

typedef struct
{
    AxesData gyro; /* Gyroscope data, in dps units */
    AxesData accel; /* Accelerometer data, in g units */
} AccelData;

typedef struct
{
    ICM_42670_GyroRange range; /* Measurements range */
    ICM_42670_GyroFreq freq;
} GyroConfigData;

typedef struct
{
    ICM_42670_AccelRange range;
    ICM_42670_AccelFreq freq;
} AccelConfigData;

int accel_start_acquisition(void);
int accel_stop_acquisition(void);
int acce_accel_set_config(AccelConfigData *config);
int accel_gyro_set_config(GyroConfigData *config);
int accel_get_data(AccelData *data);

#endif // ACCELEROMETER_H
