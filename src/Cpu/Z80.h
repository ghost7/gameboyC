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
    uint16_t val;                 //!< 16-bit value stored in the register
    Bitfield<0, 8, uint16_t> lo;  //!< Lower 8-bits of the register
    Bitfield<8, 8, uint16_t> hi;  //!< Upper 8-bits of the register
    
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
union Z80Flags
{
private:
    uint8_t value;
public:
    Bitfield<7, 1, uint8_t> Z;
    Bitfield<6, 1, uint8_t> N;
    Bitfield<5, 1, uint8_t> H;
    Bitfield<4, 1, uint8_t> C;
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

/*
 * @breif Z80 Half Registers helper.
 * 
 * This structure can be used to extract the 8-bit register values from a set of
 * Z80Registers. 
 */
struct Z80HalfRegisters
{
    uint8_t B, C, D, E, H, L, A;

    /**
     * Create the half-registers from the Z80 registers.
     *
     * @param registers The registers to create from.
     */
    Z80HalfRegisters(Z80Registers registers)
    {
        B = registers.BC.hi;
        C = registers.BC.lo;
        D = registers.DE.hi;
        E = registers.DE.lo;
        H = registers.HL.hi;
        L = registers.HL.lo;
        A = registers.AF.hi;
    }

    /**
     * Store the half-register values into Z80 registers.
     * 
     * @param registers Registers to store into.
     */
    void storeRegisters(Z80Registers* registers)
    {
        registers->BC.hi = B;
        registers->BC.lo = C;
        registers->DE.hi = D;
        registers->DE.lo = E;
        registers->HL.hi = H;
        registers->HL.lo = L;
        registers->AF.hi = A;
    }
};

#endif
