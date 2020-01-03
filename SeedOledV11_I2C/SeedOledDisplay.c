/* BASIC Library to drive the Seed oled display https://www.seeedstudio.com/Grove-OLED-Display-1-12.html */
/* based on Seed Sphere Grove Library, adapted to used onboard I2C bus */


#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include <applibs/log.h>

#include<applibs/i2c.h>

#include "seedOledDisplay.h"


int TestCompil()
{
	return 1234;
}




#define SH1107G  1
#define SSD1327  2

#define VERTICAL_MODE                       01
#define HORIZONTAL_MODE                     02

#define SeeedGrayOLED_Address		(0x3C << 1)


/*Command and register */
#define SeeedGrayOLED_Command_Mode          0x80
#define SeeedGrayOLED_Data_Mode				 0x40

#define SeeedGrayOLED_Display_Off_Cmd       0xAE
#define SeeedGrayOLED_Display_On_Cmd        0xAF

#define SeeedGrayOLED_Normal_Display_Cmd    0xA4
#define SeeedGrayOLED_Inverse_Display_Cmd   0xA7
#define SeeedGrayOLED_Activate_Scroll_Cmd   0x2F
#define SeeedGrayOLED_Dectivate_Scroll_Cmd  0x2E
#define SeeedGrayOLED_Set_ContrastLevel_Cmd 0x81

#define Scroll_Left             0x00
#define Scroll_Right            0x01

#define Scroll_2Frames          0x7
#define Scroll_3Frames          0x4
#define Scroll_4Frames          0x5
#define Scroll_5Frames          0x0
#define Scroll_25Frames         0x6
#define Scroll_64Frames         0x1
#define Scroll_128Frames        0x2
#define Scroll_256Frames        0x3


// TODO : replace global static var with local var 

static int _i2cFd = -1; 
static int Drive_IC = SH1107G;
static char addressingMode;
static uint8_t grayH;
static uint8_t grayL;

