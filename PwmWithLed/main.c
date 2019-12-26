#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include <applibs/log.h>
#include <applibs/gpio.h>

// DONT FORGOT TO SET "AzureSphereTargetApiSet": "3+Beta1909" in CMakeSettings.json to activate compilation with beta SDK/API 
#include <applibs/pwm.h>

#include "MT3620rdb.h"

typedef struct {
    int pwm2;
    int buttonA;
    int buttonB;
} MyIOs;

static const unsigned int fullCycleNs = 20000;
static const unsigned int stepIncrementNs = 1000;
static unsigned int dutyCycleNs = 10000;



static PwmState ledPwmState = { .period_nsec = fullCycleNs,
                               .polarity = PWM_Polarity_Inversed,
                               .dutyCycle_nsec = 0,
                               .enabled = true };


void SetPwmChannelState(int pwmCtrlrFD, PWM_ChannelId pwmChannel, PwmState* state)
{
    int res = PWM_Apply(pwmCtrlrFD, pwmChannel, state);
    if (res < 0)
    {
        Log_Debug("Error applying PWM : %s (%d). \n", strerror(errno), errno);
    }
    
}

void SetAllPwmValue(MyIOs *ios, unsigned int dutyCycleNS)
{
    ledPwmState.dutyCycle_nsec = dutyCycleNS;
    SetPwmChannelState(ios->pwm2, PWM_CHANNEL0, &ledPwmState);
    SetPwmChannelState(ios->pwm2, PWM_CHANNEL1, &ledPwmState);
    SetPwmChannelState(ios->pwm2, PWM_CHANNEL2, &ledPwmState);
    //SetPwmChannelState(ios->pwm2, PWM_CHANNEL3, &ledPwmState);
}

int InitGpios(MyIOs* ios)
{
    // open PWM controller 2 (connect to LED1) 
    ios->pwm2 = PWM_Open(PWM_CONTROLLER2);
    if (ios->pwm2 < 0)
    {
        Log_Debug("Error opening PWM Controller 2 : %s (%d). Check that app_manifest.json includes the PWM used.\n",
            strerror(errno), errno);
        return -1;
    }

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
    MyIOs myIOs;
    InitGpios(&myIOs);
    SetAllPwmValue(&myIOs, 0);
    SetPwmChannelState(myIOs.pwm2, PWM_CHANNEL3, &ledPwmState); // PWM2/Channel3 is set to zero (connected to wifi led Blue)

    const struct timespec sleepTime = { 0, 100000000 }; // sleeptime = 100 ms
    //const struct timespec antRebond = { .tv_sec = 0, .tv_nsec = 500000000 }; // 500ms pause when cycle value change
    while (true) {
        if (IsButtonPressed(myIOs.buttonA)) // Decrease
        {
            if (dutyCycleNs >= stepIncrementNs)
                dutyCycleNs -= stepIncrementNs;
            else
                dutyCycleNs = 0;
            Log_Debug("DEC to : %d", dutyCycleNs);
            SetAllPwmValue(&myIOs, dutyCycleNs);
        }

        if (IsButtonPressed(myIOs.buttonB)) // increase
        {
            dutyCycleNs += stepIncrementNs;
            if (dutyCycleNs > fullCycleNs)
                dutyCycleNs = fullCycleNs;
            Log_Debug("INC to : %d", dutyCycleNs);
            SetAllPwmValue(&myIOs, dutyCycleNs);
        }
        nanosleep(&sleepTime, NULL);
    }
}
