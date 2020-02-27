/* ledstripews2813.c : WS2813 addressable ledstripe library
 *
 * DESCRIPTION : this library manage a single ledstrip attached to a GPIO port. It implement WS2813 NZR protocole 
 *
 * STATUS : DRAFT
 * VERSION : 0.1
 * AUTHOR : N. CLERC
 *
 */

#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include <applibs/gpio.h>
#include <applibs/log.h>

#define PIXEL_RED_OFFSET    (1)
#define PIXEL_GREEN_OFFSET  (0)
#define PIXEL_BLUE_OFFSET   (2)


int _fd_gpio = -1;      // File Descriptor for accessing GPIO pin and send ws2813 data
int _pixelCount = -1;    // pixel count
unsigned char* _pixels=NULL; // raw storage for pixel : GRB order !

// Initialize 
int ledstripe_init(int totalpixelCount,int fd_gpio)
{
    if (_pixels != NULL)
    {
        Log_Debug("ERROR ledstripe_init(): already initialized\n");
        return -1;
    }
    if (totalpixelCount < 1)
    {
        Log_Debug("ERROR ledstripe_init(): ledstripe length must be 1 or higher\n");
        return -1;
    }
    _pixelCount = totalpixelCount;
    _fd_gpio = fd_gpio;
 
    // Allocate storage for pixel raw value
    _pixels = (unsigned char*) malloc((sizeof(char) * 3 * _pixelCount));
    
    // initia all pixels composante to 0 (black/off pixel)
    memset(_pixels, 0, sizeof(char) * 3 * _pixelCount);


    if (fd_gpio >= 0)
    {
        // TODO : initialize Timer 
    }
    else
    {
        Log_Debug("WARNING ledstripe_init(): missing GPIO FD for ws2813 data pin.\n");
    }
    return -1;
}

// pixelId is zero-based index
void ledstripe_setpixel(int pixelId, unsigned char red, unsigned char green, unsigned char blue)
{
    if (pixelId < 0 || pixelId >= _pixelCount)
    {
        Log_Debug("ERROR ledstripe_setpixel(): invalid pixel ID (%d)\n", pixelId);
        return;
    }
    Log_Debug("DEBUG: ledstripe_setpixel(): ID=%d   R=%x  G=%x  B=%x\n",pixelId,red,green,blue);
 
    _pixels[pixelId * 3 + PIXEL_RED_OFFSET] = red;
    _pixels[pixelId * 3 + PIXEL_GREEN_OFFSET] = green;
    _pixels[pixelId * 3 + PIXEL_BLUE_OFFSET] = blue;

   
}
