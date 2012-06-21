/**
 * \file cartidge.c
 * \brief Contains definitions for all functions involving loading and 
 * accessing/modifying a cartidge. 
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "cartridge.h"
#include "mbc.h"

/**
 * Error code
 */
static enum gb_error err;

/**
 * \enum gb_error
 * Possible errors produced from loading the cartidge.
 */
enum gb_error
{
 	NO_ERR,
	H_CHECK_SUM_ERR,
	G_CHECK_SUM_ERR,
	MEM_ERR,
	FILE_ERR,
	INVALID_ERR
};

/**
 * \enum rom_type
 * \brief Represents the different possible rom types. 
 */
enum rom_type
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

/**
 * \struct gb_cartidge
 * \brief Contains all the information about a gamboy cartidge, loaded from a rom file.
 *
 * Note this structure should not be accessed from outside this file.
 * Any access to information within in the cartidge should be made through
 * the functions provided in cartidge.h.
 */
struct gb_cartidge
{
	char *title;        /**< The title of the cartridge */
	char *license;      /**< The license code of the cartrige. */
	
	int rom_size;       /**< The rom size of the cartridge. */
	int rom_banks;      /**< The number of rom banks in the cartidge. */
	int ram_size;       /**< The ram size of the cartidge. */
	int ram_banks;      /**< The number of ram banks in the cartidge. */
	int rom_version;    /**< The rom version of the cartidge. */
	uint8_t* data;       /**< The data stored in the cartidge, which could read/written */

	bool CGB;           /**< True if made for the gameboy color, false otherwise */
	bool SGB;           /**< True if made for the super gameboy, false otherwise. */
	bool is_japanese;   /**< True if this is a japanese rom. */

	enum rom_type rom;  /**< The rom type. */

	read_data read;     /**< Function to read data from the cartidge, set based on rom type. */
	write_data write;   /**< Function to write data to the cartidge, set based on rom type. */
};

/**
 * Turn the contents of a file into a char array.
 *
 * \param fileName name of the rom file
 * \return char array containing the contents of the file.
 */
static uint8_t* get_file_contents(const char* fileName)
{
	FILE* ifp;
	uint8_t* buffer;
	size_t result;
	int bufferLen;

	ifp = fopen(fileName, "r");
	if(ifp == NULL)
	{
		err = MEM_ERR;
		return NULL;
	}
	
	// get the length of the file.
	fseek(ifp, 0l, SEEK_END);
	bufferLen = ftell(ifp);
	rewind(ifp);
	
	buffer = (uint8_t*) malloc(bufferLen * sizeof(uint8_t));
	if (buffer == NULL)
	{
		err = MEM_ERR;
		return NULL;	
	}
	
	result = fread(buffer, 1, bufferLen, ifp);
	// make sure the entire file was read in.
	if(result != bufferLen)
	{
		free(buffer);
		err = FILE_ERR;
		return NULL;	
	}

	fclose(ifp);
	
	return buffer;
}	

/**
 * Gets the rom size of the cartidge.
 *
 * \param rom_size_code Code stored in cartidge at 0x148, indicating the rom
 *        size and number of rom banks.
 * \param rom_size Pointer to store the rom size into.
 * \param rom_banks Pointer to store the number of rom banks into.
 */
static void get_rom_size(int8_t rom_size_code, int *rom_size, int *rom_banks)
{
	int KB = 1024;
	int MB = 1024 * 1024;
	switch (rom_size_code)
	{
		case 0x00:
			*rom_size = 32 * KB;
			*rom_banks = 0;
			break;
		case 0x01:
			*rom_size = 64 * KB;
			*rom_banks = 4;
			break;
		case 0x02:
			*rom_size = 128 * KB;
			*rom_banks = 8;
			break;
		case 0x03:
			*rom_size = 256 * KB;
			*rom_banks = 16;
			break;
		case 0x04:
			*rom_size = 512 * KB;
			*rom_banks = 32;
			break;
		case 0x05:
			*rom_size = 1 * MB;
			*rom_banks = 64;
			break;
		case 0x06:
			*rom_size = 2 * MB;
			*rom_banks = 128;
			break;
		case 0x07:
			*rom_size = 4 * MB;
			*rom_banks = 8;
			break;
		case 0x52:
			*rom_size = 1179648; // 1.125 MB
			*rom_banks = 72;
			break;
		case 0x53:
			*rom_size = 1310720; // 1.25 MB
			*rom_banks = 80;
			break;
		case 0x54:
			*rom_size = 1572864; // 1.5 MB
			*rom_banks = 96;
			break;
		default:
			*rom_size = -1;
			*rom_banks = -1;
			err = INVALID_ERR; 
	}
}

