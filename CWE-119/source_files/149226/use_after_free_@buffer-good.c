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


int main(int argc, char *argv[])
{
	char *str[1] = {(char *)NULL};
	if ((*str = (char *)malloc(256*sizeof(char))) != NULL)
	{
		strcpy(*str, "Falut!");
		**str = 'S';
		printf("%s\n", *str);
		free(*str);	
		*str = NULL;
		if (*str)								/* FIX */
			**str = 'S';
	}
	return 0;
}
