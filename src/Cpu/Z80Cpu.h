#ifndef _Z80_CPU_H_
#define _Z80_CPU_H_

#include "CpuBase.h"
#include "../Memory/Memory.h"
#include "../Memory/Customizers/IOMemory.h"
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
    Z80Cpu(Memory *mem);

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

    Memory *memory;
    IOMemory *ioMemory;
    
    Z80InstructionSet *instSet;
    Z80Registers registers;
    Z80Flags *flags;
    bool intMasterEnable;
};

#endif
