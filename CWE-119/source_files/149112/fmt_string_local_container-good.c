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
#include <stdio.h>
#include <string.h>

#define MAX_SIZE 255

typedef struct cont_t cont;
struct cont_t {
	char fmt[MAX_SIZE];
};

int main(int argc, char *argv[])
{
	if (argc > 1) {
		cont container = {.fmt=""};
		strncpy(container.fmt, argv[1],MAX_SIZE-1);
		container.fmt[MAX_SIZE - 1] = '\0';
		printf("%s", container.fmt);			/* FIX */
	}
	return 0;
}
