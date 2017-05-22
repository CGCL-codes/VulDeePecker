/* This software was developed at the National Institute of Standards and Technology by employees of the Federal Government
in the course of their official duties. Pursuant to title 17 Section 105 of the United States Code
this software is not subject to copyright protection and is in the public domain.
NIST assumes no responsibility whatsoever for its use by other parties,
and makes no guarantees, expressed or implied, about its quality, reliability, or any other characteristic.

This test case is derived from an example found in the Secure Software Inc. CLASP v2.0 User Guide.

*/


/* The following example is exploitable, due to the printf() call in the printWrapper()
function. Note: The stack buffer was added to make exploitation more
simple.
*/


#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {

	if(argc >= 2) {
		char buf[5012];

		strncpy(buf, argv[1], sizeof buf - 1);
		buf[sizeof buf - 1] = 0;

		printf(buf); /* FLAW */
	}

	return (0);
}