// This font can be freely used without any restriction(It is placed in public domain)
const unsigned char BasicFont[][8] =
{
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x5F,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x07,0x00,0x07,0x00,0x00,0x00},
  {0x00,0x14,0x7F,0x14,0x7F,0x14,0x00,0x00},
  {0x00,0x24,0x2A,0x7F,0x2A,0x12,0x00,0x00},
  {0x00,0x23,0x13,0x08,0x64,0x62,0x00,0x00},
  {0x00,0x36,0x49,0x55,0x22,0x50,0x00,0x00},
  {0x00,0x00,0x05,0x03,0x00,0x00,0x00,0x00},
  {0x00,0x1C,0x22,0x41,0x00,0x00,0x00,0x00},
  {0x00,0x41,0x22,0x1C,0x00,0x00,0x00,0x00},
  {0x00,0x08,0x2A,0x1C,0x2A,0x08,0x00,0x00},
  {0x00,0x08,0x08,0x3E,0x08,0x08,0x00,0x00},
  {0x00,0xA0,0x60,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x08,0x08,0x08,0x08,0x08,0x00,0x00},
  {0x00,0x60,0x60,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x20,0x10,0x08,0x04,0x02,0x00,0x00},
  {0x00,0x3E,0x51,0x49,0x45,0x3E,0x00,0x00},
  {0x00,0x00,0x42,0x7F,0x40,0x00,0x00,0x00},
  {0x00,0x62,0x51,0x49,0x49,0x46,0x00,0x00},
  {0x00,0x22,0x41,0x49,0x49,0x36,0x00,0x00},
  {0x00,0x18,0x14,0x12,0x7F,0x10,0x00,0x00},
  {0x00,0x27,0x45,0x45,0x45,0x39,0x00,0x00},
  {0x00,0x3C,0x4A,0x49,0x49,0x30,0x00,0x00},
  {0x00,0x01,0x71,0x09,0x05,0x03,0x00,0x00},
  {0x00,0x36,0x49,0x49,0x49,0x36,0x00,0x00},
  {0x00,0x06,0x49,0x49,0x29,0x1E,0x00,0x00},
  {0x00,0x00,0x36,0x36,0x00,0x00,0x00,0x00},
  {0x00,0x00,0xAC,0x6C,0x00,0x00,0x00,0x00},
  {0x00,0x08,0x14,0x22,0x41,0x00,0x00,0x00},
  {0x00,0x14,0x14,0x14,0x14,0x14,0x00,0x00},
  {0x00,0x41,0x22,0x14,0x08,0x00,0x00,0x00},
  {0x00,0x02,0x01,0x51,0x09,0x06,0x00,0x00},
  {0x00,0x32,0x49,0x79,0x41,0x3E,0x00,0x00},
  {0x00,0x7E,0x09,0x09,0x09,0x7E,0x00,0x00},
  {0x00,0x7F,0x49,0x49,0x49,0x36,0x00,0x00},
  {0x00,0x3E,0x41,0x41,0x41,0x22,0x00,0x00},
  {0x00,0x7F,0x41,0x41,0x22,0x1C,0x00,0x00},
  {0x00,0x7F,0x49,0x49,0x49,0x41,0x00,0x00},
  {0x00,0x7F,0x09,0x09,0x09,0x01,0x00,0x00},
  {0x00,0x3E,0x41,0x41,0x51,0x72,0x00,0x00},
  {0x00,0x7F,0x08,0x08,0x08,0x7F,0x00,0x00},
  {0x00,0x41,0x7F,0x41,0x00,0x00,0x00,0x00},
  {0x00,0x20,0x40,0x41,0x3F,0x01,0x00,0x00},
  {0x00,0x7F,0x08,0x14,0x22,0x41,0x00,0x00},
  {0x00,0x7F,0x40,0x40,0x40,0x40,0x00,0x00},
  {0x00,0x7F,0x02,0x0C,0x02,0x7F,0x00,0x00},
  {0x00,0x7F,0x04,0x08,0x10,0x7F,0x00,0x00},
  {0x00,0x3E,0x41,0x41,0x41,0x3E,0x00,0x00},
  {0x00,0x7F,0x09,0x09,0x09,0x06,0x00,0x00},
  {0x00,0x3E,0x41,0x51,0x21,0x5E,0x00,0x00},
  {0x00,0x7F,0x09,0x19,0x29,0x46,0x00,0x00},
  {0x00,0x26,0x49,0x49,0x49,0x32,0x00,0x00},
  {0x00,0x01,0x01,0x7F,0x01,0x01,0x00,0x00},
  {0x00,0x3F,0x40,0x40,0x40,0x3F,0x00,0x00},
  {0x00,0x1F,0x20,0x40,0x20,0x1F,0x00,0x00},
  {0x00,0x3F,0x40,0x38,0x40,0x3F,0x00,0x00},
  {0x00,0x63,0x14,0x08,0x14,0x63,0x00,0x00},
  {0x00,0x03,0x04,0x78,0x04,0x03,0x00,0x00},
  {0x00,0x61,0x51,0x49,0x45,0x43,0x00,0x00},
  {0x00,0x7F,0x41,0x41,0x00,0x00,0x00,0x00},
  {0x00,0x02,0x04,0x08,0x10,0x20,0x00,0x00},
  {0x00,0x41,0x41,0x7F,0x00,0x00,0x00,0x00},
  {0x00,0x04,0x02,0x01,0x02,0x04,0x00,0x00},
  {0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00},
  {0x00,0x01,0x02,0x04,0x00,0x00,0x00,0x00},
  {0x00,0x20,0x54,0x54,0x54,0x78,0x00,0x00},
  {0x00,0x7F,0x48,0x44,0x44,0x38,0x00,0x00},
  {0x00,0x38,0x44,0x44,0x28,0x00,0x00,0x00},
  {0x00,0x38,0x44,0x44,0x48,0x7F,0x00,0x00},
  {0x00,0x38,0x54,0x54,0x54,0x18,0x00,0x00},
  {0x00,0x08,0x7E,0x09,0x02,0x00,0x00,0x00},
  {0x00,0x18,0xA4,0xA4,0xA4,0x7C,0x00,0x00},
  {0x00,0x7F,0x08,0x04,0x04,0x78,0x00,0x00},
  {0x00,0x00,0x7D,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x80,0x84,0x7D,0x00,0x00,0x00,0x00},
  {0x00,0x7F,0x10,0x28,0x44,0x00,0x00,0x00},
  {0x00,0x41,0x7F,0x40,0x00,0x00,0x00,0x00},
  {0x00,0x7C,0x04,0x18,0x04,0x78,0x00,0x00},
  {0x00,0x7C,0x08,0x04,0x7C,0x00,0x00,0x00},
  {0x00,0x38,0x44,0x44,0x38,0x00,0x00,0x00},
  {0x00,0xFC,0x24,0x24,0x18,0x00,0x00,0x00},
  {0x00,0x18,0x24,0x24,0xFC,0x00,0x00,0x00},
  {0x00,0x00,0x7C,0x08,0x04,0x00,0x00,0x00},
  {0x00,0x48,0x54,0x54,0x24,0x00,0x00,0x00},
  {0x00,0x04,0x7F,0x44,0x00,0x00,0x00,0x00},
  {0x00,0x3C,0x40,0x40,0x7C,0x00,0x00,0x00},
  {0x00,0x1C,0x20,0x40,0x20,0x1C,0x00,0x00},
  {0x00,0x3C,0x40,0x30,0x40,0x3C,0x00,0x00},
  {0x00,0x44,0x28,0x10,0x28,0x44,0x00,0x00},
  {0x00,0x1C,0xA0,0xA0,0x7C,0x00,0x00,0x00},
  {0x00,0x44,0x64,0x54,0x4C,0x44,0x00,0x00},
  {0x00,0x08,0x36,0x41,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x7F,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x41,0x36,0x08,0x00,0x00,0x00,0x00},
  {0x00,0x02,0x01,0x01,0x02,0x01,0x00,0x00},
  {0x00,0x02,0x05,0x05,0x02,0x00,0x00,0x00}
};

