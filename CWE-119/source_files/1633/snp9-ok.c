/*
Description: snprintf is used repeatedly while keeping track of the residual buffer length.
Keywords: Port C Size0 Complex1 BufferOverflow Heap AdHoc IntOverflow
ValidArg: "%s %s" % ("a"*15, "b"*15)
ValidArg: "%s %s" % ("a"*15, "b"*45)
ValidArg: "%s %s" % ("a"*45, "b"*45)

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
#include <stdlib.h>
#include <string.h>

#define	MAXSIZE		40
void
test(char *str1, char *str2)
{
	char *buf, *p;
	int l, x;

	buf = malloc(MAXSIZE);
	if(!buf)
		return;
	p = buf;
	l = MAXSIZE;

	snprintf(p, l, "<%s>", str1);
	x = strlen(p);
	p += x;
	l -= x;

	if(l > 2) {
		*p++ = ' ';				/* OK */
		*p++ = '-';				/* OK */
		l -= 2;					/* OK */
	}

	/* now length may be negative */
	snprintf(p, l, "<%s>\n", str2);			/* OK */
	x = strlen(p);
	p += x;
	l -= x;

	printf("result: %s\n", buf);
	free(buf);
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

