#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "memory.h"
#include "mbc.h"

uint8_t* vRam;
uint8_t* gameBoyMem;

uint8_t (*cartidgeRead)(int8_t* gameData, int memAddr);
void (*cartidgeWrite)(int8_t* gameData, int memAddr, int8_t writeData);
void getReadWrite(Cartidge* game);
int8_t* gameData;

/**
 * Initializes memory to its default value
 * \param game cartidge being loaded into the memory
 * \return GAME_ERROR any error that may occur; NO_ERR if no error occurs.
 */
GAME_ERROR initMemory(Cartidge* game)
{
	vRam = malloc(sizeof(uint8_t) * (0xA000 - 0x8000));
	int memLoc = 0xC000;
	gameBoyMem = malloc(sizeof(uint8_t) * (0x10000 - memLoc));
	getReadWrite(game);
	gameData = game->romData;

	if(vRam == NULL || gameBoyMem == NULL)
	{
		return MEM_ERR;
	}

	// initialize register values
	gameBoyMem[0xFF05 - memLoc] = 0x00;	// TIMA
	gameBoyMem[0xFF06 - memLoc] = 0x00; // TMA
	gameBoyMem[0xFF07 - memLoc] = 0x00; // TAC
	gameBoyMem[0xFF10 - memLoc] = 0x80; // NR10
	gameBoyMem[0xFF11 - memLoc] = 0xBF; // NR11
	gameBoyMem[0xFF12 - memLoc] = 0xF3; // NR12
	gameBoyMem[0xFF14 - memLoc] = 0xBF; // NR14
	gameBoyMem[0xFF16 - memLoc] = 0x3F; // NR21
	gameBoyMem[0xFF17 - memLoc] = 0x00; // NR22
	gameBoyMem[0xFF19 - memLoc] = 0xBF; // NR24
	gameBoyMem[0xFF1A - memLoc] = 0x7F; // NR30
	gameBoyMem[0xFF1B - memLoc] = 0xFF; // NR31
	gameBoyMem[0xFF1C - memLoc] = 0x9F; // NR32
	gameBoyMem[0xFF1E - memLoc] = 0xBF; // NR33
	gameBoyMem[0xFF20 - memLoc] = 0xFF; // NR41
	gameBoyMem[0xFF21 - memLoc] = 0x00; // NR42
	gameBoyMem[0xFF22 - memLoc] = 0x00; // NR43
	gameBoyMem[0xFF23 - memLoc] = 0xBF; // NR30
	gameBoyMem[0xFF24 - memLoc] = 0x77; // NR50
	gameBoyMem[0xFF25 - memLoc] = 0xF3; // NR51
	if(game->SGB)						// NR52
		gameBoyMem[0xFF26 - memLoc] = 0xF0; 
	else
		gameBoyMem[0xFF26 - memLoc] = 0xF1; 
	gameBoyMem[0xFF40 - memLoc] = 0x91; // LCDC
	gameBoyMem[0xFF42 - memLoc] = 0x00; // SCY
	gameBoyMem[0xFF43 - memLoc] = 0x00; // SCX 
	gameBoyMem[0xFF45 - memLoc] = 0x00; // LYC
	gameBoyMem[0xFF47 - memLoc] = 0xFC; // BGP
	gameBoyMem[0xFF48 - memLoc] = 0xFF; // OBP0
	gameBoyMem[0xFF49 - memLoc] = 0xFF; // OBP1
	gameBoyMem[0xFF4A - memLoc] = 0x00; // WY
	gameBoyMem[0xFF4B - memLoc] = 0x00; // WX
	gameBoyMem[0xFFFF - memLoc] = 0x00; // IE

	return NO_ERR;
}

/**
 * Destroys the gameboy's memory
 */
void destroyMemory()
{
	free(vRam);
	free(gameBoyMem);
}

/**
 * Read from memory.
 */
uint8_t memoryRead(uint8_t memAddr)
{
	if(memAddr >= 0x8000 && memAddr <= 0x9FFF)
	{
		return vRam[memAddr - 0x8000];
	}
	else if(memAddr <= 0xBFFF)
	{
		// read from the cartidge
		return cartidgeRead(gameData, memAddr);
	}
	else
	{
		return gameBoyMem[memAddr - 0xC000];
	}
}

/**
 * Write to memory.
 */
void memoryWrite(uint8_t memAddr, uint8_t data)
{
	if(memAddr > 0x7FFF && memAddr < 0xA000)
	{
		vRam[memAddr - 0x8000] = data;
	}
	else if(memAddr >= 0xA000 && memAddr <= 0xBFFF)
	{
		// write to cartidge ram
		cartidgeWrite(gameData, memAddr, data);
	}
	else if(memAddr >= 0xC000)
	{
		gameBoyMem[memAddr - 0xC000] = data;
		// echo 0xC000-0xDDFF to 0xE000 to 0xFDFF
		if(memAddr >= 0xE000 && memAddr <= 0xFDFF)
		{
			gameBoyMem[memAddr - 0xE000] = data;
		}
		if(memAddr >= 0xC000 && memAddr <= 0xDDFF)
		{
			gameBoyMem[(memAddr | 0x2000) - 0xC000] = data;
		}
	}
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
