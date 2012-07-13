#include "IOMemory.h"

IOMemory::IOMemory() {
}

data_t IOMemory::read( addr_t addr ) 
{
    // Check if the Wave Pattern Ram is being read.
    if (addr >= 0xFF30 && addr <= 0xFF3F)
    {
        return WaveRam[addr - 0xFF30]; 
    }
    switch(addr)
    {
        case 0xFF00:
            return JOYP;
        case 0xFF01:
            return SD;
        case 0xFF02:
            return SC;
        case 0xFF04:
            return DIV;
        case 0xFF05:
            return TIMA;
        case 0xFF06:
            return TMA;
        case 0xFF07:
            return TAC;
        case 0xFF0F:
            return IFLAGS;
        case 0xFF10:
            return NR10;
        case 0xFF11:
            return NR11;
        case 0xFF12:
            return NR12;
        case 0xFF13:
            return NR13;
        case 0xFF14:
            return NR14;
        case 0xFF16:
            return NR21;
        case 0xFF17:
            return NR22;
        case 0xFF18:
            return NR23;
        case 0xFF19:
            return NR24;
        case 0xFF1A:
            return NR30;
        case 0xFF1B:
            return NR31;
        case 0xFF1C:
            return NR32;
        case 0xFF1D:
            return NR33;
        case 0xFF1E:
            return NR34;
        case 0xFF20:
            return NR41;
        case 0xFF21:
            return NR42;
        case 0xFF22:
            return NR43;
        case 0xFF23:
            return NR44;
        case 0xFF24:
            return NR50;
        case 0xFF25:
            return NR51;
        case 0xFF26:
            return NR52;
        case 0xFF40:
            return LCDC;
        case 0xFF41:
            return STAT;
        case 0xFF42:
            return SCY;
        case 0xFF43:
            return SCX;
        case 0xFF44:
            return LY;
        case 0xFF45:
            return LYC;
        case 0xFF47:
            return BGP;
        case 0xFF48:
            return OBP0;
        case 0xFF49:
            return OBP1;
        case 0xFF4A:
            return WY;
        case 0xFF4B:
            return WX;
        default:
            return 0xFF;    // Not a register...what to return?
    }
}

void IOMemory::write( addr_t addr, data_t val ) 
{
    // Check if the Wave Pattern Ram is being written to.
    if (addr >= 0xFF30 && addr <= 0xFF3F)
    {
        WaveRam[addr - 0xFF30] = val; 
    }
    switch(addr)
    {
        case 0xFF00:
            JOYP = val & 0xFFF0; // Bits 3-0 read-only
            break;
        case 0xFF01:
            SD = val;
            break;
        case 0xFF02:
            SC = val;
            break;
        case 0xFF04:
            DIV = val;
            break;
        case 0xFF05:
            TIMA = val;
            break;
        case 0xFF06:
            TMA = val;
            break;
        case 0xFF07:
            TAC = val;
            break;
        case 0xFF0F:
            IFLAGS = val;
            break;
        case 0xFF10:
            NR10 = val;
            break;
        case 0xFF11:
            NR11 = val;
            break;
        case 0xFF12:
            NR12 = val;
            break;
        case 0xFF13:
            NR13 = val;
            break;
        case 0xFF14:
            NR14 = val;
            break;
        case 0xFF16:
            NR21 = val;
            break;
        case 0xFF17:
            NR22 = val;
            break;
        case 0xFF18:
            NR23 = val;
            break;
        case 0xFF19:
            NR24 = val;
            break;
        case 0xFF1A:
            NR30 = val;
            break;
        case 0xFF1B:
            NR31 = val;
            break;
        case 0xFF1C:
            NR32 = val;
            break;
        case 0xFF1D:
            NR33 = val;
            break;
        case 0xFF1E:
            NR34 = val;
            break;
        case 0xFF20:
            NR41 = val;
            break;
        case 0xFF21:
            NR42 = val;
            break;
        case 0xFF22:
            NR43 = val;
            break;
        case 0xFF23:
            NR44 = val;
            break;
        case 0xFF24:
            NR50 = val;
            break;
        case 0xFF25:
            NR51 = val;
            break;
        case 0xFF26:
            NR52 = val & 0xFFF0; // Bit 3-0 read-only
            break;
        case 0xFF40:
            LCDC = val;
            break;
        case 0xFF41:
            STAT = val & 0xFFF8; // Bit 2-1 read-only
            break;
        case 0xFF42:
            SCY = val;
            break;
        case 0xFF43:
            SCX = val;
            break;
        case 0xFF44:
            LY = val;
            break;
        case 0xFF45:
            LYC = val;
            break;
        case 0xFF47:
            BGP = val;
            break;
        case 0xFF48:
            OBP0 = val;
            break;
        case 0xFF49:
            OBP1 = val;
            break;
        case 0xFF4A:
            WY = val;
            break;
        case 0xFF4B:
            WX = val;
            break;
    }
}
