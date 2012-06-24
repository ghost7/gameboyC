#ifndef _DEFAULT_E_RAM_H_
#define _DEFAULT_E_RAM_H_

#include "../Memory.h"
#include "../MemoryCustomizer.h"

/**
 * External RAM implementation
 */
class DefaultERam : public MemoryCustomizer {

public:
    DefaultERam( Memory* mem );
    ~DefaultERam();

    // 0xA000 - 0xBFFF
    virtual data_t read( addr_t addr );
    // 0x0000 - 0x1FFF and
    // 0xA000 - 0xBFFF
    virtual void write( addr_t addr, data_t val );

protected:
    data_t* eram;
    bool eRamEnabled;
};

#endif
