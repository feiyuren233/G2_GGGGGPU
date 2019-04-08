/*
 * vga_graphics.c
 *
 *  Created on: 2019?2?27?
 *      Author: liuli
 */

#include "vga_graphics.h"
#include "xtft.h"
#include "xparameters.h"

static XTft inst_obj;
static XTft *inst = NULL;

u32 color_asPixelVal(Color c) {
	u8 R = c.R & 0b00001111;
	u8 G = c.G & 0b00001111;
	u8 B = c.B & 0b00001111;

	return R << 20 | G << 12 | B << 4;
}

Color color_fromPixelVal(u32 v) {
	Color c;
	c.R = (v >> 20) & 0b00001111;
	c.G = (v >> 12) & 0b00001111;
	c.B = (v >> 4) & 0b00001111;

	return c;
}

void initGraphics() {
	inst = &inst_obj;

	XTft_Config conf;
	conf.AddrWidth = XPAR_TFT_0_M_AXI_ADDR_WIDTH;
	conf.BaseAddress = XPAR_TFT_0_BASEADDR;
	conf.DeviceId = XPAR_TFT_0_DEVICE_ID;
	conf.VideoMemBaseAddr = VIDEO_MEM_BASE_ADDR;

	XTft_CfgInitialize(inst, &conf, conf.BaseAddress);
	XTft_SetFrameBaseAddr(inst, conf.VideoMemBaseAddr);
	XTft_EnableDisplay(inst);
}

void drawFilledRect(u32 col, u32 row, u32 width, u32 height, Color c) {
	// XTft_FillScreen(inst, col, row, col + width - 1, row + height - 1, color_asPixelVal(c));

	u32 pixel = color_asPixelVal(c);
	u32 col_begin_times_4 = col << 2;
	u32 row_tracker = row << 12;
	u32 i=0, j=0;

	volatile u32 *cur;
	for(i=0; i<height; i++, row_tracker+=4096){
		cur = (volatile u32 *) (row_tracker + col_begin_times_4 + VIDEO_MEM_BASE_ADDR);
		for(j=0; j<width; j++){
			*cur++ = pixel;
		}
	}
}

void drawFilledCircle(u32 cent_col, u32 cent_row, u32 radius, Color c) {
	u32 row, col;
	u32 pixel = color_asPixelVal(c);
	u32 col_begin = cent_col - radius;
	u32 col_end = cent_col + radius;
	u32 row_begin = cent_row - radius;
	u32 row_end = cent_row + radius;
	u32 delta_col;
	u32 delta_row;
	u32 radius_squared = radius * radius;
	for (col = col_begin; col < col_end; col++) {
		for (row = row_begin; row < row_end; row++) {
			delta_col = cent_col - col;
			delta_row = cent_row - row;
			if (delta_col * delta_col + delta_row * delta_row <= radius_squared) {
				writePixel(col, row, pixel);
			}
		}
	}
}

void clearScreen() {
	XTft_ClearScreen(inst);
}

void writePixel(u32 col, u32 row, u32 pixel) {
	// XTft_SetPixel(inst, col, row, color_asPixelVal(c));
	Xil_Out32(VIDEO_MEM_BASE_ADDR + (((row << 10) + col) << 2), pixel);
}

void writeImage(u8 *src, u32 width, u32 height) {
	// src size >= width * height * 4
	for(int i = 0; i < height; i++) {
		memcpy((VIDEO_MEM_BASE_ADDR + ((i<<10)<<2)), (src + ((i*width)<<2)), (width <<2));
	}
}
