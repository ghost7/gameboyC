#include "Z80Debugger.h"

void Z80Debugger::attach()
{
    active = true;
}

void Z80Debugger::detach()
{
    active = false;
}

void Z80Debugger::addBreakPoint(uint8_t address)
{
    breakPoints.setBreakPoint(address);
}

bool Z80Debugger::pauseEmulation()
{
    if (active)
    {
        if (stepping)
        {
            return true; 
        }
        else
        {
            breakPoints.isBreakPointOn(registers->PC.val);
        }
    }
    return false;
}

void Z80Debugger::runToNextBreakPoint()
{
    stepping = false; 
}

void Z80Debugger::step()
{
    stepping = true;
}