static void sendCommand(uint8_t cmd)
{
	//Log_Debug("NOT IMPLEMENTED : sendCommand()");
	//GroveI2C_WriteReg8(_i2cFd, SeeedGrayOLED_Address, SeeedGrayOLED_Command_Mode, cmd);

	uint8_t send[2];
	send[0] = SeeedGrayOLED_Command_Mode;
	send[1] = cmd;
	I2CMaster_Write(_i2cFd, SeeedGrayOLED_Address, send, sizeof(send));
}



static void sendData(uint8_t data)
{
	//Log_Debug("NOT IMPLEMENTED : sendData");
	//GroveI2C_WriteReg8(_i2cFd, SeeedGrayOLED_Address, SeeedGrayOLED_Data_Mode, data);
	uint8_t send[2];
	send[0] = SeeedGrayOLED_Data_Mode;
	send[1] = data;
	I2CMaster_Write(_i2cFd, SeeedGrayOLED_Address, send, sizeof(send));
}

void SeeedOledDisplay_Init(int i2cFd, uint8_t IC)
{
	_i2cFd = i2cFd;
	Drive_IC = IC;

	static const struct timespec sleepTime = { 0, 100000000 };

	if (Drive_IC == SSD1327)
	{
		sendCommand(0xFD); // Unlock OLED driver IC MCU interface from entering command. i.e: Accept commands
		sendCommand(0x12);
		sendCommand(0xAE); // Set display off
		sendCommand(0xA8); // set multiplex ratio
		sendCommand(0x5F); // 96
		sendCommand(0xA1); // set display start line
		sendCommand(0x00);
		sendCommand(0xA2); // set display offset
		sendCommand(0x60);
		sendCommand(0xA0); // set remap
		sendCommand(0x46);
		sendCommand(0xAB); // set vdd internal
		sendCommand(0x01); //
		sendCommand(0x81); // set contrasr
		sendCommand(0x53); // 100 nit
		sendCommand(0xB1); // Set Phase Length
		sendCommand(0X51); //
		sendCommand(0xB3); // Set Display Clock Divide Ratio/Oscillator Frequency
		sendCommand(0x01);
		sendCommand(0xB9); //
		sendCommand(0xBC); // set pre_charge voltage/VCOMH
		sendCommand(0x08); // (0x08);
		sendCommand(0xBE); // set VCOMH
		sendCommand(0X07); // (0x07);
		sendCommand(0xB6); // Set second pre-charge period
		sendCommand(0x01); //
		sendCommand(0xD5); // enable second precharge and enternal vsl
		sendCommand(0X62); // (0x62);
		sendCommand(0xA4); // Set Normal Display Mode
		sendCommand(0x2E); // Deactivate Scroll
		sendCommand(0xAF); // Switch on display
		nanosleep(&sleepTime, NULL);

		// Row Address
		sendCommand(0x75);    // Set Row Address 
		sendCommand(0x00);    // Start 0
		sendCommand(0x5f);    // End 95 


		// Column Address
		sendCommand(0x15);    // Set Column Address 
		sendCommand(0x08);    // Start from 8th Column of driver IC. This is 0th Column for OLED 
		sendCommand(0x37);    // End at  (8 + 47)th column. Each Column has 2 pixels(segments)

		// Init gray level for text. Default:Brightest White
		grayH = 0xF0;
		grayL = 0x0F;
	}
	else if (Drive_IC == SH1107G)
	{
		sendCommand(0xae);  //Display OFF 
		sendCommand(0xd5);  // Set Dclk
		sendCommand(0x50);  // 100Hz
		sendCommand(0x20);  // Set row address
		sendCommand(0x81);  // Set contrast control
		sendCommand(0x80);
		sendCommand(0xa0);  // Segment remap
		sendCommand(0xa4);  // Set Entire Display ON 
		sendCommand(0xa6);  // Normal display
		sendCommand(0xad);  // Set external VCC
		sendCommand(0x80);
		sendCommand(0xc0);  // Set Common scan direction
		sendCommand(0xd9);  // Set phase leghth
		sendCommand(0x1f);
		sendCommand(0xdb);  // Set Vcomh voltage
		sendCommand(0x27);
		sendCommand(0xaf);  //Display ON
		sendCommand(0xb0);
		sendCommand(0x00);
		sendCommand(0x11);
	}
}

