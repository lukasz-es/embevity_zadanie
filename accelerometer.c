#include "accelerometer.h"

int accel_start_acquisition(void)
{
    AccelConfigData accel_config;
	accel_config.range = ACCEL_RANGE_2_G;
	accel_config.freq = ACCEL_FREQ_50_HZ;
	
	GyroConfigData gyro_config;
	gyro_config.range = GYRO_RANGE_250_DPS;
	gyro_config.freq = GYRO_FREQ_50_HZ;
	
	PowerMgmtData pwr_config;
	pwr_config.gyro = GYRO_PWR_MODE_LN;
	pwr_config.accel = ACCEL_PWR_MODE_LN;
	
	i2c_initialize_master();
	
	if (accel_accel_set_config(&accel_config) != I2C_SUCCESS)
	{
		return ACCEL_ERROR_I2C_FAIL;
	}
	
	if (accel_gyro_set_config(&gyro_config) != I2C_SUCCESS)
	{
		return ACCEL_ERROR_I2C_FAIL;
	}	
	
	if (accel_pwr_mgmt_set_config(&pwr_config) != I2C_SUCCESS)
	{
		return ACCEL_ERROR_I2C_FAIL;
	}
	
	return ACCEL_SUCCESS;
}

int accel_stop_acquisition(void)
{
    i2c_close();
	return ACCEL_SUCCESS;
}

int accel_accel_set_config(AccelConfigData *config)
{
    /* TODO: Config error check */
	unsigned char configByte = (config->range << ICM_42670_ACCEL_CFG_RANGE_BITSHIFT);
	configByte |= (config->freq << ICM_42670_ACCEL_CFG_FREQ_BITSHIFT);
	
	if (i2c_registers_write(ICM_42670_ACCEL_CONFIG0_REG, 1, &configByte) != I2C_SUCCESS)
	{
		return ACCEL_ERROR_I2C_FAIL;
	}
	
	return ACCEL_SUCCESS;
}

int accel_gyro_set_config(GyroConfigData *config)
{
    /* TODO: Config error check */
	unsigned char configByte = (config->range << ICM_42670_GYRO_CFG_RANGE_BITSHIFT);
	configByte |= (config->freq << ICM_42670_GYRO_CFG_FREQ_BITSHIFT);
	
	if (i2c_registers_write(ICM_42670_GYRO_CONFIG0_REG, 1, &configByte) != I2C_SUCCESS)
	{
		return ACCEL_ERROR_I2C_FAIL;
	}
	
	return ACCEL_SUCCESS;
}

int accel_pwr_mgmt_set_config(PowerMgmtData *config)
{
    /* TODO: LP_CLK_SEL and IDLE bits are not supported yet, add support */
	/* TODO: Config error check */
	unsigned char configByte = (config->gyro << ICM_42670_PWR_MGMT_GYRO_MODE_BITSHIFT);
	configByte |= (config->accel << ICM_42670_PWR_MGMT_ACCEL_MODE_BITSHIFT);
	
	if (i2c_registers_write(ICM_42670_PWR_MGMT0_REG, 1, &configByte) != I2C_SUCCESS)
	{
		return ACCEL_ERROR_I2C_FAIL;
	}
	
	return ACCEL_SUCCESS;
}

int accel_get_data(AccelData *data)
{
    return ACCEL_NOT_IMPLEMENTED_YET;
}
