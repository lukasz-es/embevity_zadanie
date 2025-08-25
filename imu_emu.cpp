#include <iostream>
#include "imui2c.h"

using namespace std;

int main()
{
    IMUI2C i2c;
	i2c.i2CLoop();
    cout << "Hello World!" << endl;
    return 0;
}
