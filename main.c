#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stack.h"
#include "ls2.h"

/**
 * Main function
 * Usage: ls2 <path> [exact-match-pattern]
 */
int main(int argc, char *argv[])
{

	char *pathname;
	int mode = parseMode(argc);
	stack_t *s = initstack(); // stack stores the lines to print out

	if (mode == 1)
	{
		pathname = (char *)malloc(strlen(argv[1]) + 1);
		strcpy(pathname, argv[1]);
		mode1(pathname, 0);
	}
	else
	{
		pathname = (char *)malloc(strlen(argv[1]) + 1);
		strcpy(pathname, argv[1]);
		//char *pattern = (char *)malloc(strlen(argv[2])+1);
		mode2(pathname, s, 0, argv[2]);
	}


	// print stack
	printstack(s);

	// free up stack
	freestack(s);
	free(pathname);
	
		return 0;
}
