#include <iostream>
#include "imui2c.h"

int main()
{
    IMUI2C i2c;
	i2c.i2CLoop();
    std::cout << "Emulator finished" << std::endl;
    return 0;
}
