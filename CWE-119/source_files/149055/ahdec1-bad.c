/*
Description: A string decode function misses a termination check which allows the decode to proceed past the end of the buffer.
Keywords: Port C Size0 Complex1 BufferOverflow Stack AdHocDecode NoNul
ValidArg: "a" * 31
ValidArg: "a" * 100
InvalidArg: ("a" * 30) + "%"

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

#define MAXSIZE		32

int
hexchar(char ch)
{
	if(ch >= '0' && ch <= '9')
		return ch - '0';
	if(ch >= 'a' && ch <= 'f')
		return ch - 'a' + 10;
	if(ch >= 'A' && ch <= 'F')
		return ch - 'A' + 10;
	return 0;
}

void
test(char *str)
{
	char buf[MAXSIZE];
	char *p, *q;

	/* we're decoding in place, we need a writable string */
	strncpy(buf, str, MAXSIZE);
	buf[MAXSIZE-1] = '\0';

	p = buf;
	q = p;
	while(*p) {
		if(*p == '%') {
			/* p[2] may be past the end of the string */
			*q++ = (hexchar(p[1])<<4) | hexchar(p[2]); /* FLAW */
			p += 2;
		} else
			*q++ = *p++;				/* FLAW */
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

