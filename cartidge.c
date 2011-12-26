#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cartidge.h"

static int8_t* getFileContents(char* fileName);
static char* getGameTitle(int8_t* fileData);
static char* getLicenseCode(bool SGB, bool CGB, int8_t* fileData);
static GAME_ERROR verifyCheckSums(int8_t* fileData, int romSize);

/**
 * Loads a game.
 * \param gameP Pointer to the Game pointer to load the game into
 * \param fileName name of the rom file.
 * \return error if any occurs.
 */
GAME_ERROR loadCartidge(Cartidge** gameP, char* fileName)
{
	int8_t* fileData;
	*gameP = (Cartidge*)malloc(sizeof(Cartidge));
	Cartidge* game = *gameP;
	if(game == NULL)
	{
		return MEM_ERR;
	}
	
	// convert the file into an array of bytes.
	fileData = getFileContents(fileName);
	if(fileData == NULL)
	{
#ifdef DEBUG
		printf("Error accessing file contents\n");
#endif
		free(game);
		game = NULL;
		return FILE_ERR;
	}

	int KB = 1024;
	int MB = 1048576;
	// ROM size is stored in 0x0148
	switch(fileData[0x0148])
	{
		case 0x00:
			game->romSize = 32 * KB;
			game->ramBanks = 0;
			break;
		case 0x01:
			game->romSize = 64 * KB;
			game->romBanks = 4;
			break;
		case 0x02:
			game->romSize = 128 * KB;
			game->romBanks = 8;
			break;
		case 0x03:
			game->romSize = 256 * KB;
			game->romBanks = 16;
			break;
		case 0x04:
			game->romSize = 512 * KB;
			game->romBanks = 32;
			break;
		case 0x05:
			game->romSize = 1 * MB;
			game->romBanks = 64;
			break;
		case 0x06:
			game->romSize = 2 * MB;
			game->romBanks = 128;
			break;
		case 0x07:
			game->romSize = 4 * MB;
			game->romBanks = 8;
			break;
		case 0x52:
			game->romSize = 1179648; // 1.125 MB
			game->romBanks = 72;
			break;
		case 0x53:
			game->romSize = 1310720; // 1.25 MB
			game->romBanks = 80;
			break;
		case 0x054:
			game->romSize = 1572864; // 1.5 MB
			game->romBanks = 96;
			break;
	}

	// now that we have the rom size, we can verify the check sums
	GAME_ERROR checkSums = verifyCheckSums(fileData, game->romSize);
	if(checkSums != NO_ERR)
	{
		free(game);
		free(fileData);
		game = NULL;
		return checkSums;
	}

	// RAM size is stored in 0x0149
	switch(fileData[0x0149])
	{
		case 0x00:
			game->ramSize = 0;
			game->ramBanks = 0;
			break;
		case 0x01:
			game->ramSize = 2 * KB;
			game->ramBanks = 1;
			break;
		case 0x02:
			game->ramSize = 8 * KB;
			game->ramBanks = 1;
			break;
		case 0x03:
			game->ramSize = 32 * KB;
			game->ramBanks = 4;
			break;
	}

	// extract the game title
	game->title = getGameTitle(fileData);
	if(game->title == NULL)
	{
#ifdef DEBUG
		printf("Error reading game title\n");
#endif
		free(game);
		free(fileData);
		game = NULL;
		return MEM_ERR;
	}
	
	// gameboy color?
	game->CGB = fileData[0x0143];
	// super gameboy?
	game->SGB = fileData[0x0146];
	
	// get the license code
	game->license = getLicenseCode(game->SGB, game->CGB, fileData);
	if(game->license == NULL)
	{
#ifdef DEBUG
		printf("Error reading game license\n");
#endif
		free(game->title);
		free(game);
		free(fileData);
		game = NULL;
		return MEM_ERR;
	}

	game->romType = (RomType)fileData[0x0147];

	// Only two nationalities for gameboy games, Japanese and not Japanese.
	game->japanese = (fileData[0x014A] == 0x00);

	game->romVersion = fileData[0x014C];

	game->romData = fileData;

	return NO_ERR;
}

/**
 * Prints information about a game.
 * \param game game to print.
 */
void printGameInfo(Cartidge* game)
{
	printf(
	"Title:\t\t%s\nCGB:\t\t%d\nSGB:\t\t%d\nlicense:\t%s\nromSize:\t%d\nromBanks:\t%d\nramSize:\t%d\nramBanks:\t%d\njapanese:\t%d\nromVersion:\t%d\n",
	game->title,
	game->CGB,
	game->SGB,
	game->license, 
	game->romSize,
	game->romBanks,
	game->ramSize,
	game->ramBanks,
	game->japanese,
	game->romVersion);
	printRomType(game->romType);
}

/**
 * Converts a RomType into a string and prints it.
 * \param romType rom type to print.
 */
