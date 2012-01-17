CC=gcc
CFLAGS=-Wall -c -g 
LFLAGS=-Wall -g -lSDL  
COMPILE=${CC} ${CFLAGS}
OBJS=test.o cartidge.o mbc.o memory.o cpu_z80.o cpu_z80_inst.o gpu_gb.o debug.o

test: ${OBJS}
	${CC} ${LFLAGS} ${OBJS} -o test

test.o: cartidge.o cpu_z80.o gpu_gb.o test.c
	${COMPILE} test.c

cartidge.o: mbc.o cartidge.c
	${COMPILE} cartidge.c

cpu_z80.o: cpu_z80_inst.o memory.o cpu_z80.c registers.h cpu_z80.h
	${COMPILE} cpu_z80.c

cpu_z80_inst.o: memory.o registers.h cpu_z80_inst.c
	${COMPILE} cpu_z80_inst.c

mbc.o: mbc.c
	${COMPILE} mbc.c

memory.o: mbc.o memory.h memory.c 
	${COMPILE} memory.c

gpu_gb.o: memory.o gpu_gb.c
	${COMPILE} gpu_gb.c

debug.o: debug.c
	${COMPILE} debug.c

clean:
	\rm *.o
