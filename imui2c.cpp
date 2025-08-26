#include "imui2c.h"
#include <stdio.h>

IMUI2C::IMUI2C()
{
	const std::string testFileName = "2023-01-16-15-33-09-imu.csv";
	
	std::string line;
	
	testVals.open(testFileName);
	
	if (testVals.is_open())
	{
		// skip header
		std::getline(testVals, line);
		haveDataToSend=getNextAccelValues();
	}

	i2c_initialize_slave();
}

IMUI2C::~IMUI2C()
{
    i2c_close();
	
	if (testVals.is_open())
	{
		testVals.close();
	}
}

void IMUI2C::i2CLoop()
{
    unsigned char rxoptype;
    int rxdatalen;
    unsigned char rxdata[MAX_IPC_TOTAL_SIZE];
	
    unsigned char txdata[MAX_IPC_TOTAL_SIZE];
	
	const int registerAddrPos = 0;
	const int registerValPos = 1;

	while(needToWork)
	{
		read_queue_receive(&rxoptype, &rxdatalen, rxdata);
		
		switch (rxoptype)
		{
			case QUEUE_IPC_MSG_TYPE_WRITE:
			switch(rxdata[registerAddrPos])
			{
				case ICM_42670_GYRO_CONFIG0_REG:
				case ICM_42670_ACCEL_CONFIG0_REG:
				case ICM_42670_PWR_MGMT0_REG:
					std::cout << displayConfigParamsString(rxdata[registerAddrPos], rxdata[registerValPos]) << std::endl;;
					break;
			}
				break;
				
			case QUEUE_IPC_MSG_TYPE_READ:

				switch(rxdata[0])
				{
					case ICM_42670_ACCEL_DATA_REGS:
					case ICM_42670_GYRO_DATA_REGS:
					
						std::copy(std::begin(currentAccelValues), std::end(currentAccelValues), std::begin(txdata));

						write_queue_send(QUEUE_IPC_MSG_TYPE_READ, 12, txdata);
						
						haveDataToSend=getNextAccelValues();
						break;
					
					case ICM_42670_INT_STATUS_DRDY_REG:
						if (haveDataToSend)
						{
							txdata[0] = 1;
						}
						else
						{
							txdata[0] = 0;
							needToWork=false;
						}
						write_queue_send(QUEUE_IPC_MSG_TYPE_READ, 1, txdata);
						break;
					
					default:
					break;
				}
				
				break;
				
			default:
				break;
		}
	}
}

void IMUI2C::floatToAccelValues(unsigned char *array, const float val, const float range) const
{
	const unsigned int halfRangeValue = 32767;
	const unsigned int splitPoint = 256;
	unsigned int tmp = (unsigned int)(( (val / range) + 1 ) * halfRangeValue);
	
	// Byte order is: MSB LSB
	array[0] = tmp / splitPoint;
	array[1] = tmp % splitPoint;
}

bool IMUI2C::getNextAccelValues()
{
	std::string line, part;
	std::string delimiter = ",";
	float tmp;
	
	int valueIndex=0;
    const float testAccelRange = 2.0;
	const int valueSize = 2;
	const int valuesInRow = 6;
	// Max index is 1 size less than usual due to splitting mechanism
	const int maxValueIndex = (valuesInRow - 2) * valueSize;
	
	if (!std::getline(testVals, line))
	{
		testVals.close();
		return false;
	}
	
	// split line into parts separated by comma
	auto pos = line.find(delimiter);
	while (pos != std::string::npos)
	{
		part = line.substr(0, pos);
		line.erase(0, pos + delimiter.length());
		pos = line.find(delimiter);
		
		tmp = std::stof(part);
		// TODO: I'm basically ignoring gyroscope vslues; their range is different and it should be addressed
		// TODO: Use real device config received
		
		floatToAccelValues(&currentAccelValues[valueIndex], tmp, testAccelRange);
		if (valueIndex < maxValueIndex)
		{
			valueIndex += valueSize;
		}
	}
	//Last part remains in input string
	part = line;
	floatToAccelValues(&currentAccelValues[valueIndex], tmp, testAccelRange);
	
	return true;
}

std::string IMUI2C::displayConfigParamsString(const unsigned char reg, const unsigned char val) const
{
	std::string retval;
	
	switch (reg)
	{
		case ICM_42670_GYRO_CONFIG0_REG:
		retval.append("Gyro config: ");
		retval.append(displayGyroConfigParamsString(val));
		break;
		
		case ICM_42670_ACCEL_CONFIG0_REG:
		retval.append("Accel config: ");
		retval.append(displayAccelConfigParamsString(val));
		break;
		
		case ICM_42670_PWR_MGMT0_REG:
		retval.append("Power management config: ");
		retval.append(displayPwrConfigParamsString(val));
		break;
		
		default:
			retval.append("Unknown register configured!");
		break;
	}
	
	return retval;
}

