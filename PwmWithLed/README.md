# PwmWithLed

use a PWM channel to modulate Led colors & brightness. onboard button A & B increase or decrease LED1 brightness.

AZure Sphere MT3620 mcu has 3 PWM controller, each with 4 PWM channel , for a total of 12 PWM channel.

PWM Channel use GPIO port (0 to 11) to drive PWM output. On the reference dev board, PWM Controller 2 is attached to LED1 