/**
 * Gets the ram size of the cartidge.
 *
 * \param ram_size_code Code stored in cartidge at 0x149, indicating the ram
 *        size and number of ram banks.
 * \param ram_size Pointer to store the ram size into.
 * \param ram_banks Pointer to store the number of ram banks into.
 */
static void get_ram_size(int8_t ram_size_code, int *ram_size, int *ram_banks)
{
	int KB = 1024;
	switch (ram_size_code)
	{
		case 0x00:
			*ram_size = 0;
			*ram_banks = 0;
			break;
		case 0x01:
			*ram_size = 2 * KB;
			*ram_banks = 1;
			break;
		case 0x02:
			*ram_size = 8 * KB;
			*ram_banks = 1;
			break;
		case 0x03:
			*ram_size = 32 * KB;
			*ram_banks = 4;
			break;
		default:
			*ram_size = -1;
			*ram_banks = -1;
			err = INVALID_ERR;
	}
}

/**
 * Get the title of the game from the file data.
 * 
 * \param file_data contents of the rom file.
 * \return the title of the game.
 */
static char *get_game_title(uint8_t *file_data)
{
	// title location in the data starts a 0x0134 and ends 
	// at 0x0142.
	int start = 0x0134;
	int end = 0x0143;
	char title[end - start];
	int i;
	for(i = start; i < end; i++)
	{
		title[i - start] = file_data[i];
		// null terminator, end of the title.
		if(file_data[i] == 0)
			break;
	}

	// copy the title to the return value.
	char* ret = malloc((i - start) * sizeof(char));
	if(ret == NULL)
	{
		err = MEM_ERR;
		return NULL;
	}
	strcpy(ret, title);

	return ret;
}

/**
 * Gets the license code of game.
 * 
 * \param SGB Flag to see if the game has super gameboy support.
 * \param CGB Flag to see if the game is a gameboy color game.
 * \param file_data Contents of the rom file.
 */
static char *get_license_code(bool SGB, bool CGB, uint8_t *file_data)
{
	char* license;
	if(SGB || CGB)
	{
		// if the game came after the super game boy, the license is two ascii 
		// characters in 0x0144 and 0x0145
		license = (char*)malloc(sizeof(char) * 3);
		if(license == NULL)
		{
			err = MEM_ERR;
			return NULL;
		}
		license[0] = file_data[0x0144];
		license[1] = file_data[0x0145];
		license[2] = 0;
	}
	else
	{
		// if the game came before the super game boy, the license is at 0x014B.
		license = (char*)malloc(sizeof(char) * 2);
		if(license == NULL)
		{
			err = MEM_ERR;
			return NULL;
		}
		license[0] = file_data[0x014B];
		license[1] = 0;
	}
	return license;
}

/**
 * Gets the cartidge read and write based on the rom type.
 * 
 * \param rom Rom Type.
 * \param read function pointer to store the read function.
 * \param write function pointer to store the write function.
 */
static void get_read_write(enum rom_type rom, read_data *read, write_data *write)
{
	switch(rom)
	{
		case ROM:
			*read = ROM_memoryRead;
			*write = ROM_memoryWrite;
			break;
		case MBC1:
			*read = MBC1_memoryRead;
			*write = MBC1_memoryWrite;
			break;
		case MBC2:
			*read = MBC2_memoryRead;
			*write = MBC2_memoryWrite;
			break;
		case MBC3:
			*read = MBC3_memoryRead;
			*write = MBC3_memoryWrite;
			break;
		case MBC5:
			*read = MBC5_memoryRead;
			*write = MBC5_memoryWrite;
			break;
		default:
			break;
	}
}

struct gb_cartidge *load_cartidge(const char *filename)
{
	uint8_t *fileData;
	int romSize, romBanks, ramSize, ramBanks, romVersion;
	bool isCGB, isSGB, isJap;
	enum rom_type romType;
	char *gameTitle;
	char *gameLicense;
	read_data read;
	write_data write;

	fileData = get_file_contents(filename);
	if (fileData == NULL)
	{
		return NULL;
	}

	// ROM size is stored in 0x148
	get_rom_size(fileData[0x148], &romSize, &romBanks);
	// RAM size is stored in 0x149
	get_ram_size(fileData[0x149], &ramSize, &ramBanks);
	if (romSize < 0 || ramSize < 0)
	{
		free(fileData);
		return NULL;
	}
	
	romType = (enum rom_type)fileData[0x147];

	get_read_write(romType, &read, &write);
	
	// gameboy color?
	isCGB = fileData[0x143];
	// Super gameboy?
	isSGB = fileData[0x146];

	romVersion = fileData[0x14C];

	// Only two nationalities for gameboy games, Japanese and not Japanese.
	isJap = (fileData[0x14A] == 0x00);
	
