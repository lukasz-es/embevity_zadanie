#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include "ICM_42670_defs.h"
#include "i2c_emulated.h"

#define ACCEL_SUCCESS 0

#define ACCEL_ERROR_BASE -1

#define ACCEL_ERROR_I2C_FAIL (ACCEL_ERROR_BASE-0)
#define ACCEL_ERROR_NOT_CONFIGURED (ACCEL_ERROR_BASE-1)
#define ACCEL_ERROR_INVALID_CFG_VALUE (ACCEL_ERROR_BASE-2)
#define ACCEL_ERROR_OTHER (ACCEL_ERROR_BASE-3)
#define ACCEL_NOT_IMPLEMENTED_YET (ACCEL_ERROR_BASE-4)

#define HALF_RANGE_VALUE 32767
#define SPLIT_POINT 256

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

typedef struct
{
	ICM_42670_PwrMgmt_GyroMode gyro;
	ICM_42670_PwrMgmt_AccelMode accel;
} PowerMgmtData;

int accel_start_acquisition(void);
int accel_stop_acquisition(void);
int accel_accel_set_config(AccelConfigData *config);
int accel_gyro_set_config(GyroConfigData *config);
int accel_pwr_mgmt_set_config(PowerMgmtData *config);
int accel_get_data(AccelData *data);
float floatFromAccelValues(const unsigned char *array,const float range);

#endif // ACCELEROMETER_H
