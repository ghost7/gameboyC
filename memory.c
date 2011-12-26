#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "memory.h"
#include "mbc.h"



uint8_t (*cartidgeRead)(int8_t* gameData, int memAddr);
void (*cartidgeWrite)(int8_t* gameData, int memAddr, int8_t writeData);
void getReadWrite(Cartidge* game);
int8_t* gameData;
Cartidge* cartidge;

/**
 * Initializes memory to its default value
 * \param game cartidge being loaded into the memory
 * \return GAME_ERROR any error that may occur; NO_ERR if no error occurs.
 */
GAME_ERROR initMemory(Cartidge* game)
{
	int memLoc = 0xC000;
	getReadWrite(game);
	cartidge = game;
	gameData = game->romData;

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
	interruptEnabled = false;

	return NO_ERR;
}

/**
 * Read from memory.
 * \param memAddr memory address in which to read from.
 */
uint8_t memoryRead(uint8_t memAddr)
{
	if(memAddr >= 0x8000 && memAddr <= 0x9000)        // Video RAM (switchable in CGB, add support later?)
		return videoRam[memAddr - 0x8000];
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
	else if(memAddr >= 0xFE00 && memAddr <= 0xFF9F)   // Sprite Attribute Table
		return spriteOAM[memAddr - 0xFE00];
	else if(memAddr >= 0xFF00 && memAddr <= 0xFF7F)   // I/O ports
		return ioRegisters[memAddr - 0xFF00];
	else if (memAddr >= 0xFF80 && memAddr <= 0xFFFE)  // High RAM
		return highRam[memAddr - 0xFF00];
	else if(memAddr == 0xFFFF)                        // Interrupt enabled register.
		return interruptEnabled;
	else                                              // All other addresses 
		cartidgeRead(gameData, memAddr);              // are in the cartidge.
}

/**
 * Write to memory.
 */
void memoryWrite(uint8_t memAddr, uint8_t data)
{
	// TODO do something...	
}

/**
 * Get the correct cartidge read and write functions based on the MBC model.
 */
void getReadWrite(Cartidge* game)
{
	// TODO add other models
	switch(game->romVersion)
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