	gameTitle = get_game_title(fileData); 
	if (gameTitle == NULL)
	{
		free(fileData);
		return NULL;
	}
	
	gameLicense = get_license_code(isSGB, isCGB, fileData);
	if (gameLicense == NULL)
	{
		free(gameTitle);
		free(fileData);
		return NULL;
	}

	struct gb_cartidge *cartidge = malloc(sizeof(struct gb_cartidge));
	if (cartidge == NULL)
	{
		free(gameTitle);
		free(gameLicense);
		free(fileData);
		err = MEM_ERR;
		return NULL;
	}
	
	cartidge->data = fileData;
	cartidge->title = gameTitle;
	cartidge->license = gameLicense;
	cartidge->rom_size = romSize;
	cartidge->rom_banks = romBanks;
	cartidge->ram_size = ramSize;
	cartidge->ram_banks = ramBanks;
	cartidge->rom_version = romVersion;
	cartidge->CGB = isCGB;
	cartidge->SGB = isSGB;
	cartidge->is_japanese = isJap;
	cartidge->rom = romType;
	cartidge->read = read;
	cartidge->write = write;

	return cartidge;
}

uint8_t cartidge_read(struct gb_cartidge* cartidge, int memAddr)
{
	return cartidge->read(cartidge->data, memAddr);
}

void cartidge_write(struct gb_cartidge *cartidge, int memAddr, uint8_t write_data)
{
	cartidge->write(cartidge->data, memAddr, write_data);
}

/**
 * Converts a RomType into a string and prints it.
 * \param rom rom type to print.
 */
static void print_rom_type(enum rom_type rom)
{
	char* type;
	switch(rom)
	{
		case ROM:
			type = "ROM";
			break;
		case MBC1:
			type = "MBC1";
			break;
		case MBC1_RAM:
			type = "MBC1_RAM";
			break;
		case MBC1_RAM_BATT:
			type = "MBC1_RAM_BATT";
			break;
		case MBC2:
			type = "MBC2";
			break;
		case MBC2_BATT:
			type = "MBC2_BATT";
			break;
		case ROM_RAM:
			type = "ROM_RAM";
			break;
		case ROM_RAM_BATT:
			type = "ROM_RAM_BATT";
			break;
		case MMM01:
			type = "MMM01";
			break;
		case MMM01_RAM:
			type = "MMM01_RAM";
			break;
		case MMM01_RAM_BATT:
			type = "MMM01_RAM_BATT";
			break;
		case MBC3_TIME_BATT:
			type = "MBC3_TIME_BATT";
			break;
		case MBC3_TIME_RAM_BATT:
			type = "MBC3_TIME_RAM_BATT";
			break;
		case MBC3:
			type = "MBC3";
			break;
		case MBC3_RAM:
			type = "MBC3_RAM";
			break;
		case MBC3_RAM_BATT:
			type = "MBC3_RAM_BATT";
			break;
		case MBC5:
			type = "MBC5";
			break;
		case MBC5_RAM:
			type = "MBC5_RAM";
			break;
		case MBC5_RAM_BATT:
			type = "MBC5_RAM_BATT";
			break;
		case MBC5_RUM:
			type = "MBC5_RUM";
			break;
		case MBC5_RUM_RAM:
			type = "MBC5_RUM_RAM";
			break;
		case MBC5_RUM_RAM_BATT:
			type = "MBC5_RUM_RAM_BATT";
			break;
		case POCKET_CAM:
			type = "POCKET_CAM";
			break;
		case BANDAI_TAMA5:
			type = "BANDAI_TAMA5";
			break;
		case HUC3:
			type = "HUC3";
			break;
		case HUC3_RAM_BATT:
			type = "HUC3_RAM_BATT";
			break;		
	}
	printf("Rom Type:    %s\n", type);
}

void cartidge_print(struct gb_cartidge *cartidge)
{
	printf("Title:       %s\n"
		   "License:     %s\n"
		   "Rom Size:    %d\n"
		   "Rom Banks:   %d\n"
		   "Ram Size:    %d\n"
		   "Ram Banks:   %d\n"
		   "CGB:         %s\n"
		   "SGB:         %s\n"
		   "Japanese:    %s\n"
		   "Rom Version: %d\n",
		   cartidge->title, 
		   cartidge->license,
		   cartidge->rom_size, 
		   cartidge->rom_banks, 
		   cartidge->ram_size,
		   cartidge->ram_banks, 
		   cartidge->CGB ? "true" : "false", 
		   cartidge->SGB ? "true" : "false", 
		   cartidge->is_japanese ? "true" : "false", 
		   cartidge->rom_version);
	print_rom_type(cartidge->rom);
}
