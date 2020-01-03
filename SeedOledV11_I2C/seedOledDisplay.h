#pragma once
/* HEADER for basic Library to drive the Seed oled display https://www.seeedstudio.com/Grove-OLED-Display-1-12.html */
/* based on Seed Sphere Grove Library, adapted to used onboard I2C bus */

#include "applibs_versions.h"
#include <applibs/gpio.h>


int TestCompil(void);


#define SH1107G  1
#define SSD1327  2

void SeeedOledDisplay_Init(int i2cFd, uint8_t IC);

void setNormalDisplay(void);
void setInverseDisplay(void);

void setGrayLevel(unsigned char grayLevel);

void setVerticalMode(void);
void setHorizontalMode(void);

void setTextXY(unsigned char Row, unsigned char Column);
void clearDisplay(void);
void setContrastLevel(unsigned char ContrastLevel);
void putChar(unsigned char c);
void putString(const char* String);
unsigned char putNumber(long n);

void drawBitmap(const unsigned char* bitmaparray, int bytes);

void setHorizontalScrollProperties(bool direction, unsigned char startRow, unsigned char endRow, unsigned char startColumn, unsigned char endColumn, unsigned char scrollSpeed);
void activateScroll(void);
void deactivateScroll(void);