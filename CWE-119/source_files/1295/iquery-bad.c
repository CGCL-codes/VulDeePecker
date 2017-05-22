
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
$Header: /mnt/leo2/cvs/sabo/hist-040105/bind/b3/iquery-bad.c,v 1.1.1.1 2004/01/05 17:27:40 tleek Exp $



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
$Header: /mnt/leo2/cvs/sabo/hist-040105/bind/b3/iquery-bad.c,v 1.1.1.1 2004/01/05 17:27:40 tleek Exp $



*/


/*

<source>

*/

#include <stdio.h>
#include <sys/types.h>
#include <arpa/nameser.h>
#include <arpa/nameser_compat.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <resolv.h>
#include <assert.h>






enum req_action { Finish, Refuse, Return };

int something;

/* Modified version of req_action */
static enum req_action
req_iquery(HEADER *hp, u_char **cpp, u_char *eom, int *buflenp, u_char *msg)
{
	int dlen, alen, n, type, class, count;
	char anbuf[2], *data, *fname;

	/*
	 * Skip domain name, get class, and type.
	 */
	if ((n = dn_skipname(*cpp, eom)) < 0) {
	  printf("FORMERR IQuery packet name problem\n");
	  hp->rcode = FORMERR;
	  return (Finish);
	}
	*cpp += n;
	GETSHORT(type, *cpp);
	GETSHORT(class, *cpp);
	*cpp += INT32SZ;	/* ttl */
	GETSHORT(dlen, *cpp);
	*cpp += dlen;
	if (*cpp != eom) {
	  printf("FORMERR IQuery message length off\n");
	  hp->rcode = FORMERR;
	  return (Finish);
	}

	/*
	 * not all inverse queries are handled.
	 * this is a meaningless switch statement to preserve original program's structure 
	 */

	switch (type) {
	case T_A:
	  if (something == 0) 
	    return (Refuse);
	  break;
	default:
	  return (Refuse);
	}
	printf("req: IQuery class %d type %d\n", class, type);

	fname = (char *)msg + HFIXEDSZ;
	alen = (char *)*cpp - fname;
	
	printf("Copying %d bytes from fname to anbuf which can store %d bytes\n", alen, sizeof(anbuf)); 
	/*BAD*/
	memcpy(anbuf, fname, alen);
	data = anbuf + alen - dlen;
	*cpp = (u_char *)fname;
	*buflenp -= HFIXEDSZ;
	count = 0;


	/* ..... do some other stuff */

	return (Finish);
}

/* read in a resource record from a file */
int create_msg(u_char *msg, int len){

  FILE *f;
  int i = 0;
  int c;

  if ((f = fopen("iquery-file", "r")) == NULL)
    {
      printf("iquery-file not found!\n");
      return -1;
    }
  
  while (((c = fgetc(f)) != EOF) && (i < len)) {
    *msg++ = (u_char) c;
    i++;  
  }

  fclose(f);

  return i;

}


int main(int argc, char **argv){
  
  HEADER *hp;
  u_char *msg, *cp, *eom;
  int msglen;
  FILE *f;

  assert(argc==2);
  f = fopen (argv[1], "r");
  assert(f!=NULL);
  assert ((fscanf(f, "%d", &something)) != 0);
  

  msg = (u_char *) malloc(10000*sizeof(u_char));
  msglen = create_msg(msg, 10000);

  hp = (HEADER *) msg;

  /*
  printf("Enter value for something:");
  scanf("%d", &something);
  */

  cp = msg + sizeof(HEADER);
  eom = msg + msglen;

  printf("opcode = %d\n",hp->opcode);
  
  switch(hp->opcode){
  case ns_o_iquery:           /* ns_o_iquery = */ 
    req_iquery(hp, &cp, eom, &msglen, msg);
    break;
  default:
    printf("We only support inverse queries!\n");
  }

  return 0;

}


/*

</source>

*/

