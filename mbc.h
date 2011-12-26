/*
 * Contains declarations for all cartidge read and writes.
 */


#ifndef _MBC_H_
#define _MBC_H_

#include <stdint.h>
#include "cartidge.h"

uint8_t ROM_memoryRead(Cartidge* gameData, int memAddr);
uint8_t MBC1_memoryRead(Cartidge* gameData, int memAddr);
uint8_t MBC2_memoryRead(Cartidge* gameData, int memAddr);
uint8_t MBC3_memoryRead(Cartidge* gameData, int memAddr);
uint8_t MBC5_memoryRead(Cartidge* gameData, int memAddr);

void ROM_memoryWrite(Cartidge* gameData, int memAddr, int8_t writeData);
void MBC1_memoryWrite(Cartidge* gameData, int memAddr, int8_t writeData);
void MBC2_memoryWrite(Cartidge* gameData, int memAddr, int8_t writeData);
void MBC3_memoryWrite(Cartidge* gameData, int memAddr, int8_t writeData);
void MBC5_memoryWrite(Cartidge* gameData, int memAddr, int8_t writeData);

#endif
