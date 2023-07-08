

#include "config.h"

#if USE_FLEXTFT_ILI9486

#include "ILI9486/ILI9486_t41_p.cpp"


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

void tft_update_area (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	lcd.pushPixels16bit((uint16_t*)tft_buffer, x1, y1, x2, y2);
}

void tft_clear (const uint16_t colour)
{
	tft_clearFrame((void*)tft_buffer, colour);
}

uint8_t *tft_getBuffer ()
{
	return (uint8_t*)tft_buffer;
}

void tft_rotate (const uint8_t rotation)
{
	lcd.setRotation(rotation);
}

#endif

