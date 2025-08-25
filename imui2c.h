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
    // TODO: Add copy, move constructors, copy and move operators
    
    void i2CLoop();

private:
	void floatToAccelValues(unsigned char *array, const float val, const float range) const;
	bool getNextAccelValues();
	
	unsigned char currentAccelValues[12];

	bool haveDataToSend=false;
	bool needToWork = true;
	
	std::ifstream testVals;

};

#endif // IMUI2C_H
