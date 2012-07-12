#ifndef _DEBUG_Z80_H_
#define _DEBUG_Z80_H_

#include "../Z80.h"
#include "Z80BreakPoints.h"

class Z80Debugger
{
public:
    Z80Debugger(Z80Registers *z80Registers)
    {
        registers = z80Registers;
    }
    void attach();
    void detach();
    void addBreakPoint(uint8_t address);
    bool pauseEmulation();
    void runToNextBreakPoint();
    void step();
private:
    Z80Registers *registers;
    Z80BreakPoints breakPoints;
    bool active;
    bool stepping;
};

#endif
