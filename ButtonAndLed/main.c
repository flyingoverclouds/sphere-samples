#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include <applibs/log.h>
#include <applibs/gpio.h>

#include "MT3620rbd.h"

struct MyGpio {
    int led1;
    int buttonA;
    int buttonB;
};

int InitGpios(struct MyGpio *gpios)
{
    gpios->led1 = GPIO_OpenAsOutput(LED1_GREEN, GPIO_OutputMode_PushPull, GPIO_Value_High);
    if (gpios->led1 < 0) {
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

    gpios->buttonA = GPIO_OpenAsInput(BUTTON_B);
    if (gpios->buttonA < 0) {
        Log_Debug(
            "Error opening GPIO: %s (%d). Check that app_manifest.json includes the GPIO used.\n",
            strerror(errno), errno);
        return -1;
    }

    return 0;
}

int main(void)
{
    struct MyGpio gpios;
    InitGpios(&gpios);

    const struct timespec sleepTime = {0, 100000};
    while (true) {
        nanosleep(&sleepTime, NULL);
    }
}
