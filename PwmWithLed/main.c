#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include <applibs/log.h>
#include <applibs/gpio.h>
#include <applibs/pwm.h>

#include "MT3620rdb.h"


typedef struct {
    int pwm0;
    int buttonA;
    int buttonB;
} MyGpio;


int InitGpios(MyGpio* gpios)
{
    gpios->buttonA = GPIO_OpenAsInput(BUTTON_A);
    if (gpios->buttonA < 0) {
        Log_Debug(
            "Error opening GPIO: %s (%d). Check that app_manifest.json includes the GPIO used.\n",
            strerror(errno), errno);
        return -1;
    }

    gpios->buttonB = GPIO_OpenAsInput(BUTTON_B);
    if (gpios->buttonB < 0) {
        Log_Debug(
            "Error opening GPIO: %s (%d). Check that app_manifest.json includes the GPIO used.\n",
            strerror(errno), errno);
        return -1;
    }

    return 0;
}


int IsButtonPressed(int fdButton)
{
    GPIO_Value_Type res;
    if (GPIO_GetValue(fdButton, &res) == -1)
        return 0;
    return (res == GPIO_Value_Low);
}


int main(void)
{
    MyGpio gpios;
    InitGpios(&gpios);

    const struct timespec sleepTime = { 0, 100000000 }; // sleeptime = 100 ms
    while (true) {
        //if (IsButtonPressed(gpios.buttonA))
  
        //if (IsButtonPressed(gpios.buttonB))

        nanosleep(&sleepTime, NULL);
    }
}
