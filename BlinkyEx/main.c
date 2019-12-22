#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include <applibs/log.h>
#include <applibs/gpio.h>

#include "MT3620rbd.h"

int gpioFD[4]; // gpio fd for the onbard leds (MT3260 dev board)

/* InitGpioLeds(int gpioFD[])
 *     initialize GPIO : LED 1/2/3/4
 * params: 
 *    gpiodFD : array of int , file descriptor for gpio leds
 * return: 
 *   -1 : error in intialization
 *    0 : init succeeded. 
*/
int InitGpioLeds(int gpioFD[])
{
    gpioFD[0] = GPIO_OpenAsOutput(LED1_BLUE, GPIO_OutputMode_PushPull, GPIO_Value_High);
    if (gpioFD[0] < 0) {
        Log_Debug(
            "Error opening GPIO: %s (%d). Check that app_manifest.json includes the GPIO used.\n",
            strerror(errno), errno);
        return -1;
    }
    gpioFD[1] = GPIO_OpenAsOutput(LED2_GREEN, GPIO_OutputMode_PushPull, GPIO_Value_High);
    if (gpioFD[0] < 0) {
        Log_Debug(
            "Error opening GPIO: %s (%d). Check that app_manifest.json includes the GPIO used.\n",
            strerror(errno), errno);
        return -1;
    }
    gpioFD[2] = GPIO_OpenAsOutput(LED3_RED, GPIO_OutputMode_PushPull, GPIO_Value_High);
    if (gpioFD[0] < 0) {
        Log_Debug(
            "Error opening GPIO: %s (%d). Check that app_manifest.json includes the GPIO used.\n",
            strerror(errno), errno);
        return -1;
    }
    gpioFD[3] = GPIO_OpenAsOutput(LED4_GREEN, GPIO_OutputMode_PushPull, GPIO_Value_High);
    if (gpioFD[0] < 0) {
        Log_Debug(
            "Error opening GPIO : %s (%d). Check that app_manifest.json includes the GPIO used.\n",
            strerror(errno), errno);
        return -1;
    }

    return 0;
}

void SetOnboardLedsValue(GPIO_Value led1, GPIO_Value led2, GPIO_Value led3, GPIO_Value led4)
{
    GPIO_SetValue(gpioFD[0], led1);
    GPIO_SetValue(gpioFD[1], led2);
    GPIO_SetValue(gpioFD[2], led3);
    GPIO_SetValue(gpioFD[3], led4);
}


int main(void)
{
    if (InitGpioLeds(gpioFD) < 0)
    {
        Log_Debug("Error initializing GPIO Leds");
        return -1;
    }

    const struct timespec sleepTime = {0, 300000000};
    while (true) {
        SetOnboardLedsValue(GPIO_Value_Low, GPIO_Value_Low, GPIO_Value_Low, GPIO_Value_High);
        nanosleep(&sleepTime, NULL);
        SetOnboardLedsValue(GPIO_Value_High, GPIO_Value_High, GPIO_Value_High, GPIO_Value_High);
        nanosleep(&sleepTime, NULL);
    }
}
