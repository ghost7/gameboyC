#ifndef _GB_DEBUG_H_
#define _GB_DEBUG_H_

#include <stdint.h>

void printInst(const int memAddr, const uint8_t opcode);
void printCBInst(const int memAddr, const uint8_t opcode);
void printRegisters();
void printDebug(const char* format, ...);

#endif
