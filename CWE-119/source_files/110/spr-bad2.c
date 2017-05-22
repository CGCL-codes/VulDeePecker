/*PLOVER: BUFF.OVER, BUF.LENCALC*/

/*
Description: A sprintf based on a guarded format string miscalculates the size and allows a stack buffer to be overrun.
Keywords: Size0 Complex0 BufferOverflow Stack Sprintf BadBound
ValidArg: "a"*20
InvalidArg: "a"*100

This test case may not fail on Invalid input on many compilers
due to additional padding between the buffers.
*/

#include <stdio.h>
#include <string.h>

/*
 * Only a few characters overrun, so we pick a round sized buffer
 * in hopes that the compiler doesn't add any extra padding and
 * inadvertantly provide enough room to hold the extra characters.
 */
#define	MAXSIZE		5

void
test(char *str)
{
	char buf[MAXSIZE];

	/* 32 + "<>" + NUL = 35 */
	sprintf(buf, "<%.5s>", str);		/* BAD */
	printf("result: %s\n", buf);
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

