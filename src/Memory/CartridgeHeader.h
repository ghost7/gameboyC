#ifndef _CARTRIDGE_HEADER_
#define _CARTRIDGE_HEADER_

#include <stdint.h>
#include <string>

#include "RomType.h"
#include "MemoryDefs.h"

/**
 * @brief The cartridge header contains various information about a Game Boy cartridge.
 *
 * This structure can contain all the information stored in the cartridge header. It
 * also contains functions to extract specific information from a cartridge.
 *
 * @ingroup memory
 */
struct CartridgeHeader
{
public:
    std::string title;      //!< The title of the cartridge 
    std::string newLicense; //!< The new license code
    data_t oldLicenseCode; //!< The old license code.
    // TODO: to size_t?
    int romSize;            //!< The rom size of the cartridge.
    int romBanks;           //!< The number of ram banks in the cartridge.
    int ramSize;            //!< The ram size of the cartridge.
    int ramBanks;           //!< The number of ram banks in the cartridge.
    bool isCGBOnly;         //!< True if the cartridge is only compatible with the Game Boy Color, false otherwise.
    bool isSGB;             //!< True if the cartridge supports Super Game Boy functions, false otherwise.
    bool isJap;             //!< True if the cartridge is japanese, false otherwise.
    int romVersion;         //!< Mask ROM version number. This is usually 0.
    
    bool validHeaderSum;    //!< True if the cartridge passed the header check sum, false otherwise.
    bool validGlobalSum;    //!< True if the cartridge passed the global check sum, false otherwise.
    
    RomType romType;        //!< Type of rom, e.g. MBC1.

    std::string desc;       //!< Description string of the cartridge.
    
    /**
     * @brief Extracts all the cartridge info from rom data.
     *
     * @param rawRom Contents of a gamboy rom.
     */
    CartridgeHeader(data_t* rawRom, size_t size);
    
    /**
     * @brief Gets the title from a Game Boy rom.
     *
     * Located at address 0x134-0x143.
     *
     * @param rawRom Contents of the the Game Boy rom.
     * @return string containing the game title.
     */
    static std::string getGameTitle(data_t* rawRom);

    /**
     * @brief Checks if the game is only compatible with the Game Boy Color.
     *
     * Located at address 0x143. The upper bit is used to enable CGB 
     * functions. Values with Bit 7, and either Bit 2 or 3 set will allow
     * a Game Boy Color game to run on a Game Boy.
     *
     * @returns true if the game is only for the game boy color, false
     * otherwise.
     */
    static bool isColorOnly(data_t *rawRom);

    /**
     * @brief Checks if the game supports Super Game Boy functions.
     *
     * Located at address 0x146. 0x00 for no SGB functionality. 0x30 means
     * the game supports SGB functions.
     *
     * @return true if the game supports SGB functions, false otherwise.
     */
    static bool hasSGBFunctionality(data_t *rawRom);
    
    /**
     * @brief Gets the license code from a Game Boy rom.
     *
     * New License is located at address 0x144-0x145.
     * Old License is located at address 0x14B.
     *
     * @param rawRom Contents of the Game Boy rom.
     * @param oldLicense it to store the license code for old cartridges.
     * @return If the old license code is 0x33, then a new licence code 
     * will be returned, otherwise an empty string.
     */
    static std::string getLicenseCode(data_t* rawRom, data_t *oldLicense);
    
    /**
     * @brief Gets the rom type as a string.
     *
     * @param rawRom Contents of the Game Boy rom.
     * @return rom type as a string.
     */
    static std::string getRomTypeString(data_t *rawRom);
    
    /**
     * @brief Gets the ROM size/banks from a Game Boy rom.
     * 
     * ROM size code is located at address 0x148
     *
     * @param rawRom Contents of the Game Boy rom.
     * @param romSize int to store the rom size.
     * @param romBanks int to store the number of ram banks.
     **/
    static void getRomSize(data_t *rawRom, int *romSize, int *romBanks);

    /**
     * @brief Gets the RAM size/banks from a Game Boy rom.
     *
     * RAM size code is located at address 0x149
     *
     * @param rawRom Contents of the Game Boy rom.
     * @param ramSize int to store the ram size.
     * @param ramBanks int to store the ram banks.
     */
    static void getRamSize(data_t *rawRom, int *ramSize, int *ramBanks);
    
    /**
     * @brief Checks if the game is from Japan.
     *
     * The Game Boy has only two destination codes. 0x00 for Japanese, 
     * 0x01 for Non-Japanese.
     *
     * @param rawRom Contents of the Game Boy rom.
     * @return true if the game is Japanese, false otherwise.
     */
    static bool isJapanese(data_t *rawRom);

    /**
     * @brief Gets the ROM version number.
     *
     * Located at 0x14C. This is usually 0x00.
     *
     * @param rawRom Contents of the Game boy rom.
     * @return The version number.
     */
    static int getMaskRomVersion(data_t *rawRom);

    /**
     * @brief Checks if a Game Boy rom has the correct header check sum.
     * 
     * The eight-bit check sum is located at address 0x14D
     * It is calculated with the following formula :
     * x=0:FOR i=0134h TO 014Ch:x=x-MEM[i]-1:NEXT.
     * The GAME WONT WORK if the checksum is incorrect.
     *
     * @param rawRom Contents of the Game Boy rom.
     */
    static bool hasValidHeaderSum(data_t *rawRom);

    /**
     * @brief Checks if a Game Boy rom has the correct global check sum. 
     *
     * The 16-bit-global check sum is is located at addresses 0x14E-0x14F 
     * (upper byte first). It is calculated by adding all the bytes in the 
     * cartridge except the two check sum bytes. Note the real Game Boy does 
     * not check this value.
     *
     * @param rawRom Contents of the Game Boy rom.
     * @param romSize Size of the rom.
     */
    static bool hasValidGlobalSum(data_t *rawRom, int romSize);
};

#endif
