/*
Description: A strcpy is used to copy a string into a stack buffer.  The string is shortened first but a buffer overflow is still allowed.
Fixed: The string is shortened with a recursive function.
Keywords: Port C Size0 Complex2 BufferOverflow Stack Strcpy BadBound
ValidArg: "a"*30
InvalidArg: "a"*100

Copyright 2005 Fortify Software.

Permission is hereby granted, without written agreement or royalty fee, to
use, copy, modify, and distribute this software and its documentation for
any purpose, provided that the above copyright notice and the following
three paragraphs appear in all copies of this software.

IN NO EVENT SHALL FORTIFY SOFTWARE BE LIABLE TO ANY PARTY FOR DIRECT,
INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE
USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF FORTIFY SOFTWARE HAS
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMANGE.

FORTIFY SOFTWARE SPECIFICALLY DISCLAIMS ANY WARRANTIES INCLUDING, BUT NOT
LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE, AND NON-INFRINGEMENT.

THE SOFTWARE IS PROVIDED ON AN "AS-IS" BASIS AND FORTIFY SOFTWARE HAS NO
OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
MODIFICATIONS.
*/

#include <stdio.h>
#include <string.h>

#define	MAXSIZE		40

char *
shortstr(char *p, int n, int targ)
{
	if(n > targ)
		return shortstr(p+1, n-1, targ);
	return p;
}

void
test(char *str)
{
	char buf[MAXSIZE], *str2;

	str2 = shortstr(str, strlen(str), 80);
	strcpy(buf, str2);				/* BAD */
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

