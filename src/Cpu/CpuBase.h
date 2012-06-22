#ifndef _CPU_BASE_H_
#define _CPU_BASE_H_

/*! \defgroup CPU CPU
 * Defines all logic to intiliaze, interact and emulate a CPU.
 */

/**
 * \brief CPU Interface.
 *
 * \ingroup CPU
 */
class CpuBase
{
public:
    /**
     * Initializes the CPU
     */
    virtual void init() = 0;

    /**
     * Shuts down the CPU.
     */
    virtual void shutDown() = 0;

    /**
     * Runs the CPU.
     */
    virtual void run() = 0;

    /**
     * Executes a single instruction.
     */
    virtual void step() = 0;
};

#endif
