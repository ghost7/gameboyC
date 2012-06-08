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

	struct gb_cartidge *cartidge;
	cartidge = load_cartidge(argv[1]);
	
	if (cartidge == NULL)
	{
		fprintf(stderr, "Could not read rom\n");
		exit(EXIT_FAILURE);
	}

	cartidge_print(cartidge);

	return 0;
}
