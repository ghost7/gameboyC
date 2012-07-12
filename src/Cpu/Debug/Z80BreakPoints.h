#ifndef _DEBUG_BREAKPOINTS_H_
#define _DEBUG_BREAKPOINTS_H_

#include <stdint.h>
#include <map>

class Z80BreakPoints
{
public:
    bool isBreakPointOn(uint8_t address);
    void setBreakPoint(uint8_t address);
    void clearBreakPoint(uint8_t address);
    void clearAllBreakPoints();
    void toggleBreakPoint(uint8_t address);
private:
    std::map<uint8_t, bool> breakPointMap;   
};

#endif
