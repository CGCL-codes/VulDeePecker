/* This software was developed at the National Institute of Standards and
 * Technology by employees of the Federal Government in the course of their
 * official duties. Pursuant to title 17 Section 105 of the United States
 * Code this software is not subject to copyright protection and is in the
 * public domain. NIST assumes no responsibility whatsoever for its use by
 * other parties, and makes no guarantees, expressed or implied, about its
 * quality, reliability, or any other characteristic.

 * We would appreciate acknowledgement if the software is used.
 * The SAMATE project website is: http://samate.nist.gov
*/

#include <stdlib.h>
#include <string.h>

typedef char * cptr;

int main(int argc, char *argv[])
{
	cptr buf[3] = {(char *)NULL,(char *)NULL,(char *)NULL};
	
	buf[2] = malloc(5*sizeof(char));
	if(buf[2] != NULL)
	{
		if (argc > 1)
		{
			strcpy(buf[2],argv[1]);						/* FLAW */
		}
		free(buf[2]);
	}
	return 0;
}
