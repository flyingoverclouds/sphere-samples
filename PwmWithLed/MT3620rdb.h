/* Defining alias for GPIO in MT3620 reference dev board (seeed)
 * version 1.1

*/

#pragma once

// TODO : ??replace by using the .h generator :https://docs.microsoft.com/en-us/azure-sphere/hardware/hardware-abstraction

// Defining alias for GPIO in MT3620 reference design board (seeed)



#define LED1_RED		 8
#define LED1_GREEN		 9
#define LED1_BLUE		10

#define LED2_RED		15
#define LED2_GREEN		16
#define LED2_BLUE		17

#define LED3_RED		18
#define LED3_GREEN		19
#define LED3_BLUE		20

#define LED4_RED		21
#define LED4_GREEN		22
#define LED4_BLUE		23

#define APPSTATUS_LED_RED	45
#define APPSTATUS_LED_GREEN 46
#define APPSTATUS_LED_BLUE	47

#define WIFI_LED_RED	48
#define WIFI_LED_GREEN	14
#define WIFI_LED_BLUE	11

#define BUTTON_A		12
#define BUTTON_B		13

#define PWM_CONTROLLER0 0
#define PWM_CONTROLLER1 1
// PWM_CONTROLLER2 is connected to LED1 RGB
#define PWM_CONTROLLER2 2   

// PWM channel 0 of the selected PWM controller
#define PWM_CHANNEL0    0
// PWM channel 1 of the selected PWM controller
#define PWM_CHANNEL1    1
// PWM channel 2 of the selected PWM controller
#define PWM_CHANNEL2    2
// PWM channel 3 of the selected PWM controller
#define PWM_CHANNEL3    3
