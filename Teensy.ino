/* 

Intended for compiling with:
https://github.com/MichaelMCE/TeensyFlexIoDisplay

Or alternatively roll your own:
Add your own display wrapper by implementing the below functions around your display driver

	void tft_init (void)
	void tft_clear (uint16_t)
	uint8_t *tft_getBuffer (void)
	void tft_update (void)
	tft_rotate (uint8_t)


TODO: write area destination x/y

*/


#include <Arduino.h>
#include "config.h"
#include "librawhiddesc.h"
#if USE_STARTUP_IMAGE
#include "startup_256x142_16.h"
#endif


/*
	VID:0x16C0
	PID:0x0486
	Inf:0
	Ep:0x04
	
	Use libusb inf-wizard to install a driver for the 'RawHid interface 0' device.
*/

typedef struct _recvData{
	uint8_t *readIn;
	int inCt;
}recvDataCtx_t;



#define COLOUR_24TO16(c)	((uint16_t)(((((c)>>16)&0xF8)<<8) | ((((c)>>8)&0xFC)<<3) | (((c)&0xF8)>>3)))
#define PACKET_SIZE			RAWHID_RX_SIZE_480			// Dont change - bad things happen 

static config_t config;
static uint8_t recvBuffer[PACKET_SIZE];




static inline int usb_recv (void *buffer)
{
	return RawHID.recv(buffer, 10);
	
	//return usb_serial_read(buffer, PACKET_SIZE);
}

static inline int usb_send (void *buffer, const size_t size)
{
	return RawHID.send(buffer, size);
	
	//return usb_serial_write(buffer, size);
}

static uint32_t calcWriteCrc (rawhid_header_t *desc)
{
	uint32_t crc = desc->op ^ desc->flags;

	for (int i = 0; i < 8; i++)
		crc ^= desc->u.crc.val[i];
		
	return crc;
}

static uint32_t decodeOp (rawhid_header_t *desc)
{
	uint32_t crc = calcWriteCrc(desc);
	if (crc != desc->crc){
		//printf("header crc mismatch\r\n");
		return 0;
	}
	return desc->op;
}

static uint32_t decodeHeader (rawhid_header_t *header, uint16_t *x1, uint16_t *y1, uint16_t *x2, uint16_t *y2, uint32_t *len)
{
	if ((header->u.write.x2 <= header->u.write.x1) || (header->u.write.y2 <= header->u.write.y1)){
		//printf("header area invalid\r\n");
		return 0;
	}

	if (((header->u.write.x2 - header->u.write.x1)+1) * ((header->u.write.y2 - header->u.write.y1)+1) * sizeof(uint16_t) != header->u.write.len){
		//printf("header size mismatch\r\n");
		return 0;
	}
	
	if (x1) *x1  = header->u.write.x1;
	if (y1) *y1  = header->u.write.y1;
	if (x2) *x2  = header->u.write.x2;
	if (y2) *y2  = header->u.write.y2;
	if (len)*len = header->u.write.len;

	return 1;
}

#if USE_STARTUP_IMAGE
static void setStartupImage ()
{
	// sanity check
	if (sizeof(frame256x142) > (TFT_WIDTH * TFT_HEIGHT * 2))
		return;
	
	uint16_t x1 = (TFT_WIDTH - 256) / 2;
	uint16_t y1 = (TFT_HEIGHT - 142) / 2;
	uint16_t x2 = (TFT_WIDTH - x1) - 1;
	uint16_t y2 = (TFT_HEIGHT - y1) - 1;
	tft_update_array((uint16_t*)frame256x142, x1, y1, x2, y2);
}
#endif

void setup ()
{
	//Serial.begin(9600);
	//while (!Serial);
	//printf("RawHID\r\n");
	//printf(CFG_STRING "\r\n");

	tft_init();
	tft_clear(0x0000);		// some displays require two initial clears
	tft_clear(0x0000);		// first is somethings corrupted

#if USE_STARTUP_IMAGE
	setStartupImage();
	setStartupImage();
#endif
}

static void opSetWriteCfg (rawhid_header_t *desc)
{
	if (desc->flags|RAWHID_OP_FLAG_WINDOW){
		config.tiles.window.x1 = desc->u.config.tiles.window.x1;
		config.tiles.window.y1 = desc->u.config.tiles.window.y1;
		config.tiles.window.x2 = desc->u.config.tiles.window.x2;
		config.tiles.window.y2 = desc->u.config.tiles.window.y2;
	}
	
	config.tiles.across = desc->u.config.tiles.across;
	config.tiles.down = desc->u.config.tiles.down;
	config.tiles.width = desc->u.config.tiles.width;
	config.tiles.height = desc->u.config.tiles.height;
}

