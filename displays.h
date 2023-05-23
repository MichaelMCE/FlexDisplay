
#ifndef _DISPLAYS_H_
#define _DISPLAYS_H_

#define CALC_PITCH_1(w)			(((w)>>3)+(((w)&0x07)!=0))	// 1bit packed, calculate number of storage bytes per row given width (of glyph)
#define CALC_PITCH_16(w)		((w)*sizeof(uint16_t))		// 16bit (8 bits per byte)


void tft_update ();
void tft_init ();
uint8_t *tft_getBuffer ();
void tft_clear (const uint16_t colour);



#endif

