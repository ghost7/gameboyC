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
    
    // IO Registers, reading and writing will modify these registers.
    data_t JOYP;         // 0xFF00
    data_t SD;           // 0xFF01
    data_t SC;           // 0xFF02
    data_t DIV;          // 0xFF04
    data_t TIMA;         // 0xFF05
    data_t TMA;          // 0xFF06
    data_t TAC;          // 0xFF07
    data_t IFLAGS;       // 0xFF0F
    data_t NR10;         // 0xFF10
    data_t NR11;         // 0xFF11
    data_t NR12;         // 0xFF12
    data_t NR13;         // 0xFF13
    data_t NR14;         // 0xFF14
    data_t NR21;         // 0xFF16
    data_t NR22;         // 0xFF17
    data_t NR23;         // 0xFF18
    data_t NR24;         // 0xFF19
    data_t NR30;         // 0xFF1A
    data_t NR31;         // 0xFF1B
    data_t NR32;         // 0xFF1C
    data_t NR33;         // 0xFF1D
    data_t NR34;         // 0xFF1E
    data_t NR41;         // 0xFF20
    data_t NR42;         // 0xFF21
    data_t NR43;         // 0xFF22
    data_t NR44;         // 0xFF23
    data_t NR50;         // 0xFF24
    data_t NR51;         // 0xFF25  
    data_t NR52;         // 0xFF26
    data_t WaveRam[0xF]; // 0xFF30-0xFF3F
    data_t LCDC;         // 0xFF40
    data_t STAT;         // 0xFF41
    data_t SCY;          // 0xFF42
    data_t SCX;          // 0xFF43
    data_t LY;           // 0xFF44
    data_t LYC;          // 0xFF45
    data_t BGP;          // 0xFF47
    data_t OBP0;         // 0xFF48
    data_t OBP1;         // 0xFF49
    data_t WY;           // 0xFF4A
    data_t WX;           // 0xFF4B
};

#endif
