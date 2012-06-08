/*
 * Contains declarations for all cartidge read and writes.
 */


#ifndef _MBC_H_
#define _MBC_H_

#include <stdint.h>
#include "cartidge.h"

int8_t ROM_memoryRead(int8_t* gameData, int memAddr);
int8_t MBC1_memoryRead(int8_t* gameData, int memAddr);
int8_t MBC2_memoryRead(int8_t* gameData, int memAddr);
int8_t MBC3_memoryRead(int8_t* gameData, int memAddr);
int8_t MBC5_memoryRead(int8_t* gameData, int memAddr);

void ROM_memoryWrite(int8_t* gameData, int memAddr, int8_t writeData);
void MBC1_memoryWrite(int8_t* gameData, int memAddr, int8_t writeData);
void MBC2_memoryWrite(int8_t* gameData, int memAddr, int8_t writeData);
void MBC3_memoryWrite(int8_t* gameData, int memAddr, int8_t writeData);
void MBC5_memoryWrite(int8_t* gameData, int memAddr, int8_t writeData);

#endif
