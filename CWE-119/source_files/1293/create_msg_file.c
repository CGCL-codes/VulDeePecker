
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
$Date: 2004/01/05 17:27:41 $
$Header: /mnt/leo2/cvs/sabo/hist-040105/bind/b1/create_msg_file.c,v 1.1.1.1 2004/01/05 17:27:41 tleek Exp $



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
$Date: 2004/01/05 17:27:41 $
$Header: /mnt/leo2/cvs/sabo/hist-040105/bind/b1/create_msg_file.c,v 1.1.1.1 2004/01/05 17:27:41 tleek Exp $



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

  strcpy(temp, "HEADER JUNK:");
  
  len += strlen(temp);

  while (*temp != '\0') 
    *p++ = *temp++;
  
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

  PUTSHORT(30, p); /* type = T_NXT = 30 */
  p += 2;    
  
  PUTSHORT(255, p);   /* class = C_ANY = 255*/
  p += 2;

  PUTLONG(255, p);  /* ttl */
  p += 4;

  PUTSHORT(16, p); /* dlen = len of everything starting with the covered byte 
		      (the length of the entire resource record... we lie about it
		   */
  p += 2;

  len += 10;

  strcpy(exp_dn2, "sls.lcs.mit.edu");    /* domain name */

  *dnptrs2++ = (u_char *) exp_dn2;
  *dnptrs2-- = NULL;
  lastdnptr = NULL;
  
  printf("Calling dn_comp..\n");
  comp_size = dn_comp((const char *) exp_dn2, comp_dn2, 200, dnptrs2, lastdnptr);
  printf("uncomp_size = %d\n", strlen(exp_dn2));
  printf("comp_size = %d\n", comp_size);
  printf("exp_dn2 = %s, comp_dn2 = %s\n", exp_dn2, (char *) comp_dn2);

  len += comp_size;
   
  for(i=0; i<comp_size; i++) 
    *p++ = *comp_dn2++;

  PUTLONG(1 << 24, p);             /* bitmap - set NXT type*/
  p += 4;
  PUTLONG(0, p);
  p += 4;
  PUTLONG(0, p);
  p += 4;
  PUTLONG(0, p);
  p += 4;

  len += 16;
   
  f = fopen("SIGFILE", "w");

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

