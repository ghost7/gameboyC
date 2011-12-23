CC=gcc
CFLAGS=-Wall -c -g -DDEBUG
LFLAGS=-Wall -g
COMPILE=${CC} ${CFLAGS}
OBJS=test.o cartidge.o mbc.o memory.o

test: ${OBJS}
	${CC} ${LFLAGS} ${OBJS} -o test

test.o: cartidge.o test.c
	${COMPILE} test.c

cartidge.o: mbc.o cartidge.c
	${COMPILE} cartidge.c

cpu_z80.o: cpu_z80_inst.o memory.o cpu_z80.c registers.h
	${COMPILE} cpu_z80.c

cpu_z80_inst.o: memory.o registers.h cpu_z80_inst.c
	${COMPILE} cpu_z80_inst.c

mbc.o: mbc.c
	${COMPILE} mbc.c

memory.o: mbc.o memory.c 
	${COMPILE} memory.c

clean:
	\rm *.o
