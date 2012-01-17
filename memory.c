#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "memory.h"
#include "mbc.h"

#define UNDEF_MEM 0xFF       // Is this right?

uint8_t (*cartidgeRead)(Cartidge* game, int memAddr);
void (*cartidgeWrite)(Cartidge* gameData, int memAddr, int8_t writeData);
void getReadWrite(Cartidge* game);
void DMA_OAM_Transfer(int8_t writeData);
Cartidge* cartidge;

int bootEnabled = 1;
 
uint8_t bootStrap[256] = { 0x31,0xFE,0xFF,0xAF,0x21,0xFF,0x9F,0x32,0xCB,0x7C,0x20,0xFB,0x21,0x26,0xFF,0x0E,
                           0x11,0x3E,0x80,0x32,0xE2,0x0C,0x3E,0xF3,0xE2,0x32,0x3E,0x77,0x77,0x3E,0xFC,0xE0,
                           0x47,0x11,0x04,0x01,0x21,0x10,0x80,0x1A,0xCD,0x95,0x00,0xCD,0x96,0x00,0x13,0x7B,
                           0xFE,0x34,0x20,0xF3,0x11,0xD8,0x00,0x06,0x08,0x1A,0x13,0x22,0x23,0x05,0x20,0xF9,
                           0x3E,0x19,0xEA,0x10,0x99,0x21,0x2F,0x99,0x0E,0x0C,0x3D,0x28,0x08,0x32,0x0D,0x20,
                           0xF9,0x2E,0x0F,0x18,0xF3,0x67,0x3E,0x64,0x57,0xE0,0x42,0x3E,0x91,0xE0,0x40,0x04,
                           0x1E,0x02,0x0E,0x0C,0xF0,0x44,0xFE,0x90,0x20,0xFA,0x0D,0x20,0xF7,0x1D,0x20,0xF2,
                           0x0E,0x13,0x24,0x7C,0x1E,0x83,0xFE,0x62,0x28,0x06,0x1E,0xC1,0xFE,0x64,0x20,0x06,
                           0x7B,0xE2,0x0C,0x3E,0x87,0xE2,0xF0,0x42,0x90,0xE0,0x42,0x15,0x20,0xD2,0x05,0x20,
                           0x4F,0x16,0x20,0x18,0xCB,0x4F,0x06,0x04,0xC5,0xCB,0x11,0x17,0xC1,0xCB,0x11,0x17,
                           0x05,0x20,0xF5,0x22,0x23,0x22,0x23,0xC9,0xCE,0xED,0x66,0x66,0xCC,0x0D,0x00,0x0B,
                           0x03,0x73,0x00,0x83,0x00,0x0C,0x00,0x0D,0x00,0x08,0x11,0x1F,0x88,0x89,0x00,0x0E,
                           0xDC,0xCC,0x6E,0xE6,0xDD,0xDD,0xD9,0x99,0xBB,0xBB,0x67,0x63,0x6E,0x0E,0xEC,0xCC,
                           0xDD,0xDC,0x99,0x9F,0xBB,0xB9,0x33,0x3E,0x3C,0x42,0xB9,0xA5,0xB9,0xA5,0x42,0x3C,
                           0x21,0x04,0x01,0x11,0xA8,0x00,0x1A,0x13,0xBE,0x20,0xFE,0x23,0x7D,0xFE,0x34,0x20,
                           0xF5,0x06,0x19,0x78,0x86,0x23,0x05,0x20,0xFB,0x86,0x20,0xFE,0x3E,0x01,0xE0,0x50};

/**
 * Initializes memory to its default value
 * \param game cartidge being loaded into the memory
 * \return GAME_ERROR any error that may occur; NO_ERR if no error occurs.
 */
GAME_ERROR initMemory(Cartidge* game)
{
	getReadWrite(game);
	cartidge = game;

	// initialize register values
	ioRegisters[0x05] = 0x00; // TIMA
	ioRegisters[0x06] = 0x00; // TMA
	ioRegisters[0x07] = 0x00; // TAC
	ioRegisters[0x10] = 0x80; // NR10
	ioRegisters[0x11] = 0xBF; // NR11
	ioRegisters[0x12] = 0xF3; // NR12
	ioRegisters[0x14] = 0xBF; // NR14
	ioRegisters[0x16] = 0x3F; // NR21
	ioRegisters[0x17] = 0x00; // NR22
	ioRegisters[0x19] = 0xBF; // NR24
	ioRegisters[0x1A] = 0x7F; // NR30
	ioRegisters[0x1B] = 0xFF; // NR31
	ioRegisters[0x1C] = 0x9F; // NR32
	ioRegisters[0x1E] = 0xBF; // NR33
	ioRegisters[0x20] = 0xFF; // NR41
	ioRegisters[0x21] = 0x00; // NR42
	ioRegisters[0x22] = 0x00; // NR43
	ioRegisters[0x23] = 0xBF; // NR30
	ioRegisters[0x24] = 0x77; // NR50
	ioRegisters[0x25] = 0xF3; // NR51
	if(game->SGB)             // NR52
		ioRegisters[0x26] = 0xF0; 
	else
		ioRegisters[0x26] = 0xF1; 
	ioRegisters[0x40] = 0x91; // LCDC
	ioRegisters[0x42] = 0x00; // SCY
	ioRegisters[0x43] = 0x00; // SCX 
	ioRegisters[0x45] = 0x00; // LYC
	ioRegisters[0x47] = 0xFC; // BGP
	ioRegisters[0x48] = 0xFF; // OBP0
	ioRegisters[0x49] = 0xFF; // OBP1
	ioRegisters[0x4A] = 0x00; // WY
	ioRegisters[0x4B] = 0x00; // WX
	ioRegisters[0x50] = 0x01; // BootRom flag
	register_IE = 1;          // IE	
	intMasEnable = 1;         // Interrupt Master Enable

	return NO_ERR;
}

