#ifndef _Z80_CPU_H_
#define _Z80_CPU_H_

#include "CpuBase.h"
#include "../Memory/MemoryInterface.h"
#include "Z80.h"
#include "Z80InstructionSet.h"

/**
 * \brief Z80 CPU implementation. TODO Implement...
 *
 * \ingroup CPU
 */
class Z80Cpu : CpuBase
{
public: 
    void init();
    void step();
    
    /**
     * Creates the Z80 CPU.
     *
     * \param mem Memory for reading and writing.
     * \param bootStrap true if using a boot strap, false otherwise.
     */
    Z80Cpu(MemoryInterface *mem, bool bootStrap) 
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
    void checkForInterrupts();
    /**
     * Read the timer value from memory.
     */
    void readTimer();
    /**
     * Write the timer value from memory. If the timer value 
     * is greater than 0xFF load the timer with the timer
     * modulo value.
     */
    void writeTimer();
    
    void executeInstruction(data_t cpuInst);

    void executeCBInstruction();

    MemoryInterface *memory;
    Z80InstructionSet *instSet;
    Z80Registers registers;
    Z80Flags *flags;
    bool useBootStrap;
    bool intMasterEnable;
    int timer;
};

#endif
