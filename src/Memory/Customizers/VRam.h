#ifndef _V_RAM_H_
#define _V_RAM_H_

#include "../Memory.h"
#include "../MemoryCustomizer.h"

/**
 * @brief Video RAM is slightly different than other RAM
 *
 * Video RAM can have odd behavior at times although
 * it may work if @c BasicMemory was used.
 * In an attempt to more accurately emulate the gameboy
 * I'm accurately implementing VRAM.
 */
class VRam : public MemoryCustomizer {

public:
    VRam( Memory* m );
    ~VRam();

    virtual data_t read( addr_t addr );
    virtual void write( addr_t addr, data_t val );

protected:
    data_t* mem;
    size_t size; // of mem in sizof(data_t)
    Memory* memObj;
};

#endif
