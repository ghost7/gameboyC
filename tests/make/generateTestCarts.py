#!/usr/local/bin/python

# add a tuple to this dictionary for each cartridge to generate
# ( 0: MBC type (0x0147), 1: ROM size (0x0148), 2: RAM size (0x0149) )
testCarts = ( 
    # no MBC
    #( 0x00, 0x00, 0x00 ), 
    #( 0x08, 0x00, 0x00 ),
    #( 0x09, 0x00, 0x00 ),
    #( 0x09, 0x00, 0x01 ),
    #( 0x09, 0x00, 0x02 )
    # MBC1
    ( 0x01, 0x03, 0x02 )
    ,)

KB_SIZE = 0x400
ROM_BANK_SIZE = 16*KB_SIZE
# mappings for the gb headers used in makeCartridge
ramMap = { 0x00: 0,
           0x01: 2*KB_SIZE,
           0x02: 8*KB_SIZE,
           0x03: 32*KB_SIZE }

romMap = { 0x00: 32*KB_SIZE,
           0x01: 64*KB_SIZE,
           0x02: 128*KB_SIZE,
           0x03: 256*KB_SIZE,
           0x04: 512*KB_SIZE,
           0x05: 1024*KB_SIZE,
           0x06: 2048*KB_SIZE,
           0x07: 4096*KB_SIZE,
           0x52: 1152*KB_SIZE,
           0x53: 1280*KB_SIZE,
           0x54: 1536*KB_SIZE }

#only currently supported types are used
mbcTypes = {  0x00: 'ROM_ONLY',
              0x01: 'MBC1',
              0x02: 'MBC1+RAM',
              0x03: 'MBC1+RAM+BATTERY',
              0x05: 'MBC2',
              0x06: 'MBC2+BATTERY',
              0x08: 'ROM+RAM',            
              0x09: 'ROM+RAM+BATTERY',   
              #0x0B: 'MMM01',            
              #0x0C: 'MMM01+RAM',       
              #0x0D: 'MMM01+RAM+BATTERY',       
              0x0F: 'MBC3+TIMER+BATTERY',     
              0x10: 'MBC3+TIMER+RAM+BATTERY',
              0x11: 'MBC3',                 
              0x12: 'MBC3+RAM',
              0x13: 'MBC3+RAM+BATTERY',
              #0x15: 'MBC4',
              #0x16: 'MBC4+RAM',
              #0x17: 'MBC4+RAM+BATTERY',
              0x19: 'MBC5',
              0x1A: 'MBC5+RAM',
              0x1B: 'MBC5+RAM+BATTERY',
              0x1C: 'MBC5+RUMBLE',
              0x1D: 'MBC5+RUMBLE+RAM',
              0x1E: 'MBC5+RUMBLE+RAM+BATTERY',
              #0xFC: 'POCKET CAMERA',
              #0xFD: 'BANDAI TAMA5',
              #0xFE: 'HuC3',
              #0xFF: 'HuC1+RAM+BATTERY'
           }


'''
Takes a Memory Bank Controller type, ROM Size, and RAM Size
to generate a gameboy cartridge file for testing.

Each byte of the cartridge file will contain the number of the 
ROM bank in which it resides, except for the bytes at 
locations 0x0147, 0x0148, and 0x0149.
These locations will contain the header values given as
parameters (the parameters are the mappings used in the gameboy
header, not the actual sizes).

returns the name of the file created
'''
def makeCartridge( mbcType, rom, ram ):
    ramSize = ramMap[ram]
    romSize = romMap[rom]
    fname = '{0}+{1:0=#6x}KB_ROM+{2:0=#6x}KB_RAM.gb'.format( mbcTypes[mbcType], romSize/KB_SIZE, ramSize/KB_SIZE )

    # write the bank number to each bank
    cart = open( fname, 'wb' )
    for i in range(romSize/ROM_BANK_SIZE): 
        cart.write( chr(i)*ROM_BANK_SIZE )

    # replace the header values
    cart.seek( 0x0147 )
    cart.write( chr(mbcType) )
    cart.seek( 0x0148 )
    cart.write( chr(rom) )
    cart.seek( 0x0149 )
    cart.write( chr(ram) )

    cart.close()

    return fname


if __name__ == "__main__":
    
    for cart in testCarts:
        print makeCartridge( cart[0], cart[1], cart[2] )
