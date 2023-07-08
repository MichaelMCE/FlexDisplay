

#include "config.h"

#if USE_FLEXTFT_LG4572B

#include "LG4572B/LG4572B_t41_p.cpp"


#if USE_STRIP_RENDERER
#define TFT_HEIGHT_SPLIT (SPLIT_RENDERER_HEIGHT)
#else
#define TFT_HEIGHT_SPLIT (TFT_HEIGHT)
#endif


static LG4572B_t41_p STORAGETYPE lcd = LG4572B_t41_p(TFT_RS, TFT_CS, TFT_RST, TFT_BL);
static uint8_t STORAGETYPE tft_buffer[CALC_PITCH_16(TFT_WIDTH)*TFT_HEIGHT_SPLIT];



static void tft_clearFrame (void *buffer, const uint16_t colour)
{
	uint16_t *pixels = (uint16_t*)buffer;
	
	int tPixels = TFT_WIDTH * TFT_HEIGHT_SPLIT;
	while (tPixels--) pixels[tPixels] = colour;		
}

void tft_init ()
{
	lcd.begin(TFT_SPEED);


// increase PSRAM clocks to max stable, ~130MHZ
#if USE_EXTMEM_BUFFER	
	CCM_CBCMR = (CCM_CBCMR & ~(CCM_CBCMR_FLEXSPI2_PODF_MASK |
				 CCM_CBCMR_FLEXSPI2_CLK_SEL_MASK))			|
				 CCM_CBCMR_FLEXSPI2_PODF(3) 				|
				 CCM_CBCMR_FLEXSPI2_CLK_SEL(3);
#endif
}

void tft_update ()
{
	lcd.pushPixels16bit((uint16_t*)tft_buffer, 0, 0, TFT_WIDTH-1, TFT_HEIGHT_SPLIT-1);
	//lcd.pushPixels16bitAsync((uint16_t*)tft_buffer, 0, 0, TFT_WIDTH-1, TFT_HEIGHT-1);
}

void tft_update_area (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	lcd.pushPixels16bit((uint16_t*)tft_buffer, x1, y1, x2, y2);
	//lcd.pushPixels16bitAsync((uint16_t*)tft_buffer, x1, y1, x2, y2);
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

