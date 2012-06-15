/**
 * \file registers.h
 * \brief Contains definitions for CPU components.
 */

#ifndef __Z80_REG__
#define __Z80_REG__

#include <stdint.h>

/**
 * \struct z80Reg
 * \brief Z80 cpu registers.
 */
struct z80Reg
{
	uint8_t A;		/**< A register, Accumulator. */
	uint8_t B;      /**< B register, part of the register pair BC */
	uint8_t C;      /**< C register, part of the register pair BC */
	uint8_t D;      /**< D register, part of the register pair DE */
	uint8_t E;      /**< E register, part of the register pair DE */
	uint8_t H;      /**< H register, part of the register pair HL */
	uint8_t L;      /**< L register, part of the register pair HL */
	uint16_t PC;	/**< Program Counter */
	uint16_t SP;	/**< Stack pointer */
};

/**
 * \struct z80Flags
 * \brief Z80 cpu flags.
 */
struct z80Flags
{
	int C;			/**< Carry flag */
	int Z;			/**< Zero flag */
	int N;			/**< Negation flag */
	int H;			/**< Half-carry flag */
};

#endif
