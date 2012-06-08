CC=gcc
CFLAGS=-Wall -Werror -g 
LFLAGS=
LDLIBS=

OBJS=mbc.o cartidge.o cpu_z80_inst.o cpu_z80.o gameboy.o
HEADERS=mbc.h cartidge.h cpu_z80_inst.h cpu_z80.h

all: gameboy

$(OBJS) : $(HEADERS)

gameboy: $(OBJS) $(HEADERS)
	${CC} ${CFLAGS} -o $@ ${LFLAGS} ${OBJS} ${LDLIBS}

clean:
	rm -f *.o gameboy
