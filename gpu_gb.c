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

void gpuStep(int cycles)
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

				if(register_LY == 144)
				{
					// switch to V-Blank
					register_STAT = (register_STAT & 0xFC) | 0x1;
					//request V-Blank interrupt
					register_IF |= 0x1;

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
				//renderScanline();	
			}
			break;
	}
}

static void renderScanline()
{
	int tileMapAddress = (register_LCDC & 0x08) ? 0x1C00 : 0x1800;
	int tileDataAddress = (register_LCDC & 0x40) ? 0x0800 : 0x0000;

	int y = (register_LY + register_SCY);
	int x = register_SCX;
	
	int tileRow = y & 0x7;
	int tileNum = (y / 8) + (x / 8);

	uint8_t tile = videoRam[tileMapAddress + tileNum * 5];
	int i;
	for(i = 0; i < 32; i++)
	{
		if(register_LCDC & 0x08 && tile < 128)
		{
			tile += 256;
		}
		uint8_t lowByte = videoRam[tileDataAddress + tile++];
		uint8_t highByte = videoRam[tileDataAddress + tile++];
		int j;
		for(j = 0; j < 8; j++)
		{
			setPixel(x + j, y, color[register_BGP[(highByte & 0x1) << 1 | (lowByte & 0x1)]]);
			lowByte >>= 1;
			highByte >>= 1;
		}
		x += 8;
	}
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
