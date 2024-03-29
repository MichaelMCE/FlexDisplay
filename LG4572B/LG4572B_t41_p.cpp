
/*
	Tab @ 4 spaces
*/


#if USE_FLEXTFT_LG4572B

#include "LG4572B_t41_p.h"

static LG4572B_t41_p STORAGETYPE lcd = LG4572B_t41_p(TFT_RS, TFT_CS, TFT_RST, TFT_BL);


FLASHMEM LG4572B_t41_p::LG4572B_t41_p (int8_t dc, int8_t cs, int8_t rst, int8_t bl) 
{
	_dc = dc;
	_cs = cs;
	_rst = rst;
	_bl = bl;
}


// LG4572B
PROGMEM static const setting_table_t power_on_setting_table[] = {
	{0xC0, 			2, {0x01, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0},		// Internal Oscillator Setting
	{0x20,			0, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0}, 	// Display Inversion Off
	{0x36,			1, {0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0},		// Memory Access Control
	{0x3a,			1, {0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0},		// Interface Pixel Format
	{0x35,			1, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0},		// Tearing Effect Line On
	{0xb1,			3, {0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0},		// RGB Interface Setting
	{0xb2,			2, {0x00, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0},		// Panel Characteristics Setting
	{0xb3,			1, {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0},		// Panel Drive Setting
	{0xb4,			1, {0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0},		// Display Mode Control
	{0xb5,			5, {0x10, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0}, 	// Display Control 1
	{0xb6,			6, {0x0b, 0x0f, 0x3c, 0x13, 0x13, 0xe8, 0x00, 0x00, 0x00, 0x00}, 0},		// Display Control 2
	{0xb7,			5, {0x46, 0x06, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0},		// Display Control 3
	{0xc0,			2, {0x01, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0},		// Internal Oscillator Setting
	{0xc3,			5, {0x07, 0x03, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00}, 40},	// Power Control 3
	{0xc4,			6, {0x12, 0x24, 0x12, 0x12, 0x02, 0x6b, 0x00, 0x00, 0x00, 0x00}, 20},	// Power Control 4
	{0xc5,			1, {0x69, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 10},	// Power Control 5
	{0xc6,			2, {0x41, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 10},	// Power Control 6
	{0xd0,			9, {0x01, 0x26, 0x71, 0x16, 0x04, 0x03, 0x51, 0x15, 0x04, 0x00}, 0},		// Set Positive Gamma Curve for Red
	{0xd1,			9, {0x01, 0x26, 0x71, 0x16, 0x04, 0x03, 0x51, 0x15, 0x04, 0x00}, 0},		// Set Negative Gamma Curve for Red
	{0xd2,			9, {0x01, 0x26, 0x71, 0x16, 0x04, 0x03, 0x51, 0x15, 0x04, 0x00}, 0},		// Set Positive Gamma Curve for Green
	{0xd3,			9, {0x01, 0x26, 0x71, 0x16, 0x04, 0x03, 0x51, 0x15, 0x04, 0x00}, 0},		// Set Negative Gamma Curve for Green
	{0xd4,			9, {0x01, 0x26, 0x71, 0x16, 0x04, 0x03, 0x51, 0x15, 0x04, 0x00}, 0},		// Set Positive Gamma Curve for Blue
	{0xd5,			9, {0x01, 0x26, 0x71, 0x16, 0x04, 0x03, 0x51, 0x15, 0x04, 0x00}, 0},		// Set Negative Gamma Curve for Blue
	
	{0x11,			0, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 100}, 
	{0x29,			0, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 10}
};

/* 
FLASHMEM void LG4572B_t41_p::applyCommands (const uint8_t *commands)
{
	uint8_t cmd, x, numArgs;
  
	while ((cmd = *commands++) > 0){
		x = *commands++;
		numArgs = x&0x7F;
		SglBeatWR_nPrm_8(cmd, commands, numArgs);

		commands += numArgs;
		if (*commands > 0)			// waitMs
			delay(*commands);
		commands++;
	}
}*/

FLASHMEM void LG4572B_t41_p::LCDSettingTableWrite (const setting_table_t *table)
{
	SglBeatWR_nPrm_8(table->command, table->parameter, table->parameters);
	if ((table->wait) > 0)
		delay(table->wait);
}

FLASHMEM void LG4572B_t41_p::setRotation (const uint8_t r) 
{ 
	_rotation = r&0x03;

	switch (_rotation){
      case 0:	
      case 2: _width = LG4572B_TFTWIDTH;
             _height = LG4572B_TFTHEIGHT;
             break;
      case 1: 
      case 3: _width = LG4572B_TFTHEIGHT;
             _height = LG4572B_TFTWIDTH;
			 break;
	}

	SglBeatWR_nPrm_8(LG4572B_MADCTL, &MADCTL[_rotation], 1);
	delay(5);
}

FLASHMEM void LG4572B_t41_p::init_display ()
{		
	// toggle RST low to resetset
	digitalWriteFast(_rst, LOW);
	delay(20);
	digitalWriteFast(_rst, HIGH);
	delay(20);

	CSLow();
	
	//applyCommands(initCommands);
	for (int i = 0; i < (int)(sizeof(power_on_setting_table) / sizeof(setting_table_t)); i++)
		LCDSettingTableWrite(&power_on_setting_table[i]);

	CSHigh();
	delay(5);
}

FLASHMEM void LG4572B_t41_p::setBacklight (const uint8_t value)
{
    analogWrite(_bl, value);
}

FLASHMEM void LG4572B_t41_p::begin (const uint8_t baud_div) 
{
	switch (baud_div){					// with a base freq of 240mhz
    case 2:  _baud_div = 120; break;
    case 4:  _baud_div = 60; break;
    case 8:  _baud_div = 30; break;
    case 12: _baud_div = 20; break;
    case 20: _baud_div = 12; break;
    case 24: _baud_div = 10; break;
    case 27: _baud_div = 9; break;
    case 30: _baud_div = 8; break;
    case 34: _baud_div = 7; break;
    case 40: _baud_div = 6; break;
    case 48: _baud_div = 5; break;
    case 60: _baud_div = 4; break;
    case 80: _baud_div = 3; break;
    case 120:_baud_div = 2; break;
    case 240:_baud_div = 1; break;
    default: _baud_div = 20; break;
	}
  
	pinMode(_cs, OUTPUT);  // CS
	pinMode(_dc, OUTPUT);  // DC
	pinMode(_rst, OUTPUT); // RST
	pinMode(_bl, OUTPUT);
  
	*(portControlRegister(_cs)) = 0xFF;
	*(portControlRegister(_dc)) = 0xFF;
	*(portControlRegister(_rst)) = 0xFF;
  
	digitalWriteFast(_cs, HIGH);
	digitalWriteFast(_dc, HIGH);
	digitalWriteFast(_rst, HIGH);

	FlexIO_Init();
	displayInit();

	_width  = LG4572B_TFTWIDTH;
	_height = LG4572B_TFTHEIGHT;

}

FLASHMEM void LG4572B_t41_p::onCompleteCB (CBF callback)
{
	_callback = callback;
	isCB = true;
}

FASTRUN void LG4572B_t41_p::setAddrWindow (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) 
{
	uint8_t CommandValue[4];
		
	uint8_t Command = LG4572B_CASET;
	CommandValue[0U] = x1 >> 8U;
	CommandValue[1U] = x1 & 0xFF;
	CommandValue[2U] = x2 >> 8U;
	CommandValue[3U] = x2 & 0xFF;
	SglBeatWR_nPrm_8(Command, CommandValue, 4U);
		
	Command = LG4572B_PASET;
	CommandValue[0U] = y1 >> 8U;
	CommandValue[1U] = y1 & 0xFF;
	CommandValue[2U] = y2 >> 8U;
	CommandValue[3U] = y2 & 0xFF;
	SglBeatWR_nPrm_8(Command, CommandValue, 4U);

}

FASTRUN void LG4572B_t41_p::pushPixels16bit (const uint16_t *pcolors, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint32_t area = ((x2-x1)+1) * ((y2-y1)+1);
  
	if (!((_lastx1 == x1) && (_lastx2 == x2) && (_lasty1 == y1) && (_lasty2 == y2))){
		while (WR_IRQTransferDone == false){
    		//Wait for any DMA transfers to complete
		}

		setAddrWindow(x1, y1, x2, y2);
		_lastx1 = x1; _lastx2 = x2; _lasty1 = y1; _lasty2 = y2;
	}
  
	SglBeatWR_nPrm_16(LG4572B_RAMWR, pcolors, area);
}

FASTRUN void LG4572B_t41_p::pushPixels16bitAsync (const uint16_t *pcolors, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{	
	while (WR_IRQTransferDone == false){
    	//Wait for any DMA transfers to complete
	}
	uint32_t area = ((x2-x1)+1) * ((y2-y1)+1);
  
	if (!((_lastx1 == x1) && (_lastx2 == x2) && (_lasty1 == y1) && (_lasty2 == y2))){
  		setAddrWindow(x1, y1, x2, y2);
  		_lastx1 = x1; _lastx2 = x2; _lasty1 = y1; _lasty2 = y2;
	}
  
  	MulBeatWR_nPrm_IRQ(LG4572B_RAMWR, pcolors, area);
}

FLASHMEM void LG4572B_t41_p::displayInit () 
{
	setBacklight(TFT_INTENSITY);
	init_display();
	//setRotation(3);
}

FASTRUN void LG4572B_t41_p::CSLow () 
{
	digitalWriteFast(_cs, LOW);      // Select TFT
}

FASTRUN void LG4572B_t41_p::CSHigh () 
{
	digitalWriteFast(_cs, HIGH);     // Deselect TFT
}

FASTRUN void LG4572B_t41_p::DCLow () 
{
	digitalWriteFast(_dc, LOW);      // Writing command to TFT
}

FASTRUN void LG4572B_t41_p::DCHigh () 
{
	digitalWriteFast(_dc, HIGH);     // Writing data to TFT
}


#pragma GCC push_options
#pragma GCC optimize ("O0")   
FASTRUN void LG4572B_t41_p::microSecondDelay ()
{
	for (uint32_t volatile  i = 0; i < 2; i++)
		__asm__ volatile ("nop\n\t");
	//delayMicroseconds(1);
}
#pragma GCC pop_options


FASTRUN void LG4572B_t41_p::gpioWrite ()
{
	pFlex->setIOPinToFlexMode(36);
	pinMode(37, OUTPUT);
	digitalWriteFast(37, HIGH);
}

FASTRUN void LG4572B_t41_p::gpioRead ()
{
	pFlex->setIOPinToFlexMode(37);
	pinMode(36, OUTPUT);
	digitalWriteFast(36, HIGH);
}

FASTRUN void LG4572B_t41_p::FlexIO_Init ()
{
	/* Get a FlexIO channel */
    pFlex = FlexIOHandler::flexIOHandler_list[2]; // use FlexIO3
    /* Pointer to the port structure in the FlexIO channel */
    p = &pFlex->port();
    /* Pointer to the hardware structure in the FlexIO channel */
    hw = &pFlex->hardware();
    /* Basic pin setup */

    pinMode(19, OUTPUT); // FlexIO3:0 D0
    pinMode(18, OUTPUT); // FlexIO3:1 |
    pinMode(14, OUTPUT); // FlexIO3:2 |
    pinMode(15, OUTPUT); // FlexIO3:3 |
    pinMode(40, OUTPUT); // FlexIO3:4 |
    pinMode(41, OUTPUT); // FlexIO3:5 |
    pinMode(17, OUTPUT); // FlexIO3:6 |
    pinMode(16, OUTPUT); // FlexIO3:7 D7

    /*
    pinMode(22, OUTPUT); // FlexIO3:8 D8
    pinMode(23, OUTPUT); // FlexIO3:9  |
    pinMode(20, OUTPUT); // FlexIO3:10 |
    pinMode(21, OUTPUT); // FlexIO3:11 |
    pinMode(38, OUTPUT); // FlexIO3:12 |
    pinMode(39, OUTPUT); // FlexIO3:13 |
    pinMode(26, OUTPUT); // FlexIO3:14 |
    pinMode(27, OUTPUT); // FlexIO3:15 D15
    */

    
    pinMode(36, OUTPUT); // FlexIO3:18 WR
    pinMode(37, OUTPUT); // FlexIO3:19 RD

    digitalWrite(36, HIGH);
    digitalWrite(37, HIGH);

    /* High speed and drive strength configuration */
    *(portControlRegister(36)) = 0xFF;
    *(portControlRegister(37)) = 0xFF; 

    *(portControlRegister(19)) = 0xFF;
    *(portControlRegister(18)) = 0xFF;
    *(portControlRegister(14)) = 0xFF;
    *(portControlRegister(15)) = 0xFF;
    *(portControlRegister(40)) = 0xFF;
    *(portControlRegister(41)) = 0xFF;
    *(portControlRegister(17)) = 0xFF;
    *(portControlRegister(16)) = 0xFF;

    /* Set clock */
    pFlex->setClockSettings(3, 0, 0); // (480 MHz source, 1+1, 1+0) >> 480/2/1 >> 240Mhz

    /* Set up pin mux */
    pFlex->setIOPinToFlexMode(36);
    pFlex->setIOPinToFlexMode(37);

    pFlex->setIOPinToFlexMode(19);
    pFlex->setIOPinToFlexMode(18);
    pFlex->setIOPinToFlexMode(14);
    pFlex->setIOPinToFlexMode(15);
    pFlex->setIOPinToFlexMode(40);
    pFlex->setIOPinToFlexMode(41);
    pFlex->setIOPinToFlexMode(17);
    pFlex->setIOPinToFlexMode(16);
    
    /* Enable the clock */
    hw->clock_gate_register |= hw->clock_gate_mask;
    /* Enable the FlexIO with fast access */
    p->CTRL = FLEXIO_CTRL_FLEXEN;
    
}

#if 0
FASTRUN void LG4572B_t41_p::FlexIO_Config_SnglBeat_Read ()
{
    gpioWrite();

    p->CTRL &= ~FLEXIO_CTRL_FLEXEN;
    p->CTRL |=  FLEXIO_CTRL_SWRST;
    p->CTRL &= ~FLEXIO_CTRL_SWRST;

    gpioRead();

    /* Configure the shifters */
    p->SHIFTCFG[0] = 
         FLEXIO_SHIFTCFG_INSRC*(1)                                             /* Shifter input */
       | FLEXIO_SHIFTCFG_SSTOP(0)                                              /* Shifter stop bit disabled */
       | FLEXIO_SHIFTCFG_SSTART(0)                                             /* Shifter start bit disabled and loading data on enabled */
       | FLEXIO_SHIFTCFG_PWIDTH(7);                                            /* Bus width */
     
    p->SHIFTCTL[0] = 
        FLEXIO_SHIFTCTL_TIMSEL(0)                                              /* Shifter's assigned timer index */
      | FLEXIO_SHIFTCTL_TIMPOL*(0)                                             /* Shift on posedge of shift clock */
      | FLEXIO_SHIFTCTL_PINCFG(3)                                              /* Shifter's pin configured as output */
      | FLEXIO_SHIFTCTL_PINSEL(0)                                              /* Shifter's pin start index */
      | FLEXIO_SHIFTCTL_PINPOL*(0)                                             /* Shifter's pin active high */
      | FLEXIO_SHIFTCTL_SMOD(2);                                               /* Shifter mode as transmit */

    /* Configure the timer for shift clock */
    p->TIMCMP[0] = 
        (((1 * 2) - 1) << 8)                                                   /* TIMCMP[15:8] = number of beats x 2 – 1 */
      | ((_baud_div/2) - 1);                                                   /* TIMCMP[7:0] = baud rate divider / 2 – 1 */
    
    p->TIMCFG[0] = 
        FLEXIO_TIMCFG_TIMOUT(0)                                                /* Timer output logic one when enabled and not affected by reset */
      | FLEXIO_TIMCFG_TIMDEC(0)                                                /* Timer decrement on FlexIO clock, shift clock equals timer output */
      | FLEXIO_TIMCFG_TIMRST(0)                                                /* Timer never reset */
      | FLEXIO_TIMCFG_TIMDIS(2)                                                /* Timer disabled on timer compare */
      | FLEXIO_TIMCFG_TIMENA(2)                                                /* Timer enabled on trigger high */
      | FLEXIO_TIMCFG_TSTOP(0)                                                 /* Timer stop bit disabled */
      | FLEXIO_TIMCFG_TSTART*(0);                                              /* Timer start bit disabled */
    
    p->TIMCTL[0] = 
        FLEXIO_TIMCTL_TRGSEL((((0) << 2) | 1))                                 /* Timer trigger selected as shifter's status flag */
      | FLEXIO_TIMCTL_TRGPOL*(1)                                               /* Timer trigger polarity as active low */
      | FLEXIO_TIMCTL_TRGSRC*(1)                                               /* Timer trigger source as internal */
      | FLEXIO_TIMCTL_PINCFG(3)                                                /* Timer' pin configured as output */
      | FLEXIO_TIMCTL_PINSEL(19)                                               /* Timer' pin index: WR pin */
      | FLEXIO_TIMCTL_PINPOL*(1)                                               /* Timer' pin active low */
      | FLEXIO_TIMCTL_TIMOD(1);                                                /* Timer mode as dual 8-bit counters baud/bit */

   
	Serial.printf("CCM_CDCDR: %x\n", CCM_CDCDR);
	Serial.printf("VERID:%x PARAM:%x CTRL:%x PIN: %x\n", IMXRT_FLEXIO2_S.VERID, IMXRT_FLEXIO2_S.PARAM, IMXRT_FLEXIO2_S.CTRL, IMXRT_FLEXIO2_S.PIN);
	Serial.printf("SHIFTSTAT:%x SHIFTERR=%x TIMSTAT=%x\n", IMXRT_FLEXIO2_S.SHIFTSTAT, IMXRT_FLEXIO2_S.SHIFTERR, IMXRT_FLEXIO2_S.TIMSTAT);
	Serial.printf("SHIFTSIEN:%x SHIFTEIEN=%x TIMIEN=%x\n", IMXRT_FLEXIO2_S.SHIFTSIEN, IMXRT_FLEXIO2_S.SHIFTEIEN, IMXRT_FLEXIO2_S.TIMIEN);
	Serial.printf("SHIFTSDEN:%x SHIFTSTATE=%x\n", IMXRT_FLEXIO2_S.SHIFTSDEN, IMXRT_FLEXIO2_S.SHIFTSTATE);
	Serial.printf("SHIFTCTL:%x %x %x %x\n", IMXRT_FLEXIO2_S.SHIFTCTL[0], IMXRT_FLEXIO2_S.SHIFTCTL[1], IMXRT_FLEXIO2_S.SHIFTCTL[2], IMXRT_FLEXIO2_S.SHIFTCTL[3]);
	Serial.printf("SHIFTCFG:%x %x %x %x\n", IMXRT_FLEXIO2_S.SHIFTCFG[0], IMXRT_FLEXIO2_S.SHIFTCFG[1], IMXRT_FLEXIO2_S.SHIFTCFG[2], IMXRT_FLEXIO2_S.SHIFTCFG[3]);
	Serial.printf("TIMCTL:%x %x %x %x\n", IMXRT_FLEXIO2_S.TIMCTL[0], IMXRT_FLEXIO2_S.TIMCTL[1], IMXRT_FLEXIO2_S.TIMCTL[2], IMXRT_FLEXIO2_S.TIMCTL[3]);
	Serial.printf("TIMCFG:%x %x %x %x\n", IMXRT_FLEXIO2_S.TIMCFG[0], IMXRT_FLEXIO2_S.TIMCFG[1], IMXRT_FLEXIO2_S.TIMCFG[2], IMXRT_FLEXIO2_S.TIMCFG[3]);
	Serial.printf("TIMCMP:%x %x %x %x\n", IMXRT_FLEXIO2_S.TIMCMP[0], IMXRT_FLEXIO2_S.TIMCMP[1], IMXRT_FLEXIO2_S.TIMCMP[2], IMXRT_FLEXIO2_S.TIMCMP[3]);
	Serial.printf("FlexIO bus speed: %d\n", pFlex->computeClockRate());
    
    /* Enable FlexIO */
   p->CTRL |= FLEXIO_CTRL_FLEXEN;      

}
#endif

FASTRUN void LG4572B_t41_p::FlexIO_Config_SnglBeat ()
{
    gpioWrite();

    p->CTRL &= ~FLEXIO_CTRL_FLEXEN;
    p->CTRL |= FLEXIO_CTRL_SWRST;
    p->CTRL &= ~FLEXIO_CTRL_SWRST;

    

    /* Configure the shifters */
    p->SHIFTCFG[0] = 
         FLEXIO_SHIFTCFG_INSRC*(1)                                             /* Shifter input */
       | FLEXIO_SHIFTCFG_SSTOP(0)                                              /* Shifter stop bit disabled */
       | FLEXIO_SHIFTCFG_SSTART(0)                                             /* Shifter start bit disabled and loading data on enabled */
       | FLEXIO_SHIFTCFG_PWIDTH(7);                                            /* Bus width */
     
    p->SHIFTCTL[0] = 
        FLEXIO_SHIFTCTL_TIMSEL(0)                                              /* Shifter's assigned timer index */
      | FLEXIO_SHIFTCTL_TIMPOL*(0)                                             /* Shift on posedge of shift clock */
      | FLEXIO_SHIFTCTL_PINCFG(3)                                              /* Shifter's pin configured as output */
      | FLEXIO_SHIFTCTL_PINSEL(0)                                              /* Shifter's pin start index */
      | FLEXIO_SHIFTCTL_PINPOL*(0)                                             /* Shifter's pin active high */
      | FLEXIO_SHIFTCTL_SMOD(2);                                               /* Shifter mode as transmit */

    /* Configure the timer for shift clock */
    p->TIMCMP[0] = 
        (((1 * 2) - 1) << 8)                                                   /* TIMCMP[15:8] = number of beats x 2 – 1 */
      | ((_baud_div/2) - 1);                                                   /* TIMCMP[7:0] = baud rate divider / 2 – 1 */
    
    p->TIMCFG[0] = 
        FLEXIO_TIMCFG_TIMOUT(0)                                                /* Timer output logic one when enabled and not affected by reset */
      | FLEXIO_TIMCFG_TIMDEC(0)                                                /* Timer decrement on FlexIO clock, shift clock equals timer output */
      | FLEXIO_TIMCFG_TIMRST(0)                                                /* Timer never reset */
      | FLEXIO_TIMCFG_TIMDIS(2)                                                /* Timer disabled on timer compare */
      | FLEXIO_TIMCFG_TIMENA(2)                                                /* Timer enabled on trigger high */
      | FLEXIO_TIMCFG_TSTOP(0)                                                 /* Timer stop bit disabled */
      | FLEXIO_TIMCFG_TSTART*(0);                                              /* Timer start bit disabled */
    
    p->TIMCTL[0] = 
        FLEXIO_TIMCTL_TRGSEL((((0) << 2) | 1))                                 /* Timer trigger selected as shifter's status flag */
      | FLEXIO_TIMCTL_TRGPOL*(1)                                               /* Timer trigger polarity as active low */
      | FLEXIO_TIMCTL_TRGSRC*(1)                                               /* Timer trigger source as internal */
      | FLEXIO_TIMCTL_PINCFG(3)                                                /* Timer' pin configured as output */
      | FLEXIO_TIMCTL_PINSEL(18)                                               /* Timer' pin index: WR pin */
      | FLEXIO_TIMCTL_PINPOL*(1)                                               /* Timer' pin active low */
      | FLEXIO_TIMCTL_TIMOD(1);                                                /* Timer mode as dual 8-bit counters baud/bit */

	/* 
	Serial.printf("CCM_CDCDR: %x\n", CCM_CDCDR);
	Serial.printf("VERID:%x PARAM:%x CTRL:%x PIN: %x\n", IMXRT_FLEXIO2_S.VERID, IMXRT_FLEXIO2_S.PARAM, IMXRT_FLEXIO2_S.CTRL, IMXRT_FLEXIO2_S.PIN);
	Serial.printf("SHIFTSTAT:%x SHIFTERR=%x TIMSTAT=%x\n", IMXRT_FLEXIO2_S.SHIFTSTAT, IMXRT_FLEXIO2_S.SHIFTERR, IMXRT_FLEXIO2_S.TIMSTAT);
	Serial.printf("SHIFTSIEN:%x SHIFTEIEN=%x TIMIEN=%x\n", IMXRT_FLEXIO2_S.SHIFTSIEN, IMXRT_FLEXIO2_S.SHIFTEIEN, IMXRT_FLEXIO2_S.TIMIEN);
	Serial.printf("SHIFTSDEN:%x SHIFTSTATE=%x\n", IMXRT_FLEXIO2_S.SHIFTSDEN, IMXRT_FLEXIO2_S.SHIFTSTATE);
	Serial.printf("SHIFTCTL:%x %x %x %x\n", IMXRT_FLEXIO2_S.SHIFTCTL[0], IMXRT_FLEXIO2_S.SHIFTCTL[1], IMXRT_FLEXIO2_S.SHIFTCTL[2], IMXRT_FLEXIO2_S.SHIFTCTL[3]);
	Serial.printf("SHIFTCFG:%x %x %x %x\n", IMXRT_FLEXIO2_S.SHIFTCFG[0], IMXRT_FLEXIO2_S.SHIFTCFG[1], IMXRT_FLEXIO2_S.SHIFTCFG[2], IMXRT_FLEXIO2_S.SHIFTCFG[3]);
	Serial.printf("TIMCTL:%x %x %x %x\n", IMXRT_FLEXIO2_S.TIMCTL[0], IMXRT_FLEXIO2_S.TIMCTL[1], IMXRT_FLEXIO2_S.TIMCTL[2], IMXRT_FLEXIO2_S.TIMCTL[3]);
	Serial.printf("TIMCFG:%x %x %x %x\n", IMXRT_FLEXIO2_S.TIMCFG[0], IMXRT_FLEXIO2_S.TIMCFG[1], IMXRT_FLEXIO2_S.TIMCFG[2], IMXRT_FLEXIO2_S.TIMCFG[3]);
	Serial.printf("TIMCMP:%x %x %x %x\n", IMXRT_FLEXIO2_S.TIMCMP[0], IMXRT_FLEXIO2_S.TIMCMP[1], IMXRT_FLEXIO2_S.TIMCMP[2], IMXRT_FLEXIO2_S.TIMCMP[3]);
	Serial.printf("FlexIO bus speed: %d\n", pFlex->computeClockRate());
	*/  
    /* Enable FlexIO */
	p->CTRL |= FLEXIO_CTRL_FLEXEN;      

}

FASTRUN void LG4572B_t41_p::FlexIO_Config_MultiBeat ()
{
    //uint32_t i;
    uint8_t beats = SHIFTNUM * BEATS_PER_SHIFTER;    //Number of beats = number of shifters * beats per shifter
    /* Disable and reset FlexIO */
    p->CTRL &= ~FLEXIO_CTRL_FLEXEN;
    p->CTRL |= FLEXIO_CTRL_SWRST;
    p->CTRL &= ~FLEXIO_CTRL_SWRST;

    gpioWrite();

    /* Configure the shifters */
    for (int i = 0; i <= SHIFTNUM - 1; i++)
    {
        p->SHIFTCFG[i] =
            FLEXIO_SHIFTCFG_INSRC * (1U)             /* Shifter input from next shifter's output */
            | FLEXIO_SHIFTCFG_SSTOP(0U)              /* Shifter stop bit disabled */
            | FLEXIO_SHIFTCFG_SSTART(0U)             /* Shifter start bit disabled and loading data on enabled */
            | FLEXIO_SHIFTCFG_PWIDTH(7U);            /* 8 bit shift width */
    }

    p->SHIFTCTL[0] =
        FLEXIO_SHIFTCTL_TIMSEL(0)                    /* Shifter's assigned timer index */
        | FLEXIO_SHIFTCTL_TIMPOL * (0U)              /* Shift on posedge of shift clock */
        | FLEXIO_SHIFTCTL_PINCFG(3U)                 /* Shifter's pin configured as output */
        | FLEXIO_SHIFTCTL_PINSEL(0)                  /* Shifter's pin start index */
        | FLEXIO_SHIFTCTL_PINPOL * (0U)              /* Shifter's pin active high */
        | FLEXIO_SHIFTCTL_SMOD(2U);                  /* shifter mode transmit */

    for (int i = 1; i <= SHIFTNUM - 1; i++)
    {
        p->SHIFTCTL[i] =
            FLEXIO_SHIFTCTL_TIMSEL(0)                /* Shifter's assigned timer index */
            | FLEXIO_SHIFTCTL_TIMPOL * (0U)          /* Shift on posedge of shift clock */
            | FLEXIO_SHIFTCTL_PINCFG(0U)             /* Shifter's pin configured as output disabled */
            | FLEXIO_SHIFTCTL_PINSEL(0)              /* Shifter's pin start index */
            | FLEXIO_SHIFTCTL_PINPOL * (0U)          /* Shifter's pin active high */
            | FLEXIO_SHIFTCTL_SMOD(2U);
    }

    /* Configure the timer for shift clock */
    p->TIMCMP[0] =
        ((beats * 2U - 1) << 8)                      /* TIMCMP[15:8] = number of beats x 2 – 1 */
        | (_baud_div / 2U - 1U);                     /* TIMCMP[7:0] = shift clock divide ratio / 2 - 1 */

    p->TIMCFG[0] =   FLEXIO_TIMCFG_TIMOUT(0U)        /* Timer output logic one when enabled and not affected by reset */
                     | FLEXIO_TIMCFG_TIMDEC(0U)      /* Timer decrement on FlexIO clock, shift clock equals timer output */
                     | FLEXIO_TIMCFG_TIMRST(0U)      /* Timer never reset */
                     | FLEXIO_TIMCFG_TIMDIS(2U)      /* Timer disabled on timer compare */
                     | FLEXIO_TIMCFG_TIMENA(2U)      /* Timer enabled on trigger high */
                     | FLEXIO_TIMCFG_TSTOP(0U)       /* Timer stop bit disabled */
                     | FLEXIO_TIMCFG_TSTART * (0U);  /* Timer start bit disabled */

    p->TIMCTL[0] =
        FLEXIO_TIMCTL_TRGSEL(((SHIFTNUM - 1) << 2) | 1U)   /* Timer trigger selected as highest shifter's status flag */
        | FLEXIO_TIMCTL_TRGPOL * (1U)                      /* Timer trigger polarity as active low */
        | FLEXIO_TIMCTL_TRGSRC * (1U)                      /* Timer trigger source as internal */
        | FLEXIO_TIMCTL_PINCFG(3U)                         /* Timer' pin configured as output */
        | FLEXIO_TIMCTL_PINSEL(18)                         /* Timer' pin index: WR pin */
        | FLEXIO_TIMCTL_PINPOL * (1U)                      /* Timer' pin active low */
        | FLEXIO_TIMCTL_TIMOD(1U);                         /* Timer mode 8-bit baud counter */
    /* Enable FlexIO */
	p->CTRL |= FLEXIO_CTRL_FLEXEN;

   // configure interrupts
    attachInterruptVector(hw->flex_irq, ISR);
    NVIC_ENABLE_IRQ(hw->flex_irq);
    NVIC_SET_PRIORITY(hw->flex_irq, FLEXIO_ISR_PRIORITY);

    // disable interrupts until later
    p->SHIFTSIEN &= ~(1 << SHIFTER_IRQ);
    p->TIMIEN &= ~(1 << TIMER_IRQ);
}

FASTRUN void LG4572B_t41_p::SglBeatWR_nPrm_8 (uint32_t const cmd, const uint8_t *value = NULL, uint32_t const length = 0)
{
	while (WR_IRQTransferDone == false){
   	 //Wait for any DMA transfers to complete
	}
  

    FlexIO_Config_SnglBeat ();
 
    /* Assert CS, RS pins */
    //delay(1);
    CSLow();
    DCLow();

    /* Write command index */
    p->SHIFTBUF[0] = cmd;

    /*Wait for transfer to be completed */
    while(0 == (p->SHIFTSTAT & (1 << 0))){
    	
	}

    while(0 == (p->TIMSTAT & (1 << 0))){  
    	
	}

    /* De-assert RS pin */
    
    microSecondDelay();
    DCHigh();
    microSecondDelay();
    
    if (length){
        for (uint32_t i = 0; i < length; i++){    
            p->SHIFTBUF[0] = *value++;
            
            while(0 == (p->SHIFTSTAT & (1 << 0))){  
            	
            }
        }
        
        //while(0 == (p->TIMSTAT & (1 << 0))){  
        	
		//}
    }
    microSecondDelay();
    CSHigh();
    
    /* De-assert CS pin */
}

FASTRUN void LG4572B_t41_p::SglBeatWR_nPrm_16 (uint32_t const cmd, const uint16_t *value, uint32_t const length)
{
	while (WR_IRQTransferDone == false){
    	//Wait for any DMA transfers to complete
	}
	
    FlexIO_Config_SnglBeat();

    /* Assert CS, RS pins */
    CSLow();
    DCLow();
    //microSecondDelay();
    
    /* Write command index */
    p->SHIFTBUF[0] = cmd;

    /*Wait for transfer to be completed */
    while (0 == (p->TIMSTAT & (1 << 0))){
    	
	}

    microSecondDelay();
    /* De-assert RS pin */
    DCHigh();
    microSecondDelay();

    if (length){
		for(uint32_t i = 0; i < length-1U; i++){
			uint16_t buf = *value++;
		
			while (0 == (p->SHIFTSTAT & (1U << 0))){
            	
            }
            p->SHIFTBUF[0] = buf >> 8;

            while(0 == (p->SHIFTSTAT & (1U << 0))){
            	
            }
            p->SHIFTBUF[0] = buf & 0xFF;
        }
        
        uint16_t buf = *value++;
        /* Write the last byte */
        while (0 == (p->SHIFTSTAT & (1U << 0))){
        	
		}
        p->SHIFTBUF[0] = buf >> 8;

        while (0 == (p->SHIFTSTAT & (1U << 0))){
        	
        }
       // p->TIMSTAT |= (1U << 0);

        p->SHIFTBUF[0] = buf & 0xFF;

        /*Wait for transfer to be completed */
        while (0 == (p->TIMSTAT |= (1U << 0))){
        	
        }
    }
    microSecondDelay();
    CSHigh();
}

LG4572B_t41_p *LG4572B_t41_p::IRQcallback = nullptr;

FASTRUN void LG4572B_t41_p::MulBeatWR_nPrm_IRQ (uint32_t const cmd,  const void *value, uint32_t const length) 
{
	while (WR_IRQTransferDone == false){
   		//Wait for any DMA transfers to complete
	}
  
    FlexIO_Config_SnglBeat();
    CSLow();
    DCLow();

    /* Write command index */
    p->SHIFTBUF[0] = cmd;

    /* Wait for transfer to complete */
    while(0 == (p->TIMSTAT & (1 << 0))){  
    	
	}
        
    microSecondDelay();
    /* De-assert RS pin */
    DCHigh();
    microSecondDelay();


    FlexIO_Config_MultiBeat();
    WR_IRQTransferDone = false;
    uint32_t bytes = length*2U;

    bursts_to_complete = bytes / BYTES_PER_BURST;

    int remainder = bytes % BYTES_PER_BURST;
    if (remainder != 0){
        memset(finalBurstBuffer, 0, sizeof(finalBurstBuffer));
        memcpy(finalBurstBuffer, (uint8_t*)value + bytes - remainder, remainder);
        bursts_to_complete++;
    }

    bytes_remaining = bytes;
    readPtr = (uint32_t*)value;
    //Serial.printf ("arg addr: %x, readPtr addr: %x \n", value, readPtr);
    //Serial.printf("START::bursts_to_complete: %d bytes_remaining: %d \n", bursts_to_complete, bytes_remaining);
  
    uint8_t beats = SHIFTNUM * BEATS_PER_SHIFTER;
    p->TIMCMP[0] = ((beats * 2U - 1) << 8) | (_baud_div / 2U - 1U);
    p->TIMSTAT = (1 << TIMER_IRQ); // clear timer interrupt signal
    
    asm("dsb");

    IRQcallback = this;
    // enable interrupts to trigger bursts
    p->TIMIEN |= (1 << TIMER_IRQ);
    p->SHIFTSIEN |= (1 << SHIFTER_IRQ);
    
}

FASTRUN void LG4572B_t41_p::_onCompleteCB ()
{
	if (_callback){
		_callback();
	}
	return;
}

FASTRUN void LG4572B_t41_p::flexIRQ_Callback ()
{
  
	if (p->TIMSTAT & (1 << TIMER_IRQ)){ // interrupt from end of burst
        p->TIMSTAT = (1 << TIMER_IRQ); // clear timer interrupt signal
        bursts_to_complete--;
        
        if (bursts_to_complete == 0){
            p->TIMIEN &= ~(1 << TIMER_IRQ); // disable timer interrupt
            
            asm("dsb");
            
            WR_IRQTransferDone = true;
            microSecondDelay();
            CSHigh();
            _onCompleteCB();
            return;
        }
    }

    if (p->SHIFTSTAT & (1 << SHIFTER_IRQ)){ // interrupt from empty shifter buffer
        // note, the interrupt signal is cleared automatically when writing data to the shifter buffers
        
        if (bytes_remaining == 0){ // just started final burst, no data to load
            p->SHIFTSIEN &= ~(1 << SHIFTER_IRQ); // disable shifter interrupt signal
            
        }else if (bytes_remaining < BYTES_PER_BURST){ // just started second-to-last burst, load data for final burst
            uint8_t beats = bytes_remaining / BYTES_PER_BEAT;
            p->TIMCMP[0] = ((beats * 2U - 1) << 8) | (_baud_div / 2U - 1); // takes effect on final burst
            readPtr = finalBurstBuffer;
            bytes_remaining = 0;
            
            for (int i = 0; i < SHIFTNUM; i++){
                uint32_t data = *readPtr++;
                p->SHIFTBUFBYS[i] = ((data >> 16) & 0xFFFF) | ((data << 16) & 0xFFFF0000);
            }
        }else{
            bytes_remaining -= BYTES_PER_BURST;
            for (int i = 0; i < SHIFTNUM; i++){
                uint32_t data = *readPtr++;
                p->SHIFTBUFBYS[i] = ((data >> 16) & 0xFFFF) | ((data << 16) & 0xFFFF0000);
			}
		}
	}
    asm("dsb");
}



FASTRUN void LG4572B_t41_p::ISR ()
{
	asm("dsb");
	IRQcallback->flexIRQ_Callback();
}

#endif
