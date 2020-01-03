#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include <applibs/log.h>
#include <applibs/gpio.h>
#include <applibs/i2c.h>

#include "seedOledDisplay.h"

#define MT3620_ISU3_I2C (3)

int openI2Cport(int i2cID)
{
    return -1; // to test
}

int main(void)
{
   
    Log_Debug("\nSeedOledV11_I2C : drive oled i2c display with native i2c port 3\n");

    int i2cFD = openI2Cport(MT3620_ISU3_I2C);

    if (i2cFD < 0)
    {
        Log_Debug("ERROR : unable to open I2C port 3.");
        return (-1);
    }

    unsigned int counter = 0;
   
    const struct timespec sleepTime = {1, 0};
    while (true) {
        Log_Debug("Counter = %d", counter);
        nanosleep(&sleepTime, NULL);
        counter++;
    }
}
