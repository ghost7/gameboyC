#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "Common/FileUtils.h"
#include "Cartridge/CartridgeHeader.h"

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
    
    uint8_t *rawRom = readFileToBuffer(argv[1]);
    CartridgeHeader header(rawRom);

    std::cout << header.desc << std::endl;

	return 0;
}
