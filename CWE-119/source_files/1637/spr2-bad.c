/*
Description: Sprintf is used to copy a string to a stack buffer.  The length is guarded with a length specifier in the format string but the wrong length is given.
Keywords: Port C Size0 Complex0 BufferOverflow Stack Sprintf BadBound
ValidArg: "a"*20
InvalidArg: "a"*100

This test case may not fail on Invalid input on many compilers
due to additional padding between the buffers.

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

/*
 * Only a few characters overrun, so we pick a round sized buffer
 * in hopes that the compiler doesn't add any extra padding and
 * inadvertantly provide enough room to hold the extra characters.
 */
#define	MAXSIZE		32

void
test(char *str)
{
	char buf[MAXSIZE];

	/* 32 + "<>" + NUL = 35, allowing 3 bytes of overflow */
	sprintf(buf, "<%.32s>", str);		/* BAD */
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

