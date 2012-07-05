set(SRCS gameboy.cpp
         Common/FileUtils.h
         Common/FileUtils.cpp
         Cpu/CpuBase.h
         Cpu/Z80.h
         Cpu/Z80Cpu.h
         Cpu/Z80Cpu.cpp
         Cpu/Z80InstructionSet.h
         Cpu/Z80InstructionSet.cpp
         Memory/CartridgeHeader.h
         Memory/CartridgeHeader.cpp
         Memory/MemoryLoader.h
         Memory/MemoryLoader.cpp
         #Memory/MemoryBase.h  
         Memory/Memory.h         
         Memory/MemoryInterface.h 
         Memory/Memory.cpp    
         Memory/MemoryDefs.h
         Memory/MemoryCustomizer.h
         Memory/MemoryCustomizer.cpp
         Memory/Customizers/BasicMemory.cpp
         Memory/Customizers/BasicMemory.h
         Memory/Customizers/DefaultERam.cpp
         Memory/Customizers/DefaultERam.h
         Memory/Customizers/DefaultRom.cpp
         Memory/Customizers/DefaultRom.h
         Memory/Customizers/EchoRam.cpp
         Memory/Customizers/EchoRam.h
         Memory/Customizers/IOMemory.cpp
         Memory/Customizers/IOMemory.h
         Memory/Customizers/LazyMemory.h
         Memory/Customizers/VRam.cpp
         Memory/Customizers/VRam.h
         Window/GBSDLWindow.cpp
         Window/GBSDLWindow.h
         Window/GBWindow.h
    )

add_executable(gameboy ${SRCS})

find_package(SDL)
if (NOT SDL_FOUND)
    message(FATAL_ERROR "SDL not found")
endif (NOT SDL_FOUND)

include_directories(
    ${SDL_INCLUDE_DIR}
    ${INCLUDE_DIRECTORIES}
    )
  target_link_libraries(gameboy
    ${SDL_LIBRARY}
    ${TARGET_LINK_LIBRARIES}
    )
