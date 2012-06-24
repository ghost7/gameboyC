#ifndef _ECHO_RAM_H_
#define _ECHO_RAM_H_

#include "../Memory.h"
#include "../MemoryCustomizer.h"

/**
 * @brief Echos the internal RAM
 */
class EchoRam : public MemoryCustomizer {

public:
    EchoRam( Memory* m );

    // 0xE000-0xFDFF
    virtual data_t read( addr_t addr );
    // 0xE000-0xFDFF
    virtual void write( addr_t addr, data_t val );

protected:
    MemoryInterface* mem;

};

#endif
