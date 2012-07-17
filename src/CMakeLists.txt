set(SRCS gameboy.cpp
         Common/Color.cpp
         Common/Color.h
         Common/Config.h
         Common/Config.cpp
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
         Memory/Customizers/DmgBoot.cpp
         Memory/Customizers/DmgBoot.h
         Memory/Customizers/EchoRam.cpp
         Memory/Customizers/EchoRam.h
         Memory/Customizers/IOMemory.cpp
         Memory/Customizers/IOMemory.h
         Memory/Customizers/LazyMemory.h
         Memory/Customizers/VRam.cpp
         Memory/Customizers/VRam.h
         Lcd/Lcd.cpp
         Lcd/Lcd.h
         Lcd/LcdBackground.cpp
         Lcd/LcdBackground.h
         Lcd/LcdComponent.cpp
         Lcd/LcdComponent.h
         Lcd/LcdInterface.h
         Lcd/LcdSprites.cpp
         Lcd/LcdSprites.h
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

# These source groups are here just to make the file structure in Visual Studio
# look more organized. They don't affect the build process.
source_group(Cpu  
             FILES
             Cpu/CpuBase.h
             Cpu/Z80.h
             Cpu/Z80Cpu.h
             Cpu/Z80Cpu.cpp
             Cpu/Z80InstructionSet.h
             Cpu/Z80InstructionSet.cpp
            )
            
source_group(Memory
             FILES
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
            )

source_group(Memory\\Customizers
             FILES
             Memory/Customizers/BasicMemory.cpp
             Memory/Customizers/BasicMemory.h
             Memory/Customizers/DefaultERam.cpp
             Memory/Customizers/DefaultERam.h
             Memory/Customizers/DefaultRom.cpp
             Memory/Customizers/DefaultRom.h
             Memory/Customizers/DmgBoot.cpp
             Memory/Customizers/DmgBoot.h
             Memory/Customizers/EchoRam.cpp
             Memory/Customizers/EchoRam.h
             Memory/Customizers/IOMemory.cpp
             Memory/Customizers/IOMemory.h
             Memory/Customizers/LazyMemory.h
             Memory/Customizers/VRam.cpp
             Memory/Customizers/VRam.h
             )
            
source_group(Window
             FILES
             Window/GBSDLWindow.cpp
             Window/GBSDLWindow.h
             Window/GBWindow.h
            )

source_group(Lcd
             FILES
             Lcd/Lcd.cpp
             Lcd/Lcd.h
             Lcd/LcdBackground.cpp
             Lcd/LcdBackground.h
             Lcd/LcdComponent.cpp
             Lcd/LcdComponent.h
             Lcd/LcdInterface.h
             Lcd/LcdSprites.cpp
             Lcd/LcdSprites.h
            )

source_group(Common
             FILES
             Common/Color.cpp
             Common/Color.h
             Common/Config.h
             Common/Config.cpp
             Common/FileUtils.h
             Common/FileUtils.cpp
            )
