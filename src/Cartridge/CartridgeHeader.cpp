#include <sstream>
#include "CartridgeHeader.h"

CartridgeHeader::CartridgeHeader(uint8_t *rawRom)
{
    title = getGameTitle(rawRom);
    isCGBOnly = isColorOnly(rawRom);
    isSGB = hasSGBFunctionality(rawRom);
    newLicense = getLicenseCode(rawRom, &oldLicenseCode);
    getRomSize(rawRom, &romSize, &romBanks);
    getRamSize(rawRom, &ramSize, &ramBanks);
    isJap = isJapanese(rawRom);
    romVersion = getMaskRomVersion(rawRom);
    validHeaderSum = hasValidHeaderSum(rawRom);
    validGlobalSum = hasValidGlobalSum(rawRom, romSize);

    isCGBOnly = rawRom[0x0143] == 0xC0;
    isSGB = rawRom[0x0146] == 0x03;
    romType = (RomType)rawRom[0x0147];
    isJap = rawRom[0x014A] == 0;
    romVersion = rawRom[0x014C];

    std::stringstream descStream;
    descStream       << "Title:                   " << title 
        << std::endl << "Old License Code:        " << (int)oldLicenseCode
        << std::endl << "New License:             " << newLicense 
        << std::endl << "Rom Size:                " << romSize
        << std::endl << "Rom Banks:               " << romBanks
        << std::endl << "GameBoy Color Only:      " << std::boolalpha << isCGBOnly
        << std::endl << "Super Gameboy Functions: " << std::boolalpha << isSGB
        << std::endl << "Japanese:                " << std::boolalpha << isJap
        << std::endl << "Rom Version:             " << romVersion
        << std::endl << "Header Check Sum:        " << std::boolalpha << validHeaderSum
        << std::endl << "Global Check Sum:        " << std::boolalpha << validGlobalSum
        << std::endl << "Rom Type:                " << getRomTypeString(rawRom);
    desc = descStream.str();
}

void CartridgeHeader::getRomSize(uint8_t *rawRom, int *romSize, int *romBanks)
{
    // ROM size is stored in 0x148
    uint8_t romSizeCode = rawRom[0x148];
    int KB = 1024;
	int MB = 1024 * 1024;
	switch (romSizeCode)
	{
		case 0x00:
			*romSize = 32 * KB;
			*romBanks = 0;
			break;
		case 0x01:
			*romSize = 64 * KB;
			*romBanks = 4;
			break;
		case 0x02:
			*romSize = 128 * KB;
			*romBanks = 8;
			break;
		case 0x03:
			*romSize = 256 * KB;
			*romBanks = 16;
			break;
		case 0x04:
			*romSize = 512 * KB;
			*romBanks = 32;
			break;
		case 0x05:
			*romSize = 1 * MB;
			*romBanks = 64;
			break;
		case 0x06:
			*romSize = 2 * MB;
			*romBanks = 128;
			break;
		case 0x07:
			*romSize = 4 * MB;
			*romBanks = 8;
			break;
		case 0x52:
			*romSize = 1179648; // 1.125 MB
			*romBanks = 72;
			break;
		case 0x53:
			*romSize = 1310720; // 1.25 MB
			*romBanks = 80;
			break;
		case 0x54:
			*romSize = 1572864; // 1.5 MB
			*romBanks = 96;
			break;
    }
}

void CartridgeHeader::getRamSize(uint8_t *rawRom, int *ramSize, int *ramBanks)
{
    // RAM size stored in 0x149
    uint8_t ramSizeCode = rawRom[0x149];
    int KB = 1024;
	switch (ramSizeCode)
	{
		case 0x00:
			*ramSize = 0;
			*ramBanks = 0;
			break;
		case 0x01:
			*ramSize = 2 * KB;
			*ramBanks = 1;
			break;
		case 0x02:
			*ramSize = 8 * KB;
			*ramBanks = 1;
			break;
		case 0x03:
			*ramSize = 32 * KB;
			*ramBanks = 4;
			break;
	}

}