int recvArea (recvDataCtx_t *dataCtx, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint8_t *dbuffer = (uint8_t*)tft_getBuffer();
	const int pitch = ((x2 - x1) + 1) + sizeof(uint16_t);
	const int height = (y2 - y1) + 1;
	
	for (int y = 0; y < height; y++){
		uint8_t *pixels = &dbuffer[y*pitch];
		//pixels += (x1*sizeof(uint16_t));
			
		for (int x = x1; x <= x2; x++){
			if (dataCtx->inCt >= PACKET_SIZE){
				dataCtx->inCt = 0;
				if (usb_recv(dataCtx->readIn) != PACKET_SIZE){
					return 0;
				}
			}
			*(pixels)++ = dataCtx->readIn[dataCtx->inCt++];
			*(pixels)++ = dataCtx->readIn[dataCtx->inCt++];
		}
	}
	return 1;
}

static inline uint16_t tileToX (const uint8_t tileX)
{
	return tileX * (TFT_WIDTH/config.tiles.across);
}

static inline uint16_t tileToY (const uint8_t tileY)
{
	return tileY * (TFT_HEIGHT/config.tiles.down);
}

void opWriteTiles (rawhid_header_t *indesc)
{
	rawhid_header_t desc;
	memcpy(&desc, indesc, sizeof(desc));
	
	const int updateDisplay = desc.flags&RAWHID_OP_FLAG_UPDATE;
	const uint16_t x1 = tileToX(config.tiles.window.x1);
	const uint16_t y1 = tileToY(config.tiles.window.y1);
	const uint16_t x2 = tileToX(config.tiles.window.x2);
	const uint16_t y2 = tileToY(config.tiles.window.y2);
	const uint16_t width = config.tiles.width;
	const uint16_t height = config.tiles.height; 
		
	uint16_t x = tileToX(desc.u.tiles.x);
	uint16_t y = tileToY(desc.u.tiles.y);
	int tTiles = desc.u.tiles.total;
	
	recvDataCtx_t dataCtx;
	dataCtx.readIn = (uint8_t*)recvBuffer;
	dataCtx.inCt = PACKET_SIZE+1;

	
	while (tTiles-- > 0){
		recvArea(&dataCtx, x, y, x+width-1, y+height-1);
		
		x += width;
		if (x > x2){
			x = x1;
			y += height;
			if (y > y2){
				y = y1;
				x = x1;
			}
		}
	};
	
	if (updateDisplay)
		tft_update();
}

static void opSendDeviceCfg (rawhid_header_t *desc)
{
	memset(desc, 0, sizeof(*desc));
	
	desc->op = RAWHID_OP_GETCFG;
	desc->u.cfg.supports = 0;			// todo
	desc->u.cfg.bpp = RAWHID_BPP_16;
	desc->u.cfg.width = TFT_WIDTH;
	desc->u.cfg.height = TFT_HEIGHT;
	desc->u.cfg.pitch = desc->u.cfg.width*sizeof(uint16_t);

#ifdef COL_CLAMP_MAX
	desc->flags |= RAWHID_OP_FLAG_CCLAMP;
	desc->u.cfg.rgbMin = COL_CLAMP_MIN;
	desc->u.cfg.rgbMax = COL_CLAMP_MAX;
#else
	desc->u.cfg.rgbMin = 0;
	desc->u.cfg.rgbMax = 0;
#endif


#if USE_STRIP_RENDERER
	desc->flags |= RAWHID_OP_FLAG_SRENDR;
	desc->u.cfg.stripHeight = STRIP_RENDERER_HEIGHT;
#endif

	memset(desc->u.cfg.string, 32, sizeof(desc->u.cfg.string));
	strncpy((char*)desc->u.cfg.string, CFG_STRING, sizeof(desc->u.cfg.string));

	desc->crc = calcWriteCrc(desc);
	usb_send(desc, sizeof(*desc));
}

static void opGfx (rawhid_header_t *desc)
{
	const int op = desc->u.gfxop.op;
	if (op == RAWHID_GFX_CLEAR){
		tft_clear(desc->u.gfxop.var16[0]);
		if (desc->flags&RAWHID_OP_FLAG_UPDATE)
			tft_update();

	}else if (op == RAWHID_GFX_SCROLL){
		// todo 

	}else if (op == RAWHID_GFX_BACKLIGHT){
		tft_backlight(desc->u.gfxop.var8[0]);
		
	}else if (op == RAWHID_GFX_ROTATE){
		tft_rotate(desc->u.gfxop.var8[0]);
	}
}

