/**
 * Contains functions to access Gameboy memory.
 */

#ifndef _GAME_BOY_MEM_H_
#define _GAME_BOY_MEM_H_

#include <stdint.h>
#include "cartidge.h"

uint8_t romBank0[0x4000];
uint8_t videoRam[0xA000 - 0x8000];
uint8_t workRam0[0xD000 - 0xC000];
uint8_t workRam1[0xE000 - 0xD000];
uint8_t spriteOAM[0xFEA0 - 0xFE00];
uint8_t ioRegisters[0xFF80 - 0xFF00];
uint8_t highRam[0xFFFF - 0xFF80];
uint8_t interruptEnabled;

GAME_ERROR initMemory(Cartidge* game);

void destroyMemory();

uint8_t memoryRead(uint8_t memAddr);

void memoryWrite(uint8_t memAddr, uint8_t data);

#endif
