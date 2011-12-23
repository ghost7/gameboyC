#ifndef _GB_GAME_H
#define _GB_GAME_H

#include <stdbool.h>
#include <stdint.h>

/**
 * Possible errors produced by the game.
 */
typedef enum GAME_ERROR
{
	NO_ERR,
	H_CHECK_SUM_ERR,
	G_CHECK_SUM_ERR,
	MEM_ERR,
	FILE_ERR
} GAME_ERROR;

/**
 * Cartidge type. Specifies which Memory Bank Controller (if any) is used in 
 * the cartidge, and specifies any external hardware that exists in the cartidge.
 */
typedef enum RomType
{
	ROM =					0x00,
	MBC1 =					0x01,
	MBC1_RAM =				0x02,
	MBC1_RAM_BATT =			0x03,
	MBC2 =					0x05,
	MBC2_BATT =				0x06,
	ROM_RAM =				0x08,
	ROM_RAM_BATT =			0x09,
	MMM01 =					0x0B,
	MMM01_RAM =				0x0C,
	MMM01_RAM_BATT =		0x0D,
	MBC3_TIME_BATT =		0x0F,
	MBC3_TIME_RAM_BATT =	0x10,
	MBC3 =					0x11,
	MBC3_RAM =				0x12,
	MBC3_RAM_BATT =			0x13,
//	MBC4 =					0x15,
//	MBC4_RAM = 				0x16,
//	MBC4_RAM_BATT =			0x17,
	MBC5 =					0x19,
	MBC5_RAM =				0x1A,
	MBC5_RAM_BATT =			0x1B,
	MBC5_RUM =				0x1C,
	MBC5_RUM_RAM =			0x1D,
	MBC5_RUM_RAM_BATT =		0x1E,
	POCKET_CAM =			0xFC,
	BANDAI_TAMA5 =			0xFD,
	HUC3 =					0xFE,
	HUC3_RAM_BATT =			0xFF
} RomType;

/**
 * Contains information about the game.
 */
typedef struct Cartidge
{
	char* title;
	bool CGB;
	char* license;
	bool SGB;
	int romSize;
	int romBanks;
	int ramSize;
	int ramBanks;
	bool japanese;
	int romVersion;
	int8_t* romData;
	RomType romType;
} Cartidge;

/**
 * Loads a game.
 * \param gameP Pointer to the Game pointer to load the game into
 * \param fileName name of the rom file.
 * \return error if any occurs.
 */
GAME_ERROR loadCartidge(Cartidge** game, char* fileName);


/**
 * Prints information about a game.
 * \param game game to print.
 */
void printGameInfo(Cartidge* game);

/**
 * Converts a RomType into a string and prints it.
 * \param romType rom type to print.
 */
void printRomType(RomType romType);

/**
 * Free all memory allocated to a game.
 * \param game game to be freed.
 */
void destroyGame(Cartidge* game);

#endif
