#ifndef _CPU_BASE_H_
#define _CPU_BASE_H_

/*! @defgroup CPU CPU
 * Defines all logic to intiliaze, interact and emulate a CPU.
 */

/**
 * @brief CPU Interface.
 *
 * @ingroup CPU
 */
class CpuBase
{
public:
    /**
     * Initializes the CPU
     */
    virtual void init() = 0;

    /**
     * Executes a single instruction.
     *
     * @return Number of cycles the step takes.
     */
    virtual int step() = 0;
};

#endif
