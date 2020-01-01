#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include <applibs/log.h>
#include <applibs/gpio.h>

#include <applibs/i2c.h>


// TODO BEFORE COMPILING: 
//   1 - CONNECT THE SEEED GROVE BOARD ON THE CENTERED I2C CONNECTOR (beetween the 1st i2c & Analog socket) on the MT3620 Grove shield
//   2 - Clone or fork/clone the MT3620 grove shield library (https://github.com/Seeed-Studio/MT3620_Grove_Shield)
//   3 - copy the MT3620_Grove_Shield_LIbrary in your root repos folder and compile it to obtain the lib binary
//   4 - In CMkakeLists.txt, add 'add_subdirectory("../MT3620_Grove_Shield_Library" out)'
//   5 - in CMkakeLists.txt , patch 'target_link_libraries' by adding the 'MT3620_Grove_Shield_Library' lib reference 

#include "../MT3620_Grove_Shield_Library/Grove.h"
#include "../MT3620_Grove_Shield_Library/Sensors/GroveOledDisplay96x96.h"

// i2cScanner : list and dump i2c address of devices detected on the grove shield board
void i2cScanner(int i2cFd)
{
    uint8_t i2cState;
    const struct timespec sleepTime = { 0, 10000000 };
    Log_Debug("Scanning I2C bus on Grove Shield ...\n");
    for (uint8_t addr = 1; addr < 127; addr++)
    {
        Log_Debug("I2C[%d] : ",addr);

        GroveI2C_WriteBytes(i2cFd, (uint8_t)(addr << 1), NULL, 0);
        SC18IM700_ReadReg(i2cFd, 0x0A, &i2cState);
        if (i2cState == I2C_OK)
        {
            Log_Debug("I2C_OK, address detect: 0x%02X\r\n", addr);
        }
        else Log_Debug(" no device detected.\n");


        nanosleep(&sleepTime, NULL);
    }
}


int main(void)
{
    Log_Debug("Sample for oled screen 96x96 v1.1\n");

    int i2cFd;
    GroveShield_Initialize(&i2cFd, 230400);
    if (i2cFd<0)
    {
        Log_Debug("ERROR : GroveShield_Initialize() return %d : %s. Please check your app manifest capabilities", errno, strerror(errno));
        exit(-1);
    }

    i2cScanner(i2cFd);
    exit(-1);

    GroveOledDisplay_Init(i2cFd, SSD1327 ); // SSD1327 is for v1.x oled (96x96). If v2 (128x128) -> use SH1107G


    const struct timespec sleepTime = {1, 0};
    int counter = 0;
    while (true) {
        
        clearDisplay(); // clear all pixel on screen
        setNormalDisplay(); // black background, light pixel
        setVerticalMode(); // text orientation vertical

        setGrayLevel(15);
        setTextXY(5, 5);
        putString("Azure Sphere");
        setTextXY(25, 5);
        putString("COUNTER");

        setGrayLevel((unsigned char)(counter % 16));
        setTextXY(55, 30);
        
        putString("00000");

        counter++;
        nanosleep(&sleepTime, NULL);
    }
}
