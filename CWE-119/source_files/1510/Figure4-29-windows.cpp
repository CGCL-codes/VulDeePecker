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
#include <windows.h>


unsigned char shellcode[] = "\x90\x90\x90\x90"; 
unsigned char malArg[] = "0123456789012345"
						"\x05\x00\x03\x00\x00\x00\x08\x00"
						"\xb8\xf5\x12\x00\x40\x90\x40\x00";

void mem() 
{
	HANDLE hp;
	HLOCAL h1 = 0, h2 = 0, h3 = 0, h4 = 0;
	
	hp = HeapCreate(0, 0x1000, 0x10000);
	h1 = HeapAlloc(hp, HEAP_ZERO_MEMORY, 16);
	h2 = HeapAlloc(hp, HEAP_ZERO_MEMORY, 128);
	h3 = HeapAlloc(hp, HEAP_ZERO_MEMORY, 16);
	HeapFree(hp, 0, h2);
	memcpy(h1, malArg, 32);
	h4 = HeapAlloc(hp, HEAP_ZERO_MEMORY, 128);
	return;
}
int main(int argc, char * argv[])
{
	if (argc !=1){
		printf("Usage: prog_name\n");
		exit(-1);
	}
	mem();
	return 0;
}



