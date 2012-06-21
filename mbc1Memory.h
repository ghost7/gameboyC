#ifndef _MBC1_MEMORY_H_
#define _MBC1_MEMORY_H_

#include "memoryHeaders.h"

class Mbc1Memory : public ::Memory {

public:
    Mbc1Memory( data_t* c, int cSize );
    ~Mbc1Memory();

    // read from addr
    virtual data_t read( addr_t addr );
    // write val to addr
    virtual void write( addr_t addr, data_t val );

protected:
    /** The current ROM bank number. */
    int romBank;
    /** The current RAM bank number. */
    int ramBank;
    // pointer to external RAM
    data_t* ram;
    // current banking mode
    BankingMode bankMode;
};

#endif
