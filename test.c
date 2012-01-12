#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <SDL/SDL.h>
#include "cartidge.h"
#include "cpu_z80.h"
#include "memory.h"
#include "gpu_gb.h"

int running = 1;

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

	initCpu();
	initMemory(game);
	
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("SDL could not initilize\n");
		return 1;
	}


	SDL_Surface *screen = SDL_SetVideoMode(160, 144, 32, SDL_HWSURFACE);
	if(!screen) {
		SDL_Quit();
		fprintf(stderr, "SDL_SetVideoMode failed\n");
		return 1;
	}
	
	//gpuInit(screen->pixels);
	///SDL_Flip(screen);

	SDL_Event event;
	
	while(running)
	{
		int ticks = cpuStep();
		printRegisters();
		//ticks += checkInterrupts();
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
				running = 0;
		}
		gpuStep(ticks);
		//if(SDL_Flip(screen) == -1)
		//	return 1;
	}

	destroyGame(game);


	return 0;
}

