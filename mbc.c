#include <stdint.h>
#include "mbc.h"

uint8_t* cartidgeMem;
uint8_t currRomBank;

uint8_t ROM_memoryRead(Cartidge* game, int memAddr)
{
	// TODO implement
}

uint8_t MBC1_memoryRead(Cartidge* game, int memAddr)
{
	// TODO implement
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

void MBC1_memoryWrite(Cartidge* gameData, int memAddr, int8_t writeData)
{
	// TODO implement
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