void setContrastLevel(unsigned char ContrastLevel)
{
	sendCommand(SeeedGrayOLED_Set_ContrastLevel_Cmd);
	sendCommand(ContrastLevel);
}

void setHorizontalMode(void)
{
	if (Drive_IC == SSD1327)
	{
		sendCommand(0xA0); // remap to
		sendCommand(0x42); // horizontal mode

		// Row Address
		sendCommand(0x75);    // Set Row Address 
		sendCommand(0x00);    // Start 0
		sendCommand(0x5f);    // End 95 

		// Column Address
		sendCommand(0x15);    // Set Column Address 
		sendCommand(0x08);    // Start from 8th Column of driver IC. This is 0th Column for OLED 
		sendCommand(0x37);    // End at  (8 + 47)th column. Each Column has 2 pixels(or segments)
	}
	else if (Drive_IC == SH1107G)
	{
		sendCommand(0xA0);
		sendCommand(0xC8);
	}
}

void setVerticalMode(void)
{
	if (Drive_IC == SSD1327)
	{
		sendCommand(0xA0); // remap to
		sendCommand(0x46); // Vertical mode
	}
	else if (Drive_IC == SH1107G)
	{
		sendCommand(0xA0);
		sendCommand(0xC0);
	}
}

void setTextXY(unsigned char Row, unsigned char Column)
{
	if (Drive_IC == SSD1327)
	{
		//Column Address
		sendCommand(0x15);             /* Set Column Address */
		sendCommand((uint8_t)(0x08 + (Column * 4)));  /* Start Column: Start from 8 */
		sendCommand(0x37);             /* End Column */
		// Row Address
		sendCommand(0x75);             /* Set Row Address */
		sendCommand((uint8_t)(0x00 + (Row * 8)));     /* Start Row*/
		sendCommand((uint8_t)(0x07 + (Row * 8)));     /* End Row*/
	}
	else if (Drive_IC == SH1107G)
	{
		sendCommand(0xb0 + (Row & 0x0F));  // set page/row
		sendCommand(0x10 + ((Column >> 4) & 0x07));  // set column high 3 bytex
		sendCommand(Column & 0x0F);  // set column low 4 byte
	}
}

void clearDisplay(void)
{
	unsigned char i, j;

	if (Drive_IC == SSD1327)
	{
		for (j = 0; j < 48; j++)
		{
			for (i = 0; i < 96; i++)  //clear all columns
			{
				sendData(0x00);
			}
		}
	}
	else if (Drive_IC == SH1107G)
	{
		for (i = 0; i < 16; i++) {
			sendCommand((uint8_t)(0xb0 + i));
			sendCommand(0x0);
			sendCommand(0x10);
			for (j = 0; j < 128; j++) {
				sendData(0x00);
			}
		}
	}
}

void setGrayLevel(unsigned char grayLevel)
{
	grayH = (uint8_t)((grayLevel << 4) & 0xF0);
	grayL = (uint8_t)(grayLevel & 0x0F);
}

void putChar(unsigned char C)
{
	if (C < 32 || C > 127) //Ignore non-printable ASCII characters. This can be modified for multilingual font.
	{
		C = ' '; //Space
	}

	if (Drive_IC == SSD1327)
	{
		for (int i = 0; i < 8; i = i + 2)
		{
			for (int j = 0; j < 8; j++)
			{
				// Character is constructed two pixel at a time using vertical mode from the default 8x8 font
				char c = 0x00;
				char bit1 = (BasicFont[C - 32][i] >> j) & 0x01;
				char bit2 = (BasicFont[C - 32][i + 1] >> j) & 0x01;
				// Each bit is changed to a nibble
				c |= (bit1 == 1) ? grayH : 0x00;
				c |= (bit2 == 1) ? grayL : 0x00;

				sendData(c);
			}
		}
	}
	else if (Drive_IC == SH1107G)
	{
		for (int i = 0; i < 8; i++)
		{
			//read bytes from code memory
			sendData((BasicFont[C - 32][i])); //font array starts at 0, ASCII starts at 32. Hence the translation
		}
	}
}

