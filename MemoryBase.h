#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stdint.h>

class MemoryBase
{
public:
    virtual uint8_t memoryRead(uint16_t memAddr) = 0;
    virtual void memoryWrite(uint16_t memAddr, uint8_t data) = 0;
};

#endif
