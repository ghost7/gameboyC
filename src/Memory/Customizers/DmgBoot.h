#ifndef _DMG_BOOT_H_
#define _DMG_BOOT_H_

#include "../MemoryCustomizer.h"

/**
 * DMG boot memory. This is a 256-bit program that causes the Nintendo
 * Logo to scroll to the center of the screen. It also checks the cartridge
 * header checksum and if a valid Nintendo logo exists in the cartridge.
 */
class DmgBoot : public MemoryCustomizer
{
public:
    DmgBoot();

    virtual data_t read(addr_t addr);
    virtual void write(addr_t addr, data_t val);
    
    /**
     * Is DMG boot enabled?
     */
    bool isEnabled();
private:
    data_t* dmgProgram;
    bool enabled;
};

#endif
