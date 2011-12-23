#include <stdint.h>
#include "mbc.h"

uint8_t* cartidgeMem;
uint8_t currRomBank;

uint8_t ROM_memoryRead(int8_t* gameData, int memAddr)
{
	return gameData[memAddr - 0xA000];
}

uint8_t MBC1_memoryRead(int8_t* gameData, int memAddr)
{
	// TODO implement
	return -1;
}

uint8_t MBC2_memoryRead(int8_t* gameData, int memAddr)
{
	// TODO implement
	return -1;
}

uint8_t MBC3_memoryRead(int8_t* gameData, int memAddr)
{
	// TODO implement
	return -1;
}

uint8_t MBC5_memoryRead(int8_t* gameData, int memAddr)
{
	// TODO implement
	return -1;
}


void ROM_memoryWrite(int8_t* gameData, int memAddr, int8_t writeData)
{
	// no write in ROM, nothing to do here?
}

void MBC1_memoryWrite(int8_t* gameData, int memAddr, int8_t writeData)
{
	// TODO implement
}

void MBC2_memoryWrite(int8_t* gameData, int memAddr, int8_t writeData)
{
	// TODO implement
}

void MBC3_memoryWrite(int8_t* gameData, int memAddr, int8_t writeData)
{
	// TODO implement
}

void MBC5_memoryWrite(int8_t* gameData, int memAddr, int8_t writeData)
{
	// TODO implement
}

