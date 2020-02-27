#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include <applibs/log.h>
#include <applibs/gpio.h>

#include "ledstripews2813.h"

#define SEEED_PIXELSTRIPE_LENGTH  (10)

#define GPIO_ID_WS2813  (60)

#define GPIO_ID_LED1  (10)
#define GPIO_ID_LED2 (23)

int main(void)
{
    // TODO : Add signal catch for SIGTERM
    // TODO : Add signal catch for SIGKILL

    Log_Debug(
        "\nSample to drive adressable led stripe based on WS2813\n");

    int fd_led1 = GPIO_OpenAsOutput(GPIO_ID_LED1, GPIO_OutputMode_PushPull, GPIO_Value_High);
    if (fd_led1 < 0) {
        Log_Debug(
            "ERROR: Error opening GPIO: %s (%d). Check that app_manifest.json includes the GPIO used.\n",
            strerror(errno), errno);
        return -1;
    }

    int fd_led2 = GPIO_OpenAsOutput(GPIO_ID_LED2, GPIO_OutputMode_PushPull, GPIO_Value_High);
    if (fd_led2 < 0) {
        Log_Debug(
            "ERROR: Error opening GPIO: %s (%d). Check that app_manifest.json includes the GPIO used.\n",
            strerror(errno), errno);
        return -1;
    }

    int fd_ledstripe = -1;
    //fd_ledstripe = GPIO_OpenAsOutput(GPIO_ID_WS2813, GPIO_OutputMode_PushPull, GPIO_Value_High);
    //if (fd_ledstripe < 0) {
    //    Log_Debug(
    //        "Error opening GPIO: %s (%d). Check that app_manifest.json includes the GPIO used.\n",
    //        strerror(errno), errno);
    //    return -1;
    //}

    if (ledstripe_init(SEEED_PIXELSTRIPE_LENGTH, fd_ledstripe) < 0)
    {
        Log_Debug("ERROR: Error while intializing LedStripe Library. See Error Logs output\n");
    }


    const struct timespec sleepTime = {1, 0};
    while (true) {
        GPIO_SetValue(fd_led1, GPIO_Value_Low);
        GPIO_SetValue(fd_led2, GPIO_Value_High);
        ledstripe_setpixel(0, 0xFF, 0, 0);
        ledstripe_setpixel(9, 0x00, 0, 0);
        nanosleep(&sleepTime, NULL);

        GPIO_SetValue(fd_led1, GPIO_Value_High);
        GPIO_SetValue(fd_led2, GPIO_Value_Low);
        ledstripe_setpixel(0, 0x00, 0, 0);
        ledstripe_setpixel(9, 0xFF, 0, 0);
        nanosleep(&sleepTime, NULL);
    }
}
