CC=gcc
CFLAGS=-Wall -Werror -g 
CPPFLAGS=-Wall -Werror -g
LFLAGS=
LDLIBS=

OBJS=mbc.o cartridge.o cpu_z80_inst.o cpu_z80.o memory.o gameboy.o 
HEADERS=mbc.h cartridge.h cpu_z80_inst.h cpu_z80.h memory.h

all: gameboy

$(OBJS) : $(HEADERS)

gameboy: $(OBJS) $(HEADERS)
	${CC} ${CFLAGS} -o $@ ${LFLAGS} ${OBJS} ${LDLIBS}

clean:
	rm -f *.o gameboy
