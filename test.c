#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "cartidge.h"

/**
 * Reads a rom file and prints info about the game.
 */
int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		printf("Usage: test <rom file>\n");
		exit(EXIT_FAILURE);
	}

	Cartidge* game = NULL;
	
	GAME_ERROR err = loadCartidge(&game, argv[1]);
	if(err != NO_ERR)
	{
		printf("Error: could not load game\n");
		exit(EXIT_FAILURE);
	}

	printGameInfo(game);

	destroyGame(game);

	int r = -1;
	uint8_t tes = r;
	tes += 1;
	printf("%d\n", tes);

	return 0;
}

