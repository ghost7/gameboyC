#ifndef _BASIC_MEMORY_H_
#define _BASIC_MEMORY_H_

#include "../MemoryCustomizer.h"

/**
 * @brief Whenever you need memory to do exactly what you think memory should do, use this.
 *
 * This implements basic storage and retrieval of data
 * over a specific address range. 
 */
class BasicMemory : public MemoryCustomizer {

public:
    /**
     * Create a basic memory for the given address range.
     * @pre @c start must be less than @c end
     */
    BasicMemory( addr_t start, addr_t end );
    ~BasicMemory();

    virtual data_t read( addr_t addr );
    virtual void write( addr_t addr, data_t val );

protected:
    data_t* mem;
    size_t size;
    addr_t offset;
};

#endif
