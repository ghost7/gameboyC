#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "Cpu/Z80Cpu.h"

#include "Memory/CartridgeHeader.h"
#include "Memory/MemoryDefs.h"
#include "Memory/Memory.h"
#include "Memory/MemoryLoader.h"

#include "Window/GBWindow.h"
#include "Window/GBSDLWindow.h"
#include <stdint.h>

#define MEM_SIZE 0xFFFF

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
    
    // Create Game Boy memory
    Memory* mem = MemoryLoader::loadCartridge( argv[1] );
     
    std::cout << mem->header->desc << std::endl;
    
    // Create Game Boy CPU.
    CpuBase *cpu = new Z80Cpu(mem);
    cpu->init();
    
    // Create the window.
    GBWindow *window = new GBSDLWindow();
    if (!window->init(cpu))
    {
        std::cerr << window->getErrorMessage() << std::endl;
    }

    // Game Boy Loop.
    window->loop();

    // Free resources.
    delete window;
    delete mem;
    delete cpu;

	return 0;
}
