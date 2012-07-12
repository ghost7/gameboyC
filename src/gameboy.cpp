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

class MemoryEmulator : public MemoryInterface
{
public:
    uint8_t read(addr_t memAddr)
    {
        return memory[memAddr];
    }
    void write(addr_t memAddr, data_t data)
    {
        memory[memAddr] = data;
    }
    data_t *getMemoryLocation(addr_t addr)
    {
        return &memory[addr];
    }

    void clearMemory()
    {
        memset(memory, 0, MEM_SIZE);
    }
    MemoryEmulator()
    {
        clearMemory();
    }
private:
    data_t memory[MEM_SIZE];
};

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
    CpuBase *cpu = new Z80Cpu(new MemoryEmulator());
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
