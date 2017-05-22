
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
$Date: 2004/01/05 17:27:40 $
$Header: /mnt/leo2/cvs/sabo/hist-040105/bind/b3/create_iquery.c,v 1.1.1.1 2004/01/05 17:27:40 tleek Exp $



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
$Date: 2004/01/05 17:27:40 $
$Header: /mnt/leo2/cvs/sabo/hist-040105/bind/b3/create_iquery.c,v 1.1.1.1 2004/01/05 17:27:40 tleek Exp $



*/


/*

<source>

*/

#include <stdio.h>
#include <sys/types.h>
#include <arpa/nameser.h>
#include <arpa/nameser_compat.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <resolv.h>


int main(){

  FILE *f;
  u_char buf[1000];
  u_char *p;
  char *temp, *temp1; 
  u_char *comp_dn, *comp_dn2;
  char exp_dn[200], exp_dn2[200];
  u_char **dnptrs, **lastdnptr, **dnptrs2;
  int i,len = 0, comp_size;
  u_long now;

  dnptrs = (unsigned char **) malloc(2 * sizeof(unsigned char *));
  dnptrs2 = (unsigned char **) malloc(2 * sizeof(unsigned char *));

  comp_dn = (unsigned char *) malloc(200*sizeof(unsigned char));
  comp_dn2 = (unsigned char *) malloc(200*sizeof(unsigned char));

  temp1 = (char *) malloc(400*sizeof(char));
    
  temp = temp1;
  
  p = buf;

  PUTSHORT(65535, p);
  PUTSHORT(256*8, p);  /* set h->opcode to ns_o_iquery */ 
  PUTSHORT(65535, p);
  PUTSHORT(65535, p);
  PUTSHORT(65535, p);
  PUTSHORT(65535, p);

  len += 12;
 
  strcpy(exp_dn, "lcs.mit.edu");         
  
  *dnptrs++ = (u_char *) exp_dn;
  *dnptrs-- = NULL;

  lastdnptr = NULL;

  printf("Calling dn_comp..\n");
  comp_size = dn_comp((const char *) exp_dn, comp_dn, 200, dnptrs, lastdnptr);
  printf("uncomp_size = %d\n", strlen(exp_dn));
  printf("comp_size = %d\n", comp_size);
  printf("exp_dn = %s, comp_dn = %s\n", exp_dn, (char *) comp_dn);
  
  for(i=0; i<comp_size; i++) 
    *p++ = *comp_dn++;

  len += comp_size;

  PUTSHORT(T_A, p); /* type = T_A = 1 */
  PUTSHORT(C_IN, p);   /* class = C_IN = 1*/


  PUTLONG(255, p);  /* ttl */
  PUTSHORT(4, p);  /* dlen */
 
  PUTSHORT(255, p);  /* Write IP address */  
  PUTSHORT(255, p);

  len += 14;

  f = fopen("iquery-file", "w");

  p = buf;

  printf("len = %d\n", len);
  for(i=0; i<len; i++, p++)  /* write record into file */
    fputc(*p, f);
  

  fclose(f);
  return 0;
}


/*

</source>

*/

