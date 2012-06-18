#ifndef __Z80_H__
#define __Z80_H__

#include <stdint.h>

union RegisterPair
{   
    uint16_t val;
    struct
    {
        uint8_t lo;
        uint8_t hi;
    };
    RegisterPair(uint8_t high, uint8_t low) { hi = high; lo = low; }
    RegisterPair(uint16_t value) { val = value; }
    RegisterPair() { val = 0; }
};

struct Z80Flags
{
    const unsigned unused : 4;
    bool C : 1;
    bool H : 1;
    bool N : 1;
    bool Z : 1;
};

struct Z80Registers
{
    RegisterPair BC;
    RegisterPair CD;
    RegisterPair DE;
    RegisterPair HL;
    RegisterPair AF;
    RegisterPair PC;
    RegisterPair SP;
    Z80Flags *getFlags()
    {
        return (Z80Flags*)&AF.lo;
    }
};

#endif
