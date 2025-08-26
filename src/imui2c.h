#ifndef IMUI2C_H
#define IMUI2C_H

#include "i2c_emulated.h"
#include "ICM_42670_defs.h"
#include <fstream>
#include <iostream>
#include <string>
#include <bits/stdc++.h>
#include <algorithm>
#include <iterator>

class IMUI2C
{
public:
    IMUI2C();
    ~IMUI2C();
    IMUI2C(IMUI2C &other);
	IMUI2C operator=(IMUI2C other);
    
    void i2CLoop();

private:

	const static int valueSize = 2;
	const static int valuesInRow = 6;
	const static int maxValuesRowSize = valuesInRow * valueSize;

	void floatToAccelValues(unsigned char *array, const float val, const float range) const;
	bool getNextAccelValues();
	std::string displayConfigParamsString(const unsigned char reg, const unsigned char val) const;
	std::string displayGyroConfigParamsString(const unsigned char val) const;
	std::string displayAccelConfigParamsString(const unsigned char val) const;
	std::string displayPwrConfigParamsString(const unsigned char val) const;
	
	unsigned char currentAccelValues[maxValuesRowSize];
	bool haveDataToSend=false;
	bool needToWork = true;
	std::ifstream testVals;

};

#endif // IMUI2C_H
