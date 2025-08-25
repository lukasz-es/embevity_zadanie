#ifndef IMUI2C_H
#define IMUI2C_H

#include "i2c_emulated.h"
#include "ICM_42670_defs.h"
#include <vector>
#include <iostream>

class IMUI2C
{
public:
    IMUI2C();
    ~IMUI2C();
    // TODO: Add copy, move constructors, copy and move operators
    
    void i2CLoop();

private:
	void floatToAccelValues(unsigned char *array, const float val, const float range) const;
	
	std::vector<unsigned char> pollData;

	int opcnt;
	bool haveDataToSend;
	bool needToWork = true;

};

#endif // IMUI2C_H
