

#include "common.h"

#if USE_FLEXTFT_ILI948x

#include "ILI948x/ILI948x_t41_p.cpp"


static ILI948x_t41_p STORAGETYPE lcd = ILI948x_t41_p(TFT_RS, TFT_CS, TFT_RST);
static uint8_t STORAGETYPE tft_buffer[CALC_PITCH_16(TFT_WIDTH)*TFT_HEIGHT];



static void tft_clearFrame (void *buffer, const uint16_t colour)
{
	uint16_t *pixels = (uint16_t*)buffer;
	
	int tPixels = TFT_WIDTH * TFT_HEIGHT;
	while (tPixels--) pixels[tPixels] = colour;		
}

void tft_init ()
{
	lcd.begin(TFT_SPEED);
	lcd.setRotation(3);
}

void tft_update ()
{
	lcd.pushPixels16bit((uint16_t*)tft_buffer, 0, 0, TFT_WIDTH-1, TFT_HEIGHT-1);
	//lcd.pushPixels16bitAsync((uint16_t*)tft_buffer, 0, 0, TFT_WIDTH-1, TFT_HEIGHT-1);
}

void tft_clear (const uint16_t colour)
{
	tft_clearFrame((void*)tft_buffer, colour);
}

uint8_t *tft_getBuffer ()
{
	return (uint8_t*)tft_buffer;
}


#endif

