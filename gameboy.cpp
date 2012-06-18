#include <stdlib.h>
#include <stdio.h>

#include "cartidge.h"

static void usage()
{
	fprintf(stderr, "Usage: gameboy <rom file>\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		usage();
	}

	return 0;
}
