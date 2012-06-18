CC=g++
CFLAGS=-Wall -Werror -g 
LFLAGS=
LDLIBS=

OBJS=Z80InstructionSet.o gameboy.o
HEADERS=Z80InstructionSet.h

all: gameboy

$(OBJS) : $(HEADERS)

gameboy: $(OBJS) $(HEADERS)
	${CC} ${CFLAGS} -o $@ ${LFLAGS} ${OBJS} ${LDLIBS}

clean:
	rm -f *.o gameboy
