
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

#define MAXSIZE    40
void test(void)
{
        char buf[MAXSIZE];
        cin.width(MAXSIZE-1);
        cin>>buf;               /* BAD */
        cout<<"result: "<<buf<<endl;
}

int main(int argc, char **argv)
{
        test();
        return 0;
}
