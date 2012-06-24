#ifndef _MEMORY_CUSTOMIZER_H_
#define _MEMORY_CUSTOMIZER_H_

#include "MemoryInterface.h"

/**
 * This class implements the functionality of a specific
 * address range or ranges.
 * The idea of a @c MemoryCustomizer is tied to the decisions
 * made in the @c Memory class. 
 * 
 * NOTE: top level memory layers which use this should guarantee that
 * requests to specific interfaces are within their registered
 * address range.
 */
class MemoryCustomizer : public MemoryInterface{

public:

    /**
     * @c Memory relies on the presence of raw cartridge data,
     * so instances of this class may do so as well...
     */
    MemoryCustomizer( data_t* cart, size_t cSize );
    // ...but they don't have to
    MemoryCustomizer();

    virtual data_t read( addr_t addr ) = 0;
    virtual void write( addr_t addr, data_t val ) = 0;

protected:
    data_t* cart;
    size_t cSize;
};

#endif
