
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


#include <cstdlib>
#include <ctime>
#include <cstring>

char *rand_text() {
	srand(time(0));
	unsigned length = rand() % 50 +10;
	char *t = new char[length];
	if (!t)
		return 0;
	unsigned i=0;
	for (;i<9;++i)
	{
		t[i] = (char)((rand() % 26)  + 'a');
	}
	t[i+1] = '\0';
	return t;
}

int main(int argc, char *argv[])
{
	char *buf = 0;
	buf = new char[25];

	if (buf != 0)
	{
		char *t = rand_text();
		if (t) {
			buf[strlen(t)%25]=t[strlen(t)-1];
		}
	}
	delete [] buf;
	return 0;
}
