#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "Memory/CartridgeHeader.h"
#include "Memory/MemoryDefs.h"
#include "Memory/Memory.h"
#include "Memory/MemoryLoader.h"

static void usage()
{
	fprintf(stderr, "Usage: gameboy <rom file>\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		usage();
	}
    
    Memory* mem = MemoryLoader::loadCartridge( argv[1] );

    std::cout << mem->header->desc << std::endl;

	return 0;
}
