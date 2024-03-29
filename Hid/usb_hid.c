/* Teensyduino Core Library
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2017 PJRC.COM, LLC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * 1. The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * 2. If the Software is incorporated into a build system that allows
 * selection among a list of target devices, then similar target
 * devices manufactured by PJRC.COM must be included in the list of
 * target devices and selectable in the same manner.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "usb_dev.h"


//#ifdef RAWHID_INTERFACE // defined by usb_dev.h -> usb_desc.h
//extern volatile uint8_t usb_high_speed;




#define TX_NUM   TRANSMIT_BUFFERS
static transfer_t tx_transfer[TX_NUM] __attribute__ ((used, aligned(32)));
static uint8_t txbuffer[RAWHID_TX_SIZE_480 * TX_NUM];

static uint8_t tx_head = 0;
static uint16_t tx_packet_size = RAWHID_RX_SIZE_480;


#define RX_NUM  RECIEVE_BUFFERS
static transfer_t rx_transfer[RX_NUM] __attribute__ ((used, aligned(32)));
static uint8_t rx_buffer[RAWHID_RX_SIZE_480 * RX_NUM] __attribute__ ((aligned(32)));
static uint8_t rx_list[RX_NUM + 1];

static volatile uint8_t rx_head = 0;
static volatile uint8_t rx_tail = 0;
static volatile uint32_t rx_available;
static uint16_t rx_packet_size = RAWHID_RX_SIZE_480;

static void rx_queue_transfer (int i);
static void rx_event (transfer_t *t);

extern volatile uint8_t usb_configuration;




void usb_rawhid_configure (void)
{
	
	tx_head = 0;
	rx_head = 0;
	rx_tail = 0;
	
	memset(tx_transfer, 0, sizeof(tx_transfer));
	memset(rx_transfer, 0, sizeof(rx_transfer));
	
	//if (usb_high_speed){
		tx_packet_size = RAWHID_TX_SIZE_480;
		rx_packet_size = RAWHID_RX_SIZE_480;
	//}else{
	//	tx_packet_size = RAWHID_TX_SIZE_12;
	//	rx_packet_size = RAWHID_RX_SIZE_12;
	//}

	usb_config_tx(RAWHID_TX_ENDPOINT, tx_packet_size, 0, NULL);
	usb_config_rx(RAWHID_RX_ENDPOINT, rx_packet_size, 0, rx_event);

	for (int i = 0; i < RX_NUM; i++)
		rx_queue_transfer(i);
}

static void rx_queue_transfer (int i)
{
	NVIC_DISABLE_IRQ(IRQ_USB1);
	void *buffer = rx_buffer + i * (RAWHID_RX_SIZE_480);
	//arm_dcache_flush(buffer, rx_packet_size);

	usb_prepare_transfer(rx_transfer + i, buffer, rx_packet_size, i);
	usb_receive(RAWHID_RX_ENDPOINT, rx_transfer + i);
	NVIC_ENABLE_IRQ(IRQ_USB1);
}

static void rx_event (transfer_t *t)
{
	int i = t->callback_param;
	//printf("rx event i=%d\n", i); 	// received a packet with data

	uint32_t head = rx_head;
	if (++head > RX_NUM) head = 0;
	rx_list[head] = i;
	rx_head = head;
}

int usb_rawhid_recv (void *buffer, uint32_t timeout)
{
	if (!usb_configuration)
		return -1; // usb not enumerated by host
	
	uint32_t wait_begin_at = systick_millis_count;
	uint32_t tail = rx_tail;
	
	while (1) {
		if (!usb_configuration)				// usb not enumerated by host
			return -1;
		if (tail != rx_head)
			break;
		if ((systick_millis_count - wait_begin_at > timeout) || !timeout)
			return 0;

		yield();
		yield();
	}

	if (++tail > RX_NUM) tail = 0;
	uint32_t i = rx_list[tail];
	rx_tail = tail;

	memcpy(buffer,  rx_buffer + i * RAWHID_RX_SIZE_480, rx_packet_size);
	rx_queue_transfer(i);

	return rx_packet_size;
}

int usb_rawhid_recv2 (void **buffer, uint32_t timeout)
{
	if (!usb_configuration)
		return -1; // usb not enumerated by host
	
	uint32_t wait_begin_at = systick_millis_count;
	uint32_t tail = rx_tail;
	
	while (1) {
		if (!usb_configuration)			 // usb not enumerated by host
			return -1;
		if (tail != rx_head)
			break;
		if ((systick_millis_count - wait_begin_at > timeout) || !timeout)
			return 0;

		yield();
		yield();
	}

	if (++tail > RX_NUM) tail = 0;
	uint32_t i = rx_list[tail];
	rx_tail = tail;

	*buffer = (void*)(rx_buffer + (RAWHID_RX_SIZE_480 * i));
	
	rx_queue_transfer(i);
	return rx_packet_size;
}

int usb_rawhid_send (const void *buffer, uint32_t timeout)
{
	transfer_t *xfer = tx_transfer + tx_head;
	uint32_t wait_begin_at = systick_millis_count;

	while (1) {
		if (!usb_configuration)							// usb not enumerated by host
			return -1; 
		if (!(usb_transfer_status(xfer)&0x80))			// transfer descriptor ready
			break;
		if (systick_millis_count - wait_begin_at > timeout)
			return 0;
		
		yield();
	}

	uint8_t *txdata = txbuffer + (tx_head * RAWHID_TX_SIZE_480);
	memcpy(txdata, buffer, tx_packet_size);
	//arm_dcache_flush_delete(txdata, tx_packet_size );
	
	usb_prepare_transfer(xfer, txdata, tx_packet_size, 0);
	usb_transmit(RAWHID_TX_ENDPOINT, xfer);
	
	if (++tx_head >= TX_NUM) tx_head = 0;
	return tx_packet_size;
}

int usb_rawhid_available (void)
{
	if (!usb_configuration)
		return 0;
	if (rx_head != rx_tail)
		return rx_packet_size;

	return 0;
}

/*
int usb_rawhid_rxSize (void)
{
	return rx_packet_size;
}

int usb_rawhid_txSize (void)
{
	return tx_packet_size;	
}*/

//#endif // RAWHID_INTERFACE
