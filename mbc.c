#include <stdint.h>
#include "mbc.h"

uint8_t currRomBank = 1;
uint8_t currRamBank = 1;
int romBanking = 1;

int8_t ROM_memoryRead(int8_t *game_data, int mem_addr)
{
	// TODO implement
	return game_data[mem_addr];
}

int8_t MBC1_memoryRead(int8_t *game_data, int mem_addr)
{
	// TODO implement
	/*
	if(mem_addr <= 0x3FFF)
		return game_data[mem_addr];
	else if (mem_addr >= 0x4000 && mem_addr <= 0x7FFF)
	{
		return game->romData[currRomBank * game->romSize / game->romBanks + memAddr];		
	}
	else if (memAddr >= 0xA000 && memAddr <= 0xBFFF)
	{
		return game->romData[currRamBank * game->ramSize / game->ramBanks + memAddr];
	}*/

	return -1;
}

int8_t MBC2_memoryRead(int8_t *game_data, int mem_addr)
{
	// TODO implement
	return -1;
}

int8_t MBC3_memoryRead(int8_t *game_data, int mem_addr)
{
	// TODO implement
	return -1;
}

int8_t MBC5_memoryRead(int8_t *game_data, int mem_addr)
{
	// TODO implement
	return -1;
}

void ROM_memoryWrite(int8_t *game_data, int mem_addr, int8_t write_data)
{
	// no write in ROM, nothing to do here?
}

void MBC1_memoryWrite(int8_t *game_data, int mem_addr, int8_t write_data)
{
	// TODO implement
	/* 
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
	}*/
}

void MBC2_memoryWrite(int8_t *game_data, int mem_addr, int8_t write_data)
{
	// TODO implement
}

void MBC3_memoryWrite(int8_t *game_data, int mem_addr, int8_t write_data)
{
	// TODO implement
}

void MBC5_memoryWrite(int8_t *game_data, int mem_addr, int8_t write_data)
{
	// TODO implement
}
