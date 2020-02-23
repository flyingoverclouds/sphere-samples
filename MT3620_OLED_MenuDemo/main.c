#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include <applibs/log.h>
#include <applibs/gpio.h>

#include "MT3620rdb.h"


typedef struct {
    int buttonA;
    int buttonB;
} MyIOs;


int InitGpios(MyIOs* ios)
{
    // open Button A as input
    ios->buttonA = GPIO_OpenAsInput(BUTTON_A);
    if (ios->buttonA < 0) {
        Log_Debug("Error opening GPIO: %s (%d). Check that app_manifest.json includes the GPIO used.\n",
            strerror(errno), errno);
        return -1;
    }

    // open Button B as input
    ios->buttonB = GPIO_OpenAsInput(BUTTON_B);
    if (ios->buttonB < 0) {
        Log_Debug("Error opening GPIO: %s (%d). Check that app_manifest.json includes the GPIO used.\n",
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

}
