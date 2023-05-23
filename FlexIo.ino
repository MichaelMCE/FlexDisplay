
// S6D04D1 and ILI9486 display driver
//
// Select your display, USE_FLEXTFT_S6D04D1 or USE_FLEXTFT_ILI948x, in common.h 

#include "common.h"


#define DWIDTH		TFT_WIDTH
#define DHEIGHT		TFT_HEIGHT



void setup ()
{
	//Serial.begin(9600);
	//while (!Serial);

	tft_init();
	tft_clear(0x0000);		// 16bit colour

}

void loop ()
{
	uint8_t *pixels = tft_getBuffer();
	if (pixels){
		//do something
	}
	
	
	tft_update();
	delay(1000);
}

