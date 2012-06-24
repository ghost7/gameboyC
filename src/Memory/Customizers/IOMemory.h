#ifndef _IO_MEMORY_H_
#define _IO_MEMORY_H_

#include "../MemoryCustomizer.h"

/**
 * @brief Memory behavior for the IO ports
 * 
 * Currently this class just emulates which bits can be 
 * read and which can be written.
 */
class IOMemory : public MemoryCustomizer {

public:
    IOMemory();

    virtual data_t read( addr_t addr );
    virtual void write( addr_t addr, data_t val );

protected:
    data_t mem[0x7F];
};

#endif