std::string CartridgeHeader::getGameTitle(uint8_t *rawRom)
{
    // title location in the data starts a 0x0134 and ends 
	// at 0x0142.
	int start = 0x0134;
	int end = 0x0143;
	
    std::string title;
    for(int i = start; i < end; i++)
	{
		title += rawRom[i];
		// null terminator, end of the title.
		if(rawRom[i] == 0)
			break;
	}

	return title;
}

std::string CartridgeHeader::getLicenseCode(uint8_t* rawRom, uint8_t *oldLicense)
{
    *oldLicense = rawRom[0x014B];

    if (*oldLicense == 0x33)
    {
        std::string license;
        license += rawRom[0x0144];
        license += rawRom[0x0145];
        return license;
    }
    return "";
}

bool CartridgeHeader::isColorOnly(uint8_t *rawRom)
{
    return ((rawRom[0x0143] & 0x80) != 0);
}

bool CartridgeHeader::hasSGBFunctionality(uint8_t *rawRom)
{
    return (rawRom[0x0146] == 0x03);
}

bool CartridgeHeader::isJapanese(uint8_t *rawRom)
{
    return (rawRom[0x014A] == 0);
}

int CartridgeHeader::getMaskRomVersion(uint8_t *rawRom)
{
    return rawRom[0x14C];
}

bool CartridgeHeader::hasValidHeaderSum(uint8_t *rawRom)
{
    uint8_t sum = 0;
    for (int i = 0x0134; i <= 0x014C; i++)
    {
        sum = sum - rawRom[i] - 1;
    }
    return sum == rawRom[0x014D];
}

bool CartridgeHeader::hasValidGlobalSum(uint8_t *rawRom, int romSize)
{
    uint16_t expectedSum = rawRom[0x014E] << 8 | rawRom[0x14F];
    uint16_t sum = 0;
    for (int i = 0; i < romSize; i++)
    {
        if (i != 0x014E && i != 0x014F)
        {
            sum += rawRom[i];
        }
    }
    return sum == expectedSum;
}

std::string CartridgeHeader::getRomTypeString(uint8_t *rawRom)
{
    RomType romType = (RomType)rawRom[0x0147];
	switch(romType)
	{
		case ROM:
			return "ROM";
		case MBC1:
			return "MBC1";
		case MBC1_RAM:
			return "MBC1_RAM";
		case MBC1_RAM_BATT:
			return "MBC1_RAM_BATT";
		case MBC2:
			return "MBC2";
		case MBC2_BATT:
			return "MBC2_BATT";
		case ROM_RAM:
			return "ROM_RAM";
		case ROM_RAM_BATT:
			return "ROM_RAM_BATT";
		case MMM01:
			return "MMM01";
		case MMM01_RAM:
			return "MMM01_RAM";
		case MMM01_RAM_BATT:
			return "MMM01_RAM_BATT";
		case MBC3_TIME_BATT:
			return "MBC3_TIME_BATT";
		case MBC3_TIME_RAM_BATT:
			return "MBC3_TIME_RAM_BATT";
		case MBC3:
			return "MBC3";
		case MBC3_RAM:
			return "MBC3_RAM";
		case MBC3_RAM_BATT:
			return "MBC3_RAM_BATT";
		case MBC5:
			return "MBC5";
		case MBC5_RAM:
			return "MBC5_RAM";
		case MBC5_RAM_BATT:
			return "MBC5_RAM_BATT";
		case MBC5_RUM:
			return "MBC5_RUM";
		case MBC5_RUM_RAM:
			return "MBC5_RUM_RAM";
		case MBC5_RUM_RAM_BATT:
			return "MBC5_RUM_RAM_BATT";
		case POCKET_CAM:
			return "POCKET_CAM";
		case BANDAI_TAMA5:
			return "BANDAI_TAMA5";
		case HUC3:
			return "HUC3";
		case HUC3_RAM_BATT:
			return "HUC3_RAM_BATT";
        default:
            return "Unknown";
	}
}

