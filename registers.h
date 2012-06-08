#ifndef __Z80_REG__
#define __Z80_REG__

#include <stdint.h>

struct z80Reg
{
	// Registers
	int8_t A;		// Accumulator
	int8_t B;
	int8_t C;
	int8_t D;
	int8_t E;
	int8_t H;
	int8_t L;
	uint16_t PC;	// Program counter
	uint16_t SP;	// Stack pointer
};

struct z80Flags
{
	int C;			// carry flag
	int Z;			// zero flag;
	int N;			// subtract flag
	int H;			// half-carry flag
};

#endif
