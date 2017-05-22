/*
Description: integer overflow results in a short malloc and an overflow.  A guard is put in place to protect against the overflow.
Keywords: Port C Size0 Complex1 BufferOverflow Heap AdHoc IntOverflow
ValidArg: "10"
ValidArg: "1073741823"
ValidArg: "1073741824"
ValidArg: "1073741825"

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
#include <limits.h>

void
test(unsigned int n)
{
	int *buf, i;

	/* Disallow only values that cause signed overflow */
	if(n > INT_MAX / sizeof *buf)
		return;
	buf = malloc(n * sizeof *buf);		/* OK */
	if(!buf)
		return;
	for(i = 0; i < n; i++)
		buf[i] = i;			/* OK */
	while(i-- > 0)
		printf("%x ", buf[i]);		/* OK */
	printf("\n");
	free(buf);
}

int
main(int argc, char **argv)
{
	int n;

	if(argc != 2)
		return 1;
	n = strtoul(argv[1], 0, 10);
	test(n);
	return 0;
}