void putString(const char* String)
{
	unsigned char i = 0;
	while (String[i])
	{
		putChar(String[i]);
		i++;
	}
}

unsigned char putNumber(long long_num)
{
	unsigned char char_buffer[10] = "";
	unsigned char i = 0;
	unsigned char f = 0;

	if (long_num < 0)
	{
		f = 1;
		putChar('-');
		long_num = -long_num;
	}
	else if (long_num == 0)
	{
		f = 1;
		putChar('0');
		return f;
	}

	while (long_num > 0)
	{
		char_buffer[i++] = long_num % 10;
		long_num /= 10;
	}

	f = f + i;
	for (; i > 0; i--)
	{
		putChar('0' + char_buffer[i - 1]);
	}
	return f;

}

void drawBitmap(const unsigned char* bitmaparray, int bytes)
{
	if (Drive_IC == SSD1327)
	{
		char localAddressMode = addressingMode;
		if (addressingMode != HORIZONTAL_MODE)
		{
			//Bitmap is drawn in horizontal mode
			setHorizontalMode();
		}

		for (int i = 0; i < bytes; i++)
		{

			for (int j = 0; j < 8; j = j + 2)
			{
				char c = 0x00;
				char bit1 = (uint8_t)(bitmaparray[i] << j & 0x80);
				char bit2 = (uint8_t)(bitmaparray[i] << (j + 1) & 0x80);

				// Each bit is changed to a nibble
				c |= (bit1) ? grayH : 0x00;
				// Each bit is changed to a nibble
				c |= (bit2) ? grayL : 0x00;
				sendData(c);
			}
		}
		if (localAddressMode == VERTICAL_MODE)
		{
			//If Vertical Mode was used earlier, restore it.
			setVerticalMode();
		}
	}
	else if (Drive_IC == SH1107G)
	{
		uint8_t Row = 0, column_l = 0x00, column_h = 0x10;

		setHorizontalMode();
		for (int i = 0; i < bytes; i++)
		{
			sendCommand((uint8_t)(0xb0 + Row));
			sendCommand(column_l);
			sendCommand(column_h);

			char bits = (char)bitmaparray[i];
			char tmp = 0x00;
			for (int b = 0; b < 8; b++)
			{
				tmp |= ((bits >> (7 - b)) & 0x01) << b;
			}
			sendData(tmp);
			// delay(10);
			Row++;
			if (Row >= 16) {
				Row = 0;
				column_l++;
				if (column_l >= 16) {
					column_l = 0x00;
					column_h += 0x01;
				}
			}
		}
	}
}

void setHorizontalScrollProperties(bool direction, unsigned char startRow, unsigned char endRow, unsigned char startColumn, unsigned char endColumn, unsigned char scrollSpeed)
{
	/*
Use the following defines for 'direction' :

 Scroll_Left
 Scroll_Right

Use the following defines for 'scrollSpeed' :

 Scroll_2Frames
 Scroll_3Frames
 Scroll_4Frames
 Scroll_5Frames
 Scroll_25Frames
 Scroll_64Frames
 Scroll_128Frames
 Scroll_256Frames

*/

	if (Scroll_Right == direction)
	{
		//Scroll Right
		sendCommand(0x27);
	}
	else
	{
		//Scroll Left  
		sendCommand(0x26);
	}
	sendCommand(0x00);       //Dummmy byte
	sendCommand(startRow);
	sendCommand(scrollSpeed);
	sendCommand(endRow);
	sendCommand((uint8_t)(startColumn + 8));
	sendCommand((uint8_t)(endColumn + 8));
	sendCommand(0x00);      //Dummmy byte

}

void activateScroll(void)
{
	sendCommand(SeeedGrayOLED_Activate_Scroll_Cmd);
}

void deactivateScroll(void)
{
	sendCommand(SeeedGrayOLED_Dectivate_Scroll_Cmd);
}

void setNormalDisplay(void)
{
	sendCommand(SeeedGrayOLED_Normal_Display_Cmd);
}

void setInverseDisplay(void)
{
	sendCommand(SeeedGrayOLED_Inverse_Display_Cmd);
}