/**
 * Read from memory.
 * \param memAddr memory address in which to read from.
 */
uint8_t memoryRead(uint16_t memAddr)
{
	if(memAddr <= 0xFF && bootEnabled)                 // Boot Rom (if enabled)
		return bootStrap[memAddr];
	if(memAddr >= 0x8000 && memAddr <= 0x9000)        // Video RAM (switchable in CGB, add support later?)
	{
		int lcdMode = register_STAT & 0x3;
		if(lcdMode != 3)                              // LCD controller is reading from VRAM when the lcdmode   	
			return videoRam[memAddr - 0x8000];        // equals 3, so the CPU cannot access VRAM
		else
			return UNDEF_MEM;
	}
	else if (memAddr >= 0xC000 && memAddr <= 0xCFFF)  // Work RAM Bank 0 
		return workRam0[memAddr - 0xC000];
	else if (memAddr >= 0xD000 && memAddr <= 0xDFFF)  // Work RAM Bank 1 (switchable in CGB, add support later?)
		return workRam1[memAddr - 0xD000];            
	else if (memAddr >= 0xE000 && memAddr <= 0xFDFF)  // Echo to 0xC000 - 0xDDFF
	{
		memAddr -= 0x2000;
		if (memAddr >= 0xC000 && memAddr <= 0xCFFF)
			return workRam0[memAddr - 0xC000];
		else if (memAddr >= 0xD000 && memAddr <= 0xDFFF)
			return workRam1[memAddr - 0xD000];
	}
	else if(memAddr >= 0xFE00 && memAddr <= 0xFE9F)   // Sprite Attribute Table
	{
		int lcdMode = register_STAT & 0x3;
		if(lcdMode == 0 || lcdMode == 1)              // Can only access OAM during H-Blank and V-Blank
			return spriteOAM[memAddr - 0xFE00];
		else 
			return UNDEF_MEM;
	}
	else if(memAddr >= 0xFF00 && memAddr <= 0xFF7F)   // I/O ports
	{
		memAddr &= 0xFF;
		switch(memAddr)
		{
			case 0x40:
				return register_LCDC;
			case 0x41:
				return register_STAT;
			case 0x42:
				return register_SCY;
			case 0x43:
				return register_SCX;
			case 0x44:
				return register_LY;
			case 0x45:
				return register_LYC;
			case 0x47:
				return register_BGP[3] << 6 | register_BGP[2] << 4 | register_BGP[1] << 2 | register_BGP[0];
			case 0x48:
				return register_OBP0[3] << 6 | register_OBP0[2] << 4 | register_OBP0[1] << 2 | register_OBP0[0];
			case 0x49:
				return register_OBP1[3] << 6 | register_OBP1[2] << 4 | register_OBP1[1] << 2 | register_OBP1[0];
			case 0x4A:
				return register_WY;
			case 0x4B:
				return register_WX;
			default:
				return ioRegisters[memAddr];          // Basically stuff I havent done yet
		}
	}
	else if (memAddr >= 0xFF80 && memAddr <= 0xFFFE)  // High RAM
		return highRam[memAddr - 0xFF80];
	else if(memAddr == 0xFFFF)                        // Interrupt enabled register.
		return register_IE;
	else                                              // All other addresses 
		return cartidgeRead(cartidge, memAddr);       // are in the cartidge.
	return -1;
}

/**
 * Write to memory.
 */
