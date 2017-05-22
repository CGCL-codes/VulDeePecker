/*PLOVER: MISC.DFREE*/

/*
Description: malloc'd data is freed more than once.
Keywords: Size0 Complex0 Api MemMgmt DoubleFree
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
test(char *str)
{
	char *p;

	p = strdup(str);
	if(p) {
		printf("result: %s\n", p);
		free(p);
		free(p);			/* FLAW */
	}
}

int
main(int argc, char **argv)
{
	char *userstr;

	if(argc > 1) {
		userstr = argv[1];
		test(userstr);
	}
	return 0;
}

