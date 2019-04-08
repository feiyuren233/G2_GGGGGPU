/*
 * vga_graphics.h
 *
 *  Created on: 2019?2?27?
 *      Author: liuli
 */

#ifndef SRC_VGA_GRAPHICS_H_
#define SRC_VGA_GRAPHICS_H_

#include "xtft.h"

#define VIDEO_MEM_BASE_ADDR 0x86000000

/* a struct that holds a color. The R, G, B value should not be larger than 0xF (16) */
typedef struct color_struct {
	u8 R;
	u8 G;
	u8 B;
} Color;

u32 color_asPixelVal(Color c);
Color color_fromPixelVal(u32 v);


void initGraphics();

void drawFilledRect(u32 col, u32 row, u32 width, u32 height, Color c);
void drawFilledCircle(u32 cent_col, u32 cent_row, u32 radius, Color c);
void clearScreen();
void writePixel(u32 col, u32 row, u32 pixel);
void writeImage(u8 *src, u32 width, u32 height);

#endif /* SRC_VGA_GRAPHICS_H_ */
