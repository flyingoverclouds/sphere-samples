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

    int i2cFd = I2CMaster_Open(MT3620_ISU3_I2C);
    if (i2cFd < 0)
    {
        Log_Debug("ERROR: I2CMaster_Open: errno=%d (%s)\n", errno, strerror(errno));
        return -1;
    }


    int result = I2CMaster_SetBusSpeed(i2cFd, I2C_BUS_SPEED_STANDARD);
    if (result != 0) {
        Log_Debug("ERROR: I2CMaster_SetBusSpeed: errno=%d (%s)\n", errno, strerror(errno));
        return -1;
    }

    result = I2CMaster_SetTimeout(i2cFd, 100);
    if (result != 0) {
        Log_Debug("ERROR: I2CMaster_SetTimeout: errno=%d (%s)\n", errno, strerror(errno));
        return -1;
    }

   /* result = I2CMaster_SetDefaultTargetAddress(i2cFd, lsm6ds3Address);
    if (result != 0) {
        Log_Debug("ERROR: I2CMaster_SetDefaultTargetAddress: errno=%d (%s)\n", errno,
            strerror(errno));
        return -1;
    }*/

    SeeedOledDisplay_Init(i2cFd, SSD1327); // initialize the oled display as a SSD1327 chip driven by

   // Word display
    clearDisplay();
    /*setNormalDisplay();
    setVerticalMode();*/


    for (uint8_t i = 0; i < 16; i++)
    {
        setTextXY(i, i);  //set Cursor to ith line, 0th column
        //setGrayLevel(i); //Set Grayscale level. Any number between 0 - 15.
        putString("Hello World"); //Print Hello World
    }


    unsigned int counter = 0;
    const struct timespec sleepTime = {1, 0};
    while (1) {
        Log_Debug("Counter = %d\n", counter);
        setTextXY(1, 1);
        if (counter % 2 == 0)
            putString("AAAA       ");
        else
            putString("OOOO0000000");
        putNumber(counter);
        nanosleep(&sleepTime, NULL);
        counter++;
    }
}
