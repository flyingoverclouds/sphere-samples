#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include <applibs/log.h>
#include <applibs/gpio.h>

int main(void)
{
   
    Log_Debug(
        "\nSeedOledV11_I2C : drive oled i2c display with native i2c\n");

    unsigned int counter = 0;
   
    const struct timespec sleepTime = {1, 0};
    while (true) {
        Log_Debug("Counter = %d", counter);
        nanosleep(&sleepTime, NULL);
        counter++;
    }
}
