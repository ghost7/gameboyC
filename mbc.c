#include <stdint.h>
#include "mbc.h"

uint8_t currRomBank = 1;
uint8_t currRamBank = 1;
int romBanking = 1;

uint8_t ROM_memoryRead(Cartidge* game, int memAddr)
{
	// TODO implement
	return game->romData[memAddr];
}

uint8_t MBC1_memoryRead(Cartidge* game, int memAddr)
{
	// TODO implement
	if(memAddr <= 0x3FFF)
		return game->romData[memAddr];
	else if (memAddr >= 0x4000 && memAddr <= 0x7FFF)
	{
		return game->romData[currRomBank * game->romSize / game->romBanks + memAddr];		
	}
	else if (memAddr >= 0xA000 && memAddr <= 0xBFFF)
	{
		return game->romData[currRamBank * game->ramSize / game->ramBanks + memAddr];
	}

	return -1;
}

uint8_t MBC2_memoryRead(Cartidge* game, int memAddr)
{
	// TODO implement
	return -1;
}

uint8_t MBC3_memoryRead(Cartidge* game, int memAddr)
{
	// TODO implement
	return -1;
}

uint8_t MBC5_memoryRead(Cartidge* game, int memAddr)
{
	// TODO implement
	return -1;
}

void ROM_memoryWrite(Cartidge* gameData, int memAddr, int8_t writeData)
{
	// no write in ROM, nothing to do here?
}

void MBC1_memoryWrite(Cartidge* game, int memAddr, int8_t writeData)
{
	// TODO implement
	if(memAddr >= 0xA000 && memAddr <= 0xBFFF)
	{
		game->romData[currRamBank * game->ramSize / game->ramBanks + memAddr] = writeData;
	}
	else if (memAddr >= 0x2000 && memAddr <= 0x3FFF)
	{
		writeData &= 0x1F;
		if(writeData == 0x00)
			writeData = 0x01;
		currRomBank = (currRomBank & 0x60) | writeData;
	}
	else if(memAddr >= 0x4000 && memAddr <= 0x5FFF)
	{
		if(romBanking)
		{
			writeData &= (writeData << 5) & 0x60;
			currRomBank = (currRomBank & 0x1F) | writeData;
		}
		else
		{
			currRamBank = writeData & 0x3;
		}
	}
}

void MBC2_memoryWrite(Cartidge* gameData, int memAddr, int8_t writeData)
{
	// TODO implement
}

void MBC3_memoryWrite(Cartidge* gameData, int memAddr, int8_t writeData)
{
	// TODO implement
}

void MBC5_memoryWrite(Cartidge* gameData, int memAddr, int8_t writeData)
{
	// TODO implement
}