std::string IMUI2C::displayGyroConfigParamsString(const unsigned char val) const
{
	std::string retval;
	
	unsigned char tmp;
	
	//Range
	tmp = val & ICM_42670_GYRO_CFG_RANGE_BITMASK;
	tmp >>= ICM_42670_GYRO_CFG_RANGE_BITSHIFT;
	
	switch (tmp)
	{
		
		case GYRO_RANGE_2000_DPS:
			retval.append("Range: 2000 DPS; ");
			break;
			
		case GYRO_RANGE_1000_DPS:
			retval.append("Range: 1000 DPS; ");
			break;
		
		case GYRO_RANGE_500_DPS:
			retval.append("Range: 500 DPS; ");
			break;
		
		case GYRO_RANGE_250_DPS:
			retval.append("Range: 250 DPS; ");
			break;
		
		default:
		break;
	}
	
	//Frequency
	tmp = val & ICM_42670_GYRO_CFG_FREQ_BITMASK;
	tmp >>= ICM_42670_GYRO_CFG_FREQ_BITSHIFT;
	
	switch (tmp)
	{
		
		case GYRO_FREQ_1600_HZ:
			retval.append("Freq: 1600 Hz");
			break;
			
		case GYRO_FREQ_800_HZ:
			retval.append("Freq: 800 Hz");
			break;
		
		case GYRO_FREQ_400_HZ:
			retval.append("Freq: 400 Hz");
			break;
		
		case GYRO_FREQ_200_HZ:
			retval.append("Freq: 200 Hz");
			break;
		
		case GYRO_FREQ_100_HZ:
			retval.append("Freq: 100 Hz");
			break;
		
		case GYRO_FREQ_50_HZ:
			retval.append("Freq: 50 Hz");
			break;
			
		case GYRO_FREQ_25_HZ:
			retval.append("Freq: 25 Hz");
			break;
			
		case GYRO_FREQ_12_5_HZ:
			retval.append("Freq: 12.5 Hz");
			break;
		
		default:
		break;
	}
	
	return retval;
}

std::string IMUI2C::displayAccelConfigParamsString(const unsigned char val) const
{
	std::string retval;
	
	unsigned char tmp;
	
	//Range
	tmp = val & ICM_42670_ACCEL_CFG_RANGE_BITMASK;
	tmp >>= ICM_42670_ACCEL_CFG_RANGE_BITSHIFT;
	
	switch (tmp)
	{
		
		case ACCEL_RANGE_16_G:
			retval.append("Range: 16G; ");
			break;
			
		case ACCEL_RANGE_8_G:
			retval.append("Range: 8G; ");
			break;
		
		case ACCEL_RANGE_4_G:
			retval.append("Range: 4G; ");
			break;
		
		case ACCEL_RANGE_2_G:
			retval.append("Range: 2G; ");
			break;
		
		default:
		break;
	}
	
	//Frequency
	tmp = val & ICM_42670_ACCEL_CFG_FREQ_BITMASK;
	tmp >>= ICM_42670_ACCEL_CFG_FREQ_BITSHIFT;
	
	switch (tmp)
	{
		
		case ACCEL_FREQ_1600_HZ:
			retval.append("Freq: 1600 Hz");
			break;
			
		case ACCEL_FREQ_800_HZ:
			retval.append("Freq: 800 Hz");
			break;
		
		case ACCEL_FREQ_400_HZ:
			retval.append("Freq: 400 Hz");
			break;
		
		case ACCEL_FREQ_200_HZ:
			retval.append("Freq: 200 Hz");
			break;
		
		case ACCEL_FREQ_100_HZ:
			retval.append("Freq: 100 Hz");
			break;
		
		case ACCEL_FREQ_50_HZ:
			retval.append("Freq: 50 Hz");
			break;
			
		case ACCEL_FREQ_25_HZ:
			retval.append("Freq: 25 Hz");
			break;
			
		case ACCEL_FREQ_12_5_HZ:
			retval.append("Freq: 12.5 Hz");
			break;
			
		case ACCEL_FREQ_6_25_HZ:
			retval.append("Freq: 6.25 Hz");
			break;
			
		case ACCEL_FREQ_3_125_HZ:
			retval.append("Freq: 3.125 Hz");
			break;
			
		case ACCEL_FREQ_1_5625_HZ:
			retval.append("Freq: 1.5625 Hz");
			break;
		
		default:
		break;
	}
	
	return retval;
}

std::string IMUI2C::displayPwrConfigParamsString(const unsigned char val) const
{
	std::string retval;
	
	unsigned char tmp;
	
	//Gyro mode
	tmp = val & ICM_42670_PWR_MGMT_GYRO_MODE_BITMASK;
	tmp >>= ICM_42670_PWR_MGMT_GYRO_MODE_BITSHIFT;
	
	switch (tmp)
	{
		
		case GYRO_PWR_MODE_OFF:
			retval.append("Gyro: OFF; ");
			break;
			
		case GYRO_PWR_MODE_STDBY:
			retval.append("Gyro: Standby; ");
			break;
		
		case GYRO_PWR_MODE_LN:
			retval.append("Gyro: LN; ");
			break;
		
		default:
		break;
	}
	
	//Accel mode
	tmp = val & ICM_42670_PWR_MGMT_ACCEL_MODE_BITMASK;
	tmp >>= ICM_42670_PWR_MGMT_ACCEL_MODE_BITSHIFT;
	
	switch (tmp)
	{
		
		case ACCEL_PWR_MODE_OFF:
		case ACCEL_PWR_MODE_OFF1:
			retval.append("Accel: OFF; ");
			break;
			
		case ACCEL_PWR_MODE_LP:
			retval.append("Accel: LP; ");
			break;
		
		case ACCEL_PWR_MODE_LN:
			retval.append("Accel: LN; ");
			break;
		
		default:
		break;
	}
	
	return retval;
}
