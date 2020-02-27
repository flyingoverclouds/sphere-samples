/* ledstripews2813.h : header file for WS2813 addressable ledstripe library
 *
 * STATUS : DRAFT
 * VERSION : 0.1
 * AUTHOR : N. CLERC
 *
 */
#pragma once

int ledstripe_init(int totalpixelCount, int fd_gpio);

void ledstripe_setpixel(int pixelId, unsigned char red, unsigned char green, unsigned char blue);



