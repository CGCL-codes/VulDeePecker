
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




#include <iostream>
using namespace std;
#define BUFSIZE 32

void tester (const char *__str) {
	char *test = new (nothrow) char[BUFSIZE+1];
	if (test!= 0)
	{test[strlen(__str)%BUFSIZE]='a';
	delete [] test;}
	else cout << "Error: memory could not be allocated";
	return;
}

int main(int argc, const char *argv[])
{
	if (argc > 1){
		const char *userstr=argv[1];
		tester(userstr);
	}
	return 0;
}