void printRomType(RomType romType)
{
	char* type;
	switch(romType)
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
//		case MBC4:
//			type = "MBC4";
//			break;
//		case MBC4_RAM:
//			type = "MBC4_RAM";
//			break;
//		case MBC4_RAM_BATT:
//			type = "MBC4_RAM_BATT";
//			break;
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
	printf("ROM:\t\t%s\n", type);

}

/**
 * Free all memory allocated to a game.
 * \param game game to be freed.
 */
void destroyGame(Cartidge* game)
{
	free(game->title);
	free(game->license);
	free(game->romData);
	free(game);
	game = NULL;
}

/**
 * Turn the contents of a file into a char array.
 * \param fileName name of the rom file
 * \return char array containing the contents of the file.
 */
static int8_t* getFileContents(char* fileName)
{
	FILE* ifp;
	int8_t* buffer;
	size_t result;
	int bufferLen;

	ifp = fopen(fileName, "r");
	if(ifp == NULL)
	{
		return NULL;
	}
	
	// get the length of the file.
	fseek(ifp, 0l, SEEK_END);
	bufferLen = ftell(ifp);
	rewind(ifp);
	
	buffer = (int8_t*) malloc(bufferLen * sizeof(int8_t));
	if (buffer == NULL)
	{
		return NULL;	
	}
	
	result = fread(buffer, 1, bufferLen, ifp);
	// make sure the entire file was read in.
	if(result != bufferLen)
	{
		free(buffer);
		return NULL;	
	}

	fclose(ifp);

	return buffer;
}

/**
 * Get the title of the game from the file data.
 * \param fileData contents of the rom file.
 * \return the title of the game.
 */
static char* getGameTitle(int8_t* fileData)
{
	// title location in the data starts a 0x0134 and ends 
	// at 0x0142.
	int start = 0x0134;
	int end = 0x0143;
	char title[end - start];
	int i;
	for(i = start; i < end; i++)
	{
		title[i - start] = fileData[i];
		// null terminator, end of the title.
		if(fileData[i] == 0)
			break;
	}

	// copy the title to the return value.
	char* ret = malloc((i - start) * sizeof(char));
	if(ret == NULL)
	{
		return NULL;
	}
	strcpy(ret, title);

	return ret;
}

/**
 * Gets the license code of game.
 * \param SGB flag to see if the game has super gameboy support.
 * \param CGB flad to see if the game is a gameboy color game.
 * \param fileData contents of the rom file.
 */
static char* getLicenseCode(bool SGB, bool CGB, int8_t* fileData)
{
	char* license;
	if(SGB || CGB)
	{
		// if the game came after the super game boy, the license is two ascii 
		// characters in 0x0144 and 0x0145
		license = (char*)malloc(sizeof(char) * 3);
		if(license == NULL)
		{
			return NULL;
		}
		license[0] = fileData[0x0144];
		license[1] = fileData[0x0145];
		license[2] = 0;
	}
	else
	{
		// if the game came before the super game boy, the license is at 0x014B.
		license = (char*)malloc(sizeof(char) * 2);
		if(license == NULL)
		{
			return NULL;
		}
		license[0] = fileData[0x014B];
		license[1] = 0;
	}
	return license;
}

/**
 * Verifies the header and global check sums
 * \param fileData the contents of the rom.
 * \param size of the rom.
 * \param error if any occurs. 
 */
static GAME_ERROR verifyCheckSums(int8_t* fileData, int romSize)
{
	// first check the header check sum, 1-byte value stored at 0x014D
	char hCheckSum = fileData[0x014D];
	char headerSum = 0;
	int i;
	// get the real header check sum, sum of the bytes from 0x0134 to 0x014C
	for(i = 0x0134; i <= 0x014C; i++)
	{
		headerSum = headerSum - fileData[i] - 1;
	}
	// check if the header sum check sums match.
	if(hCheckSum != headerSum)
	{
#ifdef DEBUG
		printf("Invalid header check sum. Should be %d but is %d.\n", 
			headerSum, hCheckSum);
#endif
		return H_CHECK_SUM_ERR;
	}
	
	// next check the global check sum, 16-bit value. Upper byte is at location
	// 0x014E and lower byte is at location 0x014F.
	short gCheckSum = ((fileData[0x014E] & 0xFF) << 8) | (fileData[0x014F] & 0xFF);
	// Get the real global sum, the sum of all bytes in the rom, excluding
	// 0x014E and 0x014F.
	short globalSum = 0;
	for(i = 0; i < romSize; i++)
	{
		if(i != 0x014E && i != 0x014F)
			globalSum += (fileData[i] & 0xFF);
	}
	// check if the global sums match.
	if(gCheckSum != globalSum)
	{
#ifdef DEBUG
		printf("Invalid Global check sum. Should be %d but is %d.\n", 
			globalSum, gCheckSum);
#endif
		//return G_CHECK_SUM_ERR;
	}
	
	return NO_ERR;
}