#if USE_STRIP_RENDERER
void opRecvImage (rawhid_header_t *header)
{
	const int updateDisplay = header->flags&RAWHID_OP_FLAG_UPDATE;
	
	recvDataCtx_t dataCtx;
	dataCtx.readIn = (uint8_t*)recvBuffer;
	dataCtx.inCt = PACKET_SIZE+1;
	
	int trows = STRIP_RENDERER_HEIGHT;
	const int iter = TFT_HEIGHT / STRIP_RENDERER_HEIGHT;
	
	for (int y = 0; y < iter; y++){
		if (!recvArea(&dataCtx, 0, (y*trows), TFT_WIDTH-1, (y*trows)+trows-1))
			return;
		if (updateDisplay)
			tft_update_area(0, (y*trows), TFT_WIDTH-1, (y*trows)+trows-1);
	}


	const int remaining = TFT_HEIGHT % STRIP_RENDERER_HEIGHT;
	if (remaining){
		if (!recvArea(&dataCtx, 0, (iter*trows), TFT_WIDTH-1, (iter*trows)+remaining-1))
			return;
		if (updateDisplay)
			tft_update_area(0, (iter*trows), TFT_WIDTH-1, (iter*trows)+remaining-1);
	}


	//arm_dcache_flush(tft_getBuffer(), TFT_WIDTH * STRIP_RENDERER_HEIGHT * 2);
}
#else	
void opRecvImage (rawhid_header_t *header)
{
	
	//elapsedMillis timeMs = 0;
	
	const int updateDisplay = header->flags&RAWHID_OP_FLAG_UPDATE;
	uint32_t len = 0;
	
	if (decodeHeader(header, NULL, NULL, NULL, NULL, &len)){
		uint8_t *dbuffer = (uint8_t*)tft_getBuffer();
		int bytesIn = 0;
				
		int totalReads = (len / PACKET_SIZE);
		if (!totalReads) totalReads = 1;
		
		while (totalReads--){
			bytesIn = usb_recv(dbuffer);
			if (bytesIn != PACKET_SIZE) break;
			dbuffer += PACKET_SIZE;
		}

		int finalRead = (len%PACKET_SIZE);
		if (finalRead){
			bytesIn = usb_recv(recvBuffer);
			if (bytesIn > 0)
				memcpy(dbuffer, recvBuffer, finalRead);
		}
	}
	
	//uint32_t t0 = timeMs;
	//printf("time to recv %i\n", (int)t0);
	
	//timeMs = 0;
	if (updateDisplay)
		tft_update();
	
	//t0 = timeMs;
	//printf("time to display %i\n", (int)t0);
}
#endif

void opRecvImageArea (rawhid_header_t *header)
{
	const int updateDisplay = header->flags&RAWHID_OP_FLAG_UPDATE;

	uint16_t x1 = 0;
	uint16_t y1 = 0;
	uint16_t x2 = 0;
	uint16_t y2 = 0;

	recvDataCtx_t dataCtx;
	dataCtx.readIn = (uint8_t*)recvBuffer;
	dataCtx.inCt = PACKET_SIZE+1;


// todo: do a bound check
	if (decodeHeader(header, &x1, &y1, &x2, &y2, NULL))
		recvArea(&dataCtx, x1, y1, x2, y2);

	if (updateDisplay)
		tft_update_area(x1, y1, x2, y2);
}

void loop ()
{
	rawhid_header_t *desc = (rawhid_header_t*)recvBuffer;
	memset(desc, 0, sizeof(*desc));
		
	while(1){
	desc->op = 0;
	
	int bytesIn = usb_recv(desc);
	if (bytesIn != PACKET_SIZE) return;

	const int op = decodeOp(desc);
	//printf("op: %i\n", (op));
	
	if (op == RAWHID_OP_WRITEIMAGE){
		opRecvImage(desc);

	}else if (op == RAWHID_OP_WRITEAREA){
		opRecvImageArea(desc);
		
	}else if (op == RAWHID_OP_GFXOP){
		opGfx(desc);
		
	}else if (op == RAWHID_OP_GETCFG){
		opSendDeviceCfg(desc);
		
	}else if (op == RAWHID_OP_SETCFG){
		opSetWriteCfg(desc);

	}else if (op == RAWHID_OP_WRITETILE){
		opWriteTiles(desc);
	}
	
	}
}
