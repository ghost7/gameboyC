cmake_minimum_required(VERSION 2.6)

project(Gameboy)
enable_testing()

set(SRCS src/gameboy.cpp
         src/Common/FileUtils.h
         src/Common/FileUtils.cpp
         src/Cpu/CpuBase.h
         src/Cpu/Z80.h
         src/Cpu/Z80Cpu.h
         src/Cpu/Z80Cpu.cpp
         src/Cpu/Z80InstructionSet.h
         src/Cpu/Z80InstructionSet.cpp
         src/Memory/CartridgeHeader.h
         src/Memory/CartridgeHeader.cpp
         src/Memory/MemoryLoader.h
         src/Memory/MemoryLoader.cpp
         #src/Memory/MemoryBase.h  
         src/Memory/Memory.h         
         src/Memory/MemoryInterface.h 
         src/Memory/Memory.cpp    
         src/Memory/MemoryDefs.h
         src/Memory/MemoryCustomizer.h
         src/Memory/MemoryCustomizer.cpp
         src/Memory/Customizers/BasicMemory.cpp
         src/Memory/Customizers/BasicMemory.h
         src/Memory/Customizers/DefaultERam.cpp
         src/Memory/Customizers/DefaultERam.h
         src/Memory/Customizers/DefaultRom.cpp
         src/Memory/Customizers/DefaultRom.h
         src/Memory/Customizers/EchoRam.cpp
         src/Memory/Customizers/EchoRam.h
         src/Memory/Customizers/IOMemory.cpp
         src/Memory/Customizers/IOMemory.h
         src/Memory/Customizers/LazyMemory.h
         src/Memory/Customizers/VRam.cpp
         src/Memory/Customizers/VRam.h
    )

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall")

add_executable(gameboy ${SRCS})

add_subdirectory(tests)
