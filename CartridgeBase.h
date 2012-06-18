#ifndef _CARTIDGE_BASE_H_
#define _CARTIDGE_BASE_H_

#include <stdint.h>

/**
 * \enum rom_type
 * \brief Represents the different possible rom types. 
 */
enum RomType
{
	ROM =                   0x00,
	MBC1 =                  0x01,
	MBC1_RAM =              0x02,
	MBC1_RAM_BATT =         0x03,
	MBC2 =                  0x05,
	MBC2_BATT =             0x06,
	ROM_RAM =               0x08,
	ROM_RAM_BATT =          0x09,
	MMM01 =                 0x0B,
	MMM01_RAM =             0x0C,
	MMM01_RAM_BATT =        0x0D,
	MBC3_TIME_BATT =        0x0F,
	MBC3_TIME_RAM_BATT =    0x10,
	MBC3 =                  0x11,
	MBC3_RAM =              0x12,
	MBC3_RAM_BATT =         0x13,
//	MBC4 =                  0x15,
//	MBC4_RAM =              0x16,
//	MBC4_RAM_BATT =         0x17,
	MBC5 =                  0x19,
	MBC5_RAM =              0x1A,
	MBC5_RAM_BATT =         0x1B,
	MBC5_RUM =              0x1C,
	MBC5_RUM_RAM =          0x1D,
	MBC5_RUM_RAM_BATT =     0x1E,
	POCKET_CAM =            0xFC,
	BANDAI_TAMA5 =          0xFD,
	HUC3 =                  0xFE,
	HUC3_RAM_BATT =         0xFF
};

class CartidgeBase
{
public :
    virtual void load(const char *filename);
    uint8_t read(uint16_t memAddr) = 0;
    void write(uint8_t memAddr, uint8_t data) = 0;
    virtual char *getInfo();

private : 
    uint8_t *getFileContents(const char *filename);
    void getRomSize();
    void getRamSize();
    char *getGameTitle();
    char *getLicenseCode();

    char *title;
    char *license;
    int romVersion;
    bool isCGB;
    bool isSGB;

protected:
    int romSize;
    int romBanks;
    int ramSize;
    int ramBanks;
    uint8_t *contents;
};

#endif
