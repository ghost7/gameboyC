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

// LCD registers
uint8_t register_LCDC;  // 0xFF40  LCD Control
uint8_t register_STAT;  // 0xFF41  LCDC Status 
uint8_t register_SCX;   // 0xFF42  Scroll X
uint8_t register_SCY;   // 0xFF43  Scroll Y
uint8_t register_LY;    // 0xFF44  LCDC Y-Coordinate
uint8_t register_LYC;   // 0xFF45  LY Compare
uint8_t register_WY;    // 0xFF4A  Window Y position
uint8_t register_WX;    // 0xFF4B  Window X position minus 7
uint8_t register_BGP[4];   // 0xFF47  Background Pallet Data
uint8_t register_OBP0;  // 0xFF48  Object Pallete 0 Data
uint8_t register_OBP1;  // 0xFF49  Object Pallete 1 Data

// Interrupts
uint8_t intMasEnable;   // Interrupt master enable
uint8_t register_IF;    // 0xFF0F  Interrupt flags
uint8_t register_IE;     // 0xFFFF  Interrupt enable

GAME_ERROR initMemory(Cartidge* game);

void destroyMemory();

uint8_t memoryRead(uint16_t memAddr);

void memoryWrite(uint16_t memAddr, uint8_t data);

void compareLYToLYC();

#endif
