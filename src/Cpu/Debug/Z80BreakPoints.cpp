#include "Z80BreakPoints.h"

bool Z80BreakPoints::isBreakPointOn(uint8_t address)
{
    if (breakPointMap.count(address) > 0)
    {
        return breakPointMap[address];
    }
    else
    {
        return false;
    }
}

void Z80BreakPoints::setBreakPoint(uint8_t address)
{
    breakPointMap[address] = true;
}

void Z80BreakPoints::clearBreakPoint(uint8_t address)
{
    breakPointMap.erase(address);
}

void Z80BreakPoints::clearAllBreakPoints()
{
    breakPointMap.clear();
}

void Z80BreakPoints::toggleBreakPoint(uint8_t address)
{
    if (breakPointMap.count(address) > 0)
    {
        breakPointMap[address] = !breakPointMap[address];
    }
    else
    {
        breakPointMap[address] = true;
    }
}
