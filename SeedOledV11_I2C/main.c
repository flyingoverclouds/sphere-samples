/***************************************************************************
 * This program is a counter that display counter value on OLED Screen     *
 * Oled screen is direct connected to I2C port on the RefDev board         *
 *                                                                         *
 *  /!\ : some SEED oled scren are badly identified as v1 (SSD1327) but    *
 *      in reality are v2 (SSH1107G) like mine !                           *
 ***************************************************************************/


#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include <applibs/log.h>
#include <applibs/gpio.h>
#include <applibs/i2c.h>

#include "seedOledDisplay.h"

// We use ISU 3 as I2C bus RTS3, RXD3, 3.3V on H3.3 GND on H3.2
#define MT3620_ISU3_I2C (3)


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

    // !\ : some SEED oled scren are badly identified as v1(SSD1327) but in reality are pre v2(SSH1107G) like mine with v1.1 sticker !

    SeeedOledDisplay_Init(i2cFd, SH1107G); // initialize the oled display as driven by a SH1107G 

   // Word display
    clearDisplay();
    setNormalDisplay();
    setVerticalMode();

    unsigned int counter = 0;
    const struct timespec sleepTime = {1, 0};
    while (1) {
        Log_Debug("Counter = %d\n", counter);
        setTextXY(2,2);
        putString("Counter= ");
        putNumber(counter);
        putString("    ");
        nanosleep(&sleepTime, NULL);
        counter++;
    }
}
