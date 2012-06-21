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
    )

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall")

add_executable(gameboy ${SRCS})
