cmake_minimum_required(VERSION 2.6)

project(Gameboy)

set(SRCS src/gameboy.cpp
         src/Common/FileUtils.h
         src/Common/FileUtils.cpp
         src/Cpu/CpuBase.h
         src/Cpu/Z80.h
         src/Cpu/Z80Cpu.h
         src/Cpu/Z80InstructionSet.h
         src/Cpu/Z80InstructionSet.cpp
         src/Cartridge/CartridgeHeader.h
         src/Cartridge/CartridgeHeader.cpp
         src/Cartridge/CartridgeBase.h
         src/Cartridge/CartridgeLoader.h
         src/Cartridge/CartridgeLoader.cpp
         src/Cartridge/RomCartridge.h
         src/Cartridge/RomCartridge.cpp
         src/Memory/mbc1Memory.cpp  
         src/Memory/mbc2Memory.cpp  
         src/Memory/mbc3Memory.cpp  
         src/Memory/mbc5Memory.cpp  
         src/Memory/MemoryBase.h  
         src/Memory/memory.h         
         src/Memory/memoryInterface.h 
         src/Memory/mbc1Memory.h    
         src/Memory/mbc2Memory.h    
         src/Memory/mbc3Memory.h    
         src/Memory/mbc5Memory.h    
         src/Memory/memory.cpp    
         src/Memory/memoryHeaders.h
    )

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall")

add_executable(gameboy ${SRCS})
