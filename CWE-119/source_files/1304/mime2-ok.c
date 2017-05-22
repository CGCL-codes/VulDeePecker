
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
$Date: 2004/01/05 17:27:44 $
$Header: /mnt/leo2/cvs/sabo/hist-040105/sendmail/s4/mime2-ok.c,v 1.1.1.1 2004/01/05 17:27:44 tleek Exp $



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
$Date: 2004/01/05 17:27:44 $
$Header: /mnt/leo2/cvs/sabo/hist-040105/sendmail/s4/mime2-ok.c,v 1.1.1.1 2004/01/05 17:27:44 tleek Exp $



*/


/*

<source>

*/

# include "mime2.h"
# include <string.h>
#include <assert.h>

void mime7to8(HDR *, ENVELOPE *);
char * hvalue(char *, HDR *);

int main(int argc, char **argv){

  HDR *header;
  register ENVELOPE *e;
  FILE *temp;
 
  assert (argc==2);
  header = (HDR *) malloc(sizeof(struct header));
  
  header->h_field = "Content-Transfer-Encoding";
  header->h_value = "base64";

  e = (ENVELOPE *) malloc(sizeof(struct envelope));
  e->e_id = "First Entry";

  temp = fopen(argv[1],"r");
 
  e->e_dfp = temp;
  mime7to8(header, e);

  fclose(temp);

  return 0;


  return 0;
}

static char index_64[128] =
{
	-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
	-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
	-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,62, -1,-1,-1,63,
	52,53,54,55, 56,57,58,59, 60,61,-1,-1, -1,-1,-1,-1,
	-1, 0, 1, 2,  3, 4, 5, 6,  7, 8, 9,10, 11,12,13,14,
	15,16,17,18, 19,20,21,22, 23,24,25,-1, -1,-1,-1,-1,
	-1,26,27,28, 29,30,31,32, 33,34,35,36, 37,38,39,40,
	41,42,43,44, 45,46,47,48, 49,50,51,-1, -1,-1,-1,-1
};

#define CHAR64(c)  (((c) < 0 || (c) > 127) ? -1 : index_64[(c)])

/*
**  MIME7TO8 -- output 7 bit encoded MIME body in 8 bit format
**
**  This is a hack. Supports translating the two 7-bit body-encodings
**  (quoted-printable and base64) to 8-bit coded bodies.
**
**  There is not much point in supporting multipart here, as the UA
**  will be able to deal with encoded MIME bodies if it can parse MIME
**  multipart messages.
**
**  Note also that we wont be called unless it is a text/plain MIME
**  message, encoded base64 or QP and mailer flag '9' has been defined
**  on mailer.
**
**  Contributed by Marius Olaffson <marius@rhi.hi.is>. Modified by Misha Zitser
**
**	Parameters:
**		header -- the header for this body part.
**		e -- envelope.
**
**	Returns:
**		none.
*/

