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
#include <stdio.h>
#include <time.h>

unsigned int getRand()
{
	unsigned int r;
	FILE *f;

	f = fopen("/dev/urandom", "rb");
	if(f == NULL)
	{
		fprintf(stderr, "Error opening file\n");
		exit(-1);
	}

	if(fread(&r, sizeof r, 1, f) != 1)
	{
		fprintf(stderr, "Error reading file\n");
		fclose(f);
		exit(-1);
	}

	if(fclose(f) != 0)
		fprintf(stderr, "Error closing file\n");

	return r;
}

// Binary Russian Dice
char *rand_text() {
	unsigned length = getRand() % 50 - 1;
	char *t = malloc((length + 1) * sizeof(char));
	if (!t) 
		return NULL;
	unsigned i=0;
	for (;i<length;++i)
	{
		t[i] = (char)((getRand() % 26)  + 'a');
	}
	t[i] = '\0';
	return t;
}

int main(int argc, char *argv[])
{
	char *buf = (char *)NULL;
	buf = malloc(25*sizeof(char));
	
	if (buf != (char *)NULL)
	{
		char *t = rand_text();
		if (t) {
			strcpy(buf,t);							/* FLAW */
			free(t);
			printf("%s\n", buf);
		}
		free(buf);
	}
	return 0;
}

