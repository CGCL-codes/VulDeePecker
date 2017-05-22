/*PLOVER: BUFF.OVER, BUFF.FORMAT*/

/*
Description: Printf is called with a user supplied format string.
Keywords: Size0 Complex0 Taint FormatString
ValidArg: "'NormalString\n'"
InvalidArg: "%s"*100
*/

#include <stdio.h>

void
test(char *str)
{
	printf(str);				/* FLAW */
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

