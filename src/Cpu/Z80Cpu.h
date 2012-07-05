#ifndef _Z80_CPU_H_
#define _Z80_CPU_H_

#include "CpuBase.h"
#include "../Memory/MemoryInterface.h"
#include "Z80.h"
#include "Z80InstructionSet.h"

/**
 * @brief Z80 CPU implementation. 
 *
 * @ingroup CPU
 */
class Z80Cpu : public ::CpuBase
{
public: 
    void init();
    int step();
    
    /**
     * Creates the Z80 CPU.
     *
     * @param mem Memory for reading and writing.
     * @param bootStrap true if using a boot strap, false otherwise.
     */
    Z80Cpu(MemoryInterface *mem, bool bootStrap = false) 
    { 
        memory = mem;
        useBootStrap = bootStrap;
    };

    Z80Registers *GetRegisters()
    {
        return &registers;
    }

private:
    /**
     * Checks for any interrupts and makes a system call if necessarry.
     */
    int checkForInterrupts();
    void advanceTimer(int stepTime); 
    
    int executeInstruction(data_t cpuInst);

    int executeCBInstruction();

    MemoryInterface *memory;
    Z80InstructionSet *instSet;
    Z80Registers registers;
    Z80Flags *flags;
    bool useBootStrap;
    bool intMasterEnable;
};

#endif
