/* This software was developed at the National Institute of Standards and
 * Technology by employees of the Federal Government in the course of their
 * official duties. Pursuant to title 17 Section 105 of the United States
 * Code this software is not subject to copyright protection and is in the
 * public domain. NIST assumes no responsibility whatsoever for its use by
 * other parties, and makes no guarantees, expressed or implied, about its
 * quality, reliability, or any other characteristic.

 * We would appreciate acknowledgement if the software is used.
 * The SAMATE project website is: http://samate.nist.gov
 *
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static char shellcode[] = "\xeb\x0cjump12chars_\x90\x90\x90\x90\x90\x90\x90\x90"; 

int main(void)
{
	int size = sizeof(shellcode);
	char *shellcode_location;

	shellcode_location = (char *)malloc(size);
	if (shellcode_location == NULL)
        	{printf("Memory allocation problem"); return (1);}
	strcpy(shellcode_location, shellcode);
	printf("%s", shellcode_location);
	free(shellcode_location);
	// free(shellcode_location);							/* FIX */
	return 0;
}
