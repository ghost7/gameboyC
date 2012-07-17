#ifndef __Z80_H__
#define __Z80_H__

#include <stdint.h>
#include "../Common/Bitfield.h"

/**
 * @brief 16-bit register pair.
 *
 * @ingroup CPU
 */
union RegisterPair
{   
    uint16_t val;       //!< 16-bit value stored in the register
    /*struct
    {
        uint8_t lo;     //!< Lower 8-bits of the register
        uint8_t hi;     //!< Upper 8-bits of the register
    };*/
    Bitfield<0, 8> lo;
    Bitfield<8, 8> hi;
    
    /**
     * Creates a RegisterPair from two 8-bit register values.
     *
     * @param high Upper 8-bits of the RegisterPair.
     * @param low Lower 8-bits of the RegisterPair.
     */
    RegisterPair(uint8_t high, uint8_t low) { hi = high; lo = low; }
    
    /**
     * Creates a RegisterPair with the specifed value.
     *
     * @param value value to store in the register.
     */
    RegisterPair(uint16_t value) { val = value; }
    
    /**
     * Creates a RegisterPair with a value of 0.
     */
    RegisterPair() { val = 0; }
};

/**
 * @brief Z80 Flag 8-bit register, with the format: ZNHC0000
 *
 * @ingroup CPU 
 */
struct Z80Flags
{
    unsigned unused : 4;        //!< Lower 4-bits that should remain 0.
    unsigned C : 1;             //!< Carry flag.
    unsigned H : 1;             //!< Half Carry Flag.
    unsigned N : 1;             //!< Negation flag.
    unsigned Z : 1;             //!< Zero flag.
    Z80Flags() { }
};

/**
 * @brief Z80 Registers.
 *
 * @ingroup CPU
 */
struct Z80Registers
{
    RegisterPair BC;            //!< BC Register Pair.
    RegisterPair DE;            //!< BC Register Pair.
    RegisterPair HL;            //!< HL Register Pair.
    RegisterPair AF;            //!< Accumulator and Flags.
    RegisterPair PC;            //!< Program Counter.
    RegisterPair SP;            //!< Stack Pointer.

    /**
     * Gets the Z80 flags, which are the lower 8 bits of the AF RegisterPair.
     *
     * @return Z80 Flags associated to this set of registers.
     */
    Z80Flags *getFlags()
    {
        return (Z80Flags*)&AF.lo;
    }
};

#endif
