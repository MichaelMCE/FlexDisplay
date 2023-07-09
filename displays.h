
#ifndef _DISPLAYS_H_
#define _DISPLAYS_H_



#include "ILI9486/ILI9486_t41_p.h"
#include "S6D04D1/S6D04D1_t41_p.h"
#include "ILI9806/ILI9806_t41_p.h"
#include "RM68120/RM68120_t41_p.h"
#include "R61529/R61529_t41_p.h"
#include "LG4572B/LG4572B_t41_p.h"
#include "NT35510/NT35510_t41_p.h"
#include "NT35516/NT35516_t41_p.h"



#define CALC_PITCH_1(w)			(((w)>>3)+(((w)&0x07)!=0))	// 1bit packed, calculate number of storage bytes per row given width (of glyph)
#define CALC_PITCH_16(w)		((w)*sizeof(uint16_t))		// 16bit (8 bits per byte)


void tft_update ();
void tft_update_area (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void tft_init ();
void tft_clear (const uint16_t colour);
void tft_rotate (const uint8_t rotation);
uint8_t *tft_getBuffer ();


#endif

