#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stdint.h>

/*! \defgroup memory Memory
 * Defines all logic to create and interact with memory.
 */

/**
 * \brief Memory Interface.
 *
 * \ingroup memory
 */
class MemoryBase
{
public:
    /**
     * \brief Read from memory.
     *
     * \param memAddr Memory address in which to read from.
     */
    virtual uint8_t memoryRead(uint16_t memAddr) = 0;

    /**
     * \brief Write to memory.
     *
     * \param memAddr Memory address to write to.
     * \param data Data to write.
     */
    virtual void memoryWrite(uint16_t memAddr, uint8_t data) = 0;
};

#endif
