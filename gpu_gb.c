#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "memory.h"
#include "gpu_gb.h"

static const int buffWidth = 160;
static const int buffHeight = 144;

static int gpuTicks = 0;
static uint32_t* lcdFrameBuffer;
static uint32_t createColor(int r, int g, int b);
static void setPixel(unsigned int x, unsigned int y, const uint32_t color);
static uint32_t getPixel(unsigned int x, unsigned int y);

static void renderScanline();

static uint32_t color[4];

void gpuInit(uint32_t* frameBuffer)
{
	lcdFrameBuffer = frameBuffer;
	// clear the frame buffer
	memset(lcdFrameBuffer, 0, sizeof(uint32_t) * buffWidth * buffHeight);
	color[0] = createColor(255, 255, 255);
	color[1] = createColor(192, 192, 192);
	color[2] = createColor(96, 96, 96);
	color[3] = 0;
}

void gpuStep(int cycles, SDL_Surface* screen)
{
	gpuTicks += cycles;

	// see what mode we are in
	switch(register_STAT & 0x3)
	{
		// H-Blank
		case 0:
			if(gpuTicks >= 204)
			{
				gpuTicks = 0;
				register_LY++;
			//	compareLYtoLYC();

				if(register_LY >= 144)
				{
					// switch to V-Blank
					register_STAT = (register_STAT & 0xFC) | 0x1;
					//request V-Blank interrupt
					register_IF |= 0x1;
					
					SDL_Flip(screen);

					// Send LCD interrupt, if V-Blank interrupt in the STAT register is set
					if(register_STAT & 0x10)
					{
						register_IF |= 0x2;
					}
				}
				else
				{
					// search OAM-RAM
					register_STAT = (register_STAT & 0xFC) | 0x2;

					// Send LCD interrupt if OAM interrupt in the STAT register is set
					if(register_STAT & 0x20)
					{
						register_IF |= 0x2;
					}
				}
			}
			break;
		// V-Blank
		case 1:
			if(gpuTicks >= 456)
			{
				gpuTicks = 0;
				register_LY++;
			//	compareLYtoLYC();
				
				if(register_LY > 153)
				{
					// restart drawing scanlines
					register_LY = 0;
				//  compareLYtoLYC();

					// switch to searching OAM mode
					register_STAT = (register_STAT & 0xFC) | 0x2;

					// Send LCD interrupt if OAM Interrupt in the STAT register is set
					if(register_STAT & 0x10)
					{
						register_IF |= 0x2;
					}
				}
			}
			break;
		// Searching OAM-RAM
		case 2:                    
			if(gpuTicks >= 80)
			{
				// Switch to mode 3, accessing VRAM
				gpuTicks = 0;
				register_STAT = (register_STAT & 0xFC) | 0x3;
			}
			break;
		// Transfer data to LCD driver
		case 3:
			if(gpuTicks >= 172)
			{
				gpuTicks = 0;
				// Switch to H-Blank
				register_STAT = (register_STAT & 0xFC);

				// Send LCD interrupt if H-Blank interrupt in STAT register is set
				if(register_STAT & 0x4)
				{
					register_IF |= 0x2;
				}

				// Draw the scanline
				renderScanline();	
			}
			break;
	}
}

static void drawBackground()
{
	// base addresses
	int tileMapAddress = (register_LCDC & 0x08) ? 0x1C00 : 0x1800;
	int dataAddress = (register_LCDC & 0x10) ? 0x0000 : 0x0800;
	
	// add the y-offset to the tile map address.
	int yOff = ((register_SCY + register_LY) & 0xFFFF) / 8 * 32;
	// add the x-offset to the tile map address.
	int xOff = (register_SCX / 8) & 0xFF;
	tileMapAddress += xOff + yOff;
	// this is the number of tile that will be displayed
	int tileNum = videoRam[tileMapAddress] & 0xFFFF;
	// tile set #0, this means the tile number is signed
	if(register_LCDC & 0x08 && tileNum > 127)
		tileNum -= 256;
	
	// get the address of the tile to display
	int tileDataAddress = dataAddress + (tileNum << 4);
	// which row of the tile are we displaying?
	tileDataAddress += ((register_SCY + register_LY) & 7) << 1;
	// each row in the tile is defined using two bytes.
	int8_t tileLow = videoRam[tileDataAddress++];
	int8_t tileHigh = videoRam[tileDataAddress];
	// which colunm of the tile are we displaying?
	int tileX = register_SCX & 7;

	int x;
	for(x = 0; x < 160; x++)
	{
		// get the color
		int colorIndex = (tileLow >> (7-tileX) & 1) | ((tileHigh >> (7-tileX) & 1) << 1);
		setPixel(x, register_LY, color[register_BGP[colorIndex]]);
		
		tileX++;
		// this tile is done, move on to the next one
		if(tileX == 8)
		{
			tileX = 0;
			tileMapAddress++;
			tileNum = videoRam[tileMapAddress] & 0xFFFF;
			if(register_LCDC & 0x40 && tileNum > 127)
				tileNum -= 256;
			tileDataAddress = dataAddress + (tileNum << 4);
			tileDataAddress += ((register_SCY + register_LY) & 7) << 1;
			tileLow = videoRam[tileDataAddress++];
			tileHigh = videoRam[tileDataAddress];
		}
	}
}

static void drawSprites()
{
	int i;
	for(i = 0; i < 40; i++)
	{
		int spriteY = spriteOAM[i * 4] - 16;
		int spriteX = spriteOAM[(i + 1) * 4] - 8;
		int spriteTileNum = spriteOAM[(i + 2) * 4];
		int spriteAtt = spriteOAM[(i + 3) * 4];
		
		// check if the sprite lies on the scanline
		if(spriteY <= register_LY && spriteY + 8 > register_LY)
		{
			// get the palette to be used for this sprite
			uint8_t* palette = spriteAtt & 0x10 ? register_OBP1: register_OBP0;
			
			int tileRow;
			// Check if Y has to be flipped, if so use the opposite side of the tile
			if(spriteAtt & 0x40)
				tileRow = (7 - (register_LY - spriteY)) << 1;
			else
				tileRow = (register_LY - spriteY) << 1;

			// get the part of the tile we are displaying
			int tileLow = videoRam[(spriteTileNum << 4) + tileRow];
			int tileHigh = videoRam[(spriteTileNum << 4) + tileRow + 1];

			int x;
			for(x = 0; x < 8; x++)
			{
				if(spriteX + x >= 0 && spriteX + x < 160)
				{
					int tileX = spriteAtt & 0x20 ? x : 7 - x; 
					// get the color
					int colorIndex = (tileLow >> tileX & 1) | ((tileHigh >> tileX & 1) << 1);
					uint32_t spriteColor = color[palette[colorIndex]];
					if(spriteColor != 0 && (spriteAtt & 0x80 || getPixel(spriteX + x, register_LY) == 0))
					{
						setPixel(spriteX + x, register_LY, spriteColor);
					}
				}
			}
		}
	}
}

static void renderScanline()
{
	if(register_LCDC & 0x1)
		drawBackground();
	if(register_LCDC & 0x2)
		drawSprites();
}

static uint32_t createColor(int r, int g, int b)
{
	// top 8 bits are the alpha value, set the color to be completely opaque
	return (255 << 24) | (r << 16) | (g << 8) | b;
}

void setPixel(unsigned int x, unsigned int y, const uint32_t color)
{
	lcdFrameBuffer[y * buffWidth + x] = color;
}

static uint32_t getPixel(unsigned int x, unsigned int y)
{
	return lcdFrameBuffer[y * buffWidth + x];
}
