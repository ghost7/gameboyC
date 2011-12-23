/**
 * Contains functions to access Gameboy memory.
 */

#ifndef _GAME_BOY_MEM_H_
#define _GAME_BOY_MEM_H_

#include <stdint.h>
#include "cartidge.h"

GAME_ERROR initMemory(Cartidge* game);

void destroyMemory();

uint8_t memoryRead(uint8_t memAddr);

void memoryWrite(uint8_t memAddr, uint8_t data);

#endif