void
mime7to8(header, e)
	HDR *header;
	register ENVELOPE *e;
{
 
	register char *p;
	u_char *fbufp;
	char canary[10];
	u_char fbuf[MAXLINE + 1];
	
	strcpy(canary, "GOOD"); /* use canary to see if fbuf gets overflowed */ 

	p = hvalue("Content-Transfer-Encoding", header);
	if (p == NULL)
	  {
	    printf("Content-Transfer-Encoding not found in header\n");
	    return;
	  }
   
	/*
	**  Translate body encoding to 8-bit.  Supports two types of
	**  encodings; "base64" and "quoted-printable". Assume qp if
	**  it is not base64.
	*/

	if (strcasecmp(p, "base64") == 0)
	{
		int c1, c2, c3, c4;

		fbufp = fbuf;
		while ((c1 = fgetc(e->e_dfp)) != EOF)
		{
			if (isascii(c1) && isspace(c1))
				continue;

			do
			{
				c2 = fgetc(e->e_dfp);
			} while (isascii(c2) && isspace(c2));
			if (c2 == EOF)
				break;

			do
			{
				c3 = fgetc(e->e_dfp);
			} while (isascii(c3) && isspace(c3));
			if (c3 == EOF)
				break;

			do
			{
				c4 = fgetc(e->e_dfp);
			} while (isascii(c4) && isspace(c4));
			if (c4 == EOF)
				break;

			if (c1 == '=' || c2 == '=')
				continue;
			c1 = CHAR64(c1);
			c2 = CHAR64(c2);
			
			/*We have read in four characters */
			if (fbufp >= &fbuf[MAXLINE]){
			  /*OK*/
			  *fbufp = '\0';
			  fbufp = fbuf;
			}
			else
			  *fbufp = (c1 << 2) | ((c2 & 0x30) >> 4);              /* Write first char to fbuf */
			

			/* Check to see if either 1) reached a '\n' or */
			/* 2) fbufp about to point past end of fbuf (unfortunately there is a typo in the if-check) */

			/* suppose the string in e->e_dfp is longer than MAXLINE + 1 and doesn't contain any '\n's, then" */       
		        /* we will never catch the case when fbufp points past the end of fbuf because of the */
			/*typo in the check below. fbuf will get overflowed with arbitrary data */

			if ((*fbufp++ == '\n' || fbufp >= &fbuf[MAXLINE]))   /* typo fixed */
			{
			 
			  if (fbufp != &fbuf[1]){
			    /*OK*/
			    if (*--fbufp != '\n' || *--fbufp != '\r')          /* fixed possible read below bounds */
			      fbufp++;                        
			  }
			     
			  *fbufp = '\0';
			  /* putline((char *) fbuf, mci); */
			  printf("resetting fbufp\n");
			  fbufp = fbuf;                                       /* fbufp gets reset to fbuf. */
			                                           
			}
			if (c3 == '=')
				continue;
			c3 = CHAR64(c3);

			if (fbufp >= &fbuf[MAXLINE])
			  {
			    /*OK*/
			    *fbufp = '\0';
			    fbufp = fbuf;
			  }
			else
			  *fbufp = ((c2 & 0x0f) << 4) | ((c3 & 0x3c) >> 2);
			
			if (*fbufp++ == '\n' || fbufp >= &fbuf[MAXLINE])  /* typo fixed */
			{
			 
			  if (fbufp != &fbuf[1]){
			    /*OK*/
			    if (*--fbufp != '\n' || *--fbufp != '\r')
			      fbufp++;
			  }
			  *fbufp = '\0';
				/* putline((char *) fbuf, mci);*/
				printf("resetting fbufp\n");
				fbufp = fbuf;
			}
			if (c4 == '=')
				continue;
			c4 = CHAR64(c4);

	
			if (fbufp >= &fbuf[MAXLINE])
			  {
			    /*OK*/
			    *fbufp = '\0';
			    fbufp = fbuf;
			  }
			else
			  *fbufp = ((c3 & 0x03) << 6) | c4;
			
			if (*fbufp++ == '\n' || fbufp >= &fbuf[MAXLINE])    /* typo fixed */
			{
			  if (fbufp != &fbuf[1]){
			    /*OK*/
			    if (*--fbufp != '\n' || *--fbufp != '\r')
			    fbufp++;
			  }
			  *fbufp = '\0';
			  /* putline((char *) fbuf, mci); */
			  printf("resetting fbufp\n");
			  fbufp = fbuf;
			}
		}

		/* force out partial last line */
		if (fbufp >= fbuf)
		{
			*fbufp = '\0';
			/* putline((char *) fbuf, mci); */
		}
	}
	else
	{
	  /* do something else...  */
	  printf("We only support base64 encoding...\n");
	}

	printf("canary should be GOOD\n");
	printf("canary = %s\n", canary);
}



/*
**  HVALUE -- return value of a header.
**
**	Only "real" fields (i.e., ones that have not been supplied
**	as a default) are used.
**
**	Parameters:
**		field -- the field name.
**		header -- the header list.
**
**	Returns:
**		pointer to the value part.
**		NULL if not found.
**
**	Side Effects:
**		none.
*/

char * hvalue(field, header)
	char *field;
	HDR *header;
{
	register HDR *h;

	for (h = header; h != NULL; h = h->h_link)
	{
		if (!bitset(H_DEFAULT, h->h_flags) &&
		    strcasecmp(h->h_field, field) == 0)
			return (h->h_value);
	}
	return (NULL);
}


/*

</source>

*/

