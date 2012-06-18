#ifndef _Z80_CPU_H_
#define _Z80_CPU_H_

#include "CpuBase.h"
#include "MemoryBase.h"
#include "Z80.h"

class Z80Cpu : CpuBase
{
public 
    init();
    shutdown();
    run();
    step();

    Z80Cpu(MemoryBase *mem) { memory = mem };
private 
    MemoryBase *memory;
    Z80InstuctionSet *instSet;
    Z80Registers registers;
};

#endif
