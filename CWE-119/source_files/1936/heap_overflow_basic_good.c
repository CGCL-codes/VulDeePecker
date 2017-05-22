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

#define BUFSIZE 256

int main(int argc, char **argv) 
{
	char *buf;
	buf = (char *)malloc(BUFSIZE);
	if (strlen(argv[1]) < BUFSIZE)
		strcpy(buf, argv[1]);
	free(buff);
}