void memoryWrite(uint16_t memAddr, uint8_t data)
{
	if(memAddr <= 0xFF && bootEnabled)                 // Boot Rom (if enabled)
		bootStrap[memAddr] = data;
	if(memAddr >= 0x8000 && memAddr <= 0xA000)        // Video RAM (switchable in CGB, add support later?)
	{
		int lcdMode = register_STAT & 0x3;
		//if(lcdMode != 3)                              // LCD controller is reading from VRAM when the lcdMode   	
			videoRam[memAddr - 0x8000] = data;        // equals 3, so the CPU cannot access VRAM
	}
	else if (memAddr >= 0xC000 && memAddr <= 0xCFFF)  // Work RAM Bank 0 
		workRam0[memAddr - 0xC000] = data;
	else if (memAddr >= 0xD000 && memAddr <= 0xDFFF)  // Work RAM Bank 1 (switchable in CGB, add support later?)
		workRam1[memAddr - 0xD000] = data;            
	else if (memAddr >= 0xE000 && memAddr <= 0xFDFF)  // Echo to 0xC000 - 0xDDFF
	{
		memAddr -= 0x2000;
		if (memAddr >= 0xC000 && memAddr <= 0xCFFF)
			workRam0[memAddr - 0xC000] = data;
		else if (memAddr >= 0xD000 && memAddr <= 0xDFFF)
			workRam1[memAddr - 0xD000] = data;
	}
	else if(memAddr >= 0xFE00 && memAddr <= 0xFE9F)   // Sprite Attribute Table
	{
		int lcdMode = register_STAT & 0x3;
		//if((lcdMode == 0) | (lcdMode == 1))           // Can only access OAM during H-Blank or V-Blank
			spriteOAM[memAddr - 0xFE00] = data;
	}
	else if(memAddr >= 0xFF00 && memAddr <= 0xFF7F)   // I/O ports
	{
		memAddr &= 0xFF;
		switch(memAddr)
		{
			case 0x40:
				register_LCDC = data;
				return;
			case 0x41:
				register_STAT = data & 0xFC;          // Bit 0 and 1 of the STAT register are read only
				return;
			case 0x42:
				register_SCY = data;
				return;		
			case 0x43:
				register_SCX = data;
				return;
			case 0x44:
				// register_LY is read only
				return;
			case 0x45:
				register_LYC = data;
				//compareLYCToLY();
				return;
			case 0x46:
				DMA_OAM_Transfer(data);
			case 0x47:
				register_BGP[0] = data & 0x3;
				register_BGP[1] = (data >> 2) & 0x3;
				register_BGP[2] = (data >> 4) & 0x3;
				register_BGP[3] = (data >> 6) & 0x3;
				return;
			case 0x48:
				register_OBP0[0] = data & 0x3;
				register_OBP0[1] = (data >> 2) & 0x3;
				register_OBP0[2] = (data >> 4) & 0x3;
				register_OBP0[3] = (data >> 6) & 0x3;
				return;
			case 0x49:
				register_OBP1[0] = data & 0x3;
				register_OBP1[1] = (data >> 2) & 0x3;
				register_OBP1[2] = (data >> 4) & 0x3;
				register_OBP1[3] = (data >> 6) & 0x3;
				return;
			case 0x4A:
				register_WY = data;
				return;
			case 0x4B:
				register_WX = data;
				return;
			case 0x50:
				bootEnabled = 0;
				printf("bootrom finished!\n");
				//exit(0);
			default:
				ioRegisters[memAddr] = data;          // Basically stuff I havent done yet
				return;
		}
	}
	else if (memAddr >= 0xFF80 && memAddr <= 0xFFFE)  // High RAM
		highRam[memAddr - 0xFF80] = data;
	else if(memAddr == 0xFFFF)                        // Interrupt enabled register.
		register_IE = data;
	else                                              // All other addresses 
		cartidgeWrite(cartidge, memAddr, data);       // are in the cartidge.
}

/**
 * Get the correct cartidge read and write functions based on the MBC model.
 */ 
void getReadWrite(Cartidge* game)
{
	// TODO add other models
	switch(game->romType)
	{
		case ROM:
			cartidgeRead = &ROM_memoryRead;
			cartidgeWrite = &ROM_memoryWrite;
			break;
		case MBC1:
			cartidgeRead = &MBC1_memoryRead;
			cartidgeWrite = &MBC1_memoryWrite;
			break;
		case MBC2:
			cartidgeRead = &MBC2_memoryRead;
			cartidgeWrite = &MBC2_memoryWrite;
			break;
		case MBC3:
			cartidgeRead = &MBC3_memoryRead;
			cartidgeWrite = &MBC3_memoryWrite;
			break;
		case MBC5:
			cartidgeRead = &MBC5_memoryRead;
			cartidgeWrite = &MBC5_memoryWrite;
			break;
		default:
			printf("unsupported ROM type:");
			printRomType(game->romVersion);
	}
}

void compareLYToLYC()
{
	if(register_LY == register_LYC)
	{
		// set the coincident bit in the STAT register
		register_STAT |= 4;
		
		// Check if the coincedence interrupt is enabled
		if(register_STAT & 0x40)
		{
			// set the STAT interrupt flag
			register_IF |= 2;
		}
	}
	else
	{
		// clear the coincident bit in the STAT register
		register_STAT &= 0xFB;
	}
}

void DMA_OAM_Transfer(int8_t writeData)
{
	writeData <<= 8;
	int i;
	// Transfer from ROM or RAM to OAM memory.
	for(i = 0; i < 0x9F; i++)
	{
		spriteOAM[0xFE00 | i] = memoryRead(writeData | i);
	}
}
