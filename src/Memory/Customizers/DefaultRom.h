#ifndef _DEFAULT_ROM_H_
#define _DEFAULT_ROM_H_

#include "../MemoryCustomizer.h"

/**
 * 0x0000 - 0x7FFF read only
 * The default actions for ROM are the same as for when there
 * is no MBC. The possibility of RAM banking is included.
 */
class DefaultRom : public MemoryCustomizer {

public:
    DefaultRom( data_t* cart, size_t cSize );
    virtual data_t read( addr_t addr );
    virtual void write( addr_t addr, data_t val );
};

#endif
