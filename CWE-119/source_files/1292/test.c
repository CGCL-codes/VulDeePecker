
/*

MIT Copyright Notice

Copyright 2003 M.I.T.

Permission is hereby granted, without written agreement or royalty fee, to use, 
copy, modify, and distribute this software and its documentation for any 
purpose, provided that the above copyright notice and the following three 
paragraphs appear in all copies of this software.

IN NO EVENT SHALL M.I.T. BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, 
INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE 
AND ITS DOCUMENTATION, EVEN IF M.I.T. HAS BEEN ADVISED OF THE POSSIBILITY OF 
SUCH DAMANGE.

M.I.T. SPECIFICALLY DISCLAIMS ANY WARRANTIES INCLUDING, BUT NOT LIMITED TO 
THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
AND NON-INFRINGEMENT.

THE SOFTWARE IS PROVIDED ON AN "AS-IS" BASIS AND M.I.T. HAS NO OBLIGATION TO 
PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

$Author: tleek $
$Date: 2004/01/05 17:27:36 $
$Header: /mnt/leo2/cvs/sabo/hist-040105/bind/b2/test.c,v 1.1.1.1 2004/01/05 17:27:36 tleek Exp $



*/


/*

BIND Copyright Notice

 Copyright (C) 2000-2002  Internet Software Consortium.

 Permission to use, copy, modify, and distribute this software for any
 purpose with or without fee is hereby granted, provided that the above
 copyright notice and this permission notice appear in all copies.

 THE SOFTWARE IS PROVIDED "AS IS" AND INTERNET SOFTWARE CONSORTIUM
 DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL
 INTERNET SOFTWARE CONSORTIUM BE LIABLE FOR ANY SPECIAL, DIRECT,
 INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING
 FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
 WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.


$Author: tleek $
$Date: 2004/01/05 17:27:36 $
$Header: /mnt/leo2/cvs/sabo/hist-040105/bind/b2/test.c,v 1.1.1.1 2004/01/05 17:27:36 tleek Exp $



*/


/*

<source>

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <values.h>

#define MAXDATA 10

int main(){

  int n;
  unsigned short s1 = 10;
  unsigned int s2 = 10;
  size_t s3 = 10;
  char buf[MAXDATA];

  n = -1;

  if (n < s1)
    printf("-1 < 10 (unsigned short) \n");
  else
    printf("-1 >= 10 (unsigned short)\n");

   if (n < s2)
    printf("-1 < 10 (unsigned int) \n");
   else
    printf("-1 >= 10 (unsigned int) \n");

   if (n < s3)
    printf("-1 < 10 (size_t) \n");
  else
    printf("-1 >= 10 (size_t) \n");

   if (n < sizeof(buf))
     printf("-1 < 10 (sizeof(buf)) \n");
   else
     printf("-1 >= 10 (sizeof(buf)) \n");


  return 0;
}

/*

</source>

*/

