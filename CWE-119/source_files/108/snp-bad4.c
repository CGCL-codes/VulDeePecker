/*PLOVER: BUFF.OVER, BUFF.LENCALC*/

/*
Description: snprintf is used repeatedly while keeping track of the residual buffer length, however an accounting error allows a stack buffer to be overrun.
Keywords: Size0 Complex1 BufferOverflow Stack AdHoc IntOverflow
ValidArg: "%s %s" % ("a"*15, "b"*15)
ValidArg: "%s %s" % ("a"*15, "b"*45)
InvalidArg: "%s %s" % ("a"*45, "b"*45)
*/

#include <stdio.h>
#include <string.h>

#define	MAXSIZE		40
void
test(char *str1, char *str2)
{
	char buf[MAXSIZE];
	char *p;
	int l, x;

	p = buf;
	l = sizeof buf;

	snprintf(p, l, "<%s>", str1);
	x = strlen(p);
	p += x;
	l -= x;

	/* there's no check on the length here */
	*p++ = ' ';					/* BAD */
	*p++ = '-';					/* BAD */
	l -= 2;						/* BAD */

	/* now length may be negative */
	snprintf(p, l, "<%s>\n", str2);			/* BAD */
	x = strlen(p);
	p += x;
	l -= x;

	printf("result: %s\n", buf);
}

int
main(int argc, char **argv)
{
	char *userstr, *userstr2;

	if(argc > 2) {
		userstr = argv[1];
		userstr2 = argv[2];
		test(userstr, userstr2);
	}
	return 0;
}

