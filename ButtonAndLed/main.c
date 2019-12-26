#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include <applibs/log.h>
#include <applibs/gpio.h>

#include "MT3620rbd.h"

typedef struct  {
    int led1;
    int led3;
    int buttonA;
    int buttonB;
} MyGpio;

int InitGpios( MyGpio *gpios)
{
    gpios->led1 = GPIO_OpenAsOutput(LED1_RED, GPIO_OutputMode_PushPull, GPIO_Value_High);
    if (gpios->led1 < 0) {
        Log_Debug(
            "Error opening GPIO: %s (%d). Check that app_manifest.json includes the GPIO used.\n",
            strerror(errno), errno);
        return -1;
    }

    gpios->led3 = GPIO_OpenAsOutput(LED3_BLUE, GPIO_OutputMode_PushPull, GPIO_Value_High);
    if (gpios->led3 < 0) {
        Log_Debug(
            "Error opening GPIO: %s (%d). Check that app_manifest.json includes the GPIO used.\n",
            strerror(errno), errno);
        return -1;
    }


    gpios->buttonA = GPIO_OpenAsInput(BUTTON_A);
    if (gpios->buttonA< 0) {
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

void SetLed(int fdLed,bool light)
{
    GPIO_SetValue(fdLed, light?GPIO_Value_Low:GPIO_Value_High);
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

    SetLed(gpios.led1, false);
    SetLed(gpios.led3, false);

    const struct timespec sleepTime = {0, 100000000 }; // sleeptime = 100 ms
    while (true) {
        if (IsButtonPressed(gpios.buttonA))
            SetLed(gpios.led1, true);
        else SetLed(gpios.led1, false);

        if (IsButtonPressed(gpios.buttonB))
            SetLed(gpios.led3, true);
        else SetLed(gpios.led3, false);

        nanosleep(&sleepTime, NULL);
    }
}
