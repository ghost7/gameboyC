#ifndef _LAZY_MEMORY_H_
#define _LAZY_MEMORY_H_

#include "../MemoryCustomizer.h"

/**
 * Use this if you want write to do nothing and 
 * read to return the ever consistent 0.
 */
class LazyMemory : public MemoryCustomizer {

public:
    virtual data_t read( addr_t addr ) { return 0; }
    virtual void write( addr_t addr, data_t val ) {}
};

#endif
