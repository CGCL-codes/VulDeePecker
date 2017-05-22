
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
$Date: 2004/01/05 17:27:48 $
$Header: /mnt/leo2/cvs/sabo/hist-040105/sendmail/s1/crackaddr-ok.c,v 1.1.1.1 2004/01/05 17:27:48 tleek Exp $



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
$Date: 2004/01/05 17:27:48 $
$Header: /mnt/leo2/cvs/sabo/hist-040105/sendmail/s1/crackaddr-ok.c,v 1.1.1.1 2004/01/05 17:27:48 tleek Exp $



*/


/*

<source>

*/



// NB: Derived from sendmail/headers.c

/*
 * Copyright (c) 1998-2003 Sendmail, Inc. and its suppliers.
 *      All rights reserved.
 * Copyright (c) 1983, 1995-1997 Eric P. Allman.  All rights reserved.
 * Copyright (c) 1988, 1993
 *      The Regents of the University of California.  All rights reserved.
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the sendmail distribution.
 *
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


/* ccured needs this */
#pragma ccuredvararg("scanf", printf(1))


/* macro substitution character */
#define MACROEXPAND	((unsigned char)0201)	/* macro expansion */
#define MAXNAME		30		/* max length of a name */

/* check to see if there is room */
#define SM_HAVE_ROOM		((bp < buflim) && (buflim <= bufend))

#define SM_APPEND_CHAR(c)					\
	do							\
	{							\
	  printf ("bp-obp = %d   buflim-obp=%d\n", (bp-obp), (buflim-obp)); \
		if (SM_HAVE_ROOM)				\
			*bp++ = (c);				\
		else						\
			goto returng;				\
	} while (0)


enum bool {false, true}; 


int	ColonOkInAddr;	/* single colon legal in address */
char	*MustQuoteChars;	/* quote these characters in phrases */


/*
**  CRACKADDR -- parse an address and turn it into a macro
**
**	This doesn't actually parse the address -- it just extracts
**	it and replaces it with "$g".  The parse is totally ad hoc
**	and isn't even guaranteed to leave something syntactically
**	identical to what it started with.  However, it does leave
**	something semantically identical.
**
**	This algorithm has been cleaned up to handle a wider range
**	of cases -- notably quoted and backslash escaped strings.
**	This modification makes it substantially better at preserving
**	the original syntax.
**
**	Parameters:
**		addr -- the address to be cracked.
**
**	Returns:
**		a pointer to the new version.
**
**	Side Effects:
**		none.
**
**	Warning:
**		The return value is saved in local storage and should
**		be copied if it is to be reused.
*/

char *
crackaddr(addr)
	register char *addr;
{
	register char *p;
	register char c;
	int cmtlev;
	int realcmtlev;
	int anglelev;
	int copylev;
	int bracklev;
	enum bool qmode;
	enum bool realqmode;
        enum bool putgmac = false;
	enum bool quoteit = false;
	enum bool gotangle = false;
	enum bool gotcolon = false;
	enum bool addangle;
	register char *bp;
	char *obp;
	char *buflim;
	char *bufhead;
	char *bufend; 
	char *addrhead;
	static char buf[MAXNAME + 1];
	static char test_buf[10]; /* use as a canary to detect overflow of */
	                          /* buf[]*/
	
	strcpy(test_buf, "GOOD");
	
        printf("Inside crackaddr!\n");

	
	/* strip leading spaces */
	while (*addr != '\0' && isascii((int)*addr) && isspace((int)*addr))
		addr++;

	/*
	**  Start by assuming we have no angle brackets.  This will be
	**  adjusted later if we find them.
	*/

	buflim = bufend = &buf[sizeof(buf) - 1];
	bp = bufhead = buf;
	obp = bp;
	p = addrhead = addr;
	copylev = anglelev = cmtlev = realcmtlev = 0;
	bracklev = 0;
	qmode = realqmode = addangle = false;

        printf("qmode = %d\n", qmode);

	while ((c = *p++) != '\0')
	{
		/*Try to keep legal syntax using spare buffer space
		**(maintained by buflim).
		*/

	  printf("c = %c\n", c);
              
		if (copylev > 0)
		  /*OK*/
		  SM_APPEND_CHAR(c);
		
		/* check for backslash escapes */
		if (c == '\\')
		{
			/* arrange to quote the address */
			if (cmtlev <= 0 && !qmode)
				quoteit = true;

			if ((c = *p++) == '\0')
			{
				/* too far */
				p--;
				goto putg;
			}
			if (copylev > 0)
			  /*OK*/
			  SM_APPEND_CHAR(c);
			
			goto putg;
		}

		/* check for quoted strings */
		if (c == '"' && cmtlev <= 0)
		  {
			qmode = !qmode;
			if (copylev > 0 && SM_HAVE_ROOM)
			  {
			    if (realqmode)
			      buflim--;
			    else
				  buflim++;
			    realqmode = !realqmode;
			  }
			continue;
		}
		if (qmode)
			goto putg;

		/* check for comments */
		if (c == '(')
		{
		  printf("left ( seen....\n");
			cmtlev++;

			/* allow space for closing paren */
			if (SM_HAVE_ROOM)
			{
				buflim--;
				realcmtlev++;
				if (copylev++ <= 0)
				{
				  if (bp != bufhead)
				    /*OK*/
				    SM_APPEND_CHAR(' ');
				  
				  /*OK*/
				  SM_APPEND_CHAR(c);
				}
			}
		}
		if (cmtlev > 0)
		{
			if (c == ')')
			{
				cmtlev--;
				copylev--;
				if (SM_HAVE_ROOM)
				{
					realcmtlev--;
					buflim++;
				}
			}
			continue;
		}
		else if (c == ')')
		{
			/* syntax error: unmatched ) */
			if (copylev > 0 && SM_HAVE_ROOM)
				bp--;
		}

		/* count nesting on [ ... ] (for IPv6 domain literals) */
		if (c == '[')
			bracklev++;
		  else if (c == ']')
			bracklev--;

		/* check for group: list; syntax */
		if (c == ':' && anglelev <= 0 && bracklev <= 0 &&
		    !gotcolon && !ColonOkInAddr)
		{		        
			register char *q;

			/*
			**  Check for DECnet phase IV ``::'' (host::user)
			**  or **  DECnet phase V ``:.'' syntaxes.  The latter
			**  covers ``user@DEC:.tay.myhost'' and
			**  ``DEC:.tay.myhost::user'' syntaxes (bletch).
			*/
			
			if (*p == ':' || *p == '.')
			{
				if (cmtlev <= 0 && !qmode)
					quoteit = true;
				if (copylev > 0)
				{
				  /*OK*/
				  SM_APPEND_CHAR(c); 
				  /*OK*/
				  SM_APPEND_CHAR(*p);
				}
				p++;
				goto putg;
			}

			gotcolon = true;

			bp = bufhead;
			if (quoteit)
			{
			  /*OK*/
			  SM_APPEND_CHAR('"');

			  /* back up over the ':' and any spaces */
			  --p;
			  
			  while (p > addr && isascii((int) *--p) && isspace((int) *p))
				  continue;
			  p++;
			}
			for (q = addrhead; q < p; )
			{
				c = *q++;
				if (quoteit && c == '"')
				  {
				    /*OK*/
				    SM_APPEND_CHAR('\\');
				    /*OK*/
				    SM_APPEND_CHAR(c);
				  }
				else
				  /*OK*/
				  SM_APPEND_CHAR(c);
			}
			if (quoteit)
			{
				if (bp == &bufhead[1])
					bp--;
				else	
				  /*OK*/
				  SM_APPEND_CHAR('"');
				
				while ((c = *p++) != ':')
				  /*OK*/
				  SM_APPEND_CHAR(c);
				
				/*OK*/
				SM_APPEND_CHAR(c);
			}

			/* any trailing white space is part of group: */
			while (isascii((int)*p) && isspace((int)*p))
			  {
			    /*OK*/
			    SM_APPEND_CHAR(*p);
			    p++;
			  }
			copylev = 0;
			putgmac = quoteit = false;
			bufhead = bp;
			addrhead = p;
			continue;
		}

		if (c == ';' && copylev <= 0 && !ColonOkInAddr)
		  /*OK*/
		  SM_APPEND_CHAR(c);

		/* check for characters that may have to be quoted */
		if (strchr(MustQuoteChars, c) != NULL)
		{
			/*
			**  If these occur as the phrase part of a <>
			**  construct, but are not inside of () or already
			**  quoted, they will have to be quoted.  Note that
			**  now (but don't actually do the quoting).
			*/

			if (cmtlev <= 0 && !qmode)
				quoteit = true;
		}

		/* check for angle brackets */
		if (c == '<')
		{
			register char *q;

			/* assume first of two angles is bogus */
			if (gotangle)
				quoteit = true;
			gotangle = true;

			/* oops -- have to change our mind */
			anglelev = 1;
			if (SM_HAVE_ROOM)
			  {
			    if (!addangle)
			      buflim--;
			    addangle = true;
			  }
			    

			bp = bufhead;
			if (quoteit)
			  {
			    /*OK*/
			    SM_APPEND_CHAR('"');
			    
			    /* back up over the '<' and any spaces */
			    --p;
			    while (p > addr && isascii((int)*--p) && isspace((int)*p))
			      continue;
			    p++;
			  }
			for (q = addrhead; q < p; )
			  {
				c = *q++;
				if (quoteit && c == '"')
				  {
				    /*OK*/
				    SM_APPEND_CHAR('\\');
				    /*OK*/
				    SM_APPEND_CHAR(c);
				  }
				else
				  /*OK*/
				  SM_APPEND_CHAR(c);
			}
			if (quoteit)
			{
			  if (bp == &buf[1])
			    bp--;
			  else
			    /*OK*/
			    SM_APPEND_CHAR('"');
			  while ((c = *p++) != '<')
			    /*OK*/
			    SM_APPEND_CHAR(c);
			  
			  /*OK*/
			  SM_APPEND_CHAR(c);
			}
			copylev = 0;
			putgmac = quoteit = false;
			continue;
		}

		if (c == '>')
		{
			if (anglelev > 0)
			{
				anglelev--;
				if (SM_HAVE_ROOM)
				{
				  if (addangle)
				    buflim++;
				  addangle = false;	
				  
				}
			}
			else if (SM_HAVE_ROOM)
			{
				/* syntax error: unmatched > */
				if (copylev > 0)
					bp--;
				quoteit = true;
				continue;
			}
			if (copylev++ <= 0)
			  /*OK*/
			  SM_APPEND_CHAR(c);
			  
			continue;
		}

		/* must be a real address character */
	putg:
		if (copylev <= 0 && !putgmac)
		  {
		    if (bp > buf && bp[-1] == ')')
		      /*OK*/
		      SM_APPEND_CHAR(' ');
		    
		    /*OK*/
		    SM_APPEND_CHAR(MACROEXPAND);
		    /*OK*/
		    SM_APPEND_CHAR('g');
		    putgmac = true;
		    
		  }
	}
	
	/* repair any syntactic damage */
	if (realqmode && bp < bufend)
	  /*OK*/
	  *bp++ = '"';
	while (realcmtlev-- > 0 && bp < bufend)
	  /*OK*/
	  *bp++ = ')';
	if (addangle && bp < bufend)
	  /*OK*/
	  *bp++ = '>';
	
	/*OK*/
	*bp = '\0';
  	if (bp < bufend)
	  goto success;

 returng:
	/*String too long, punt */
	buf[0] = '<';
	buf[1] = MACROEXPAND;
	buf[2]= 'g';
	buf[3] = '>';
	buf[4]= '\0';
        printf("Dropped invalid comments from header address\n");

success:	

	printf("test_buf should equal GOOD\n");
	printf("test_buf = %s\n", test_buf);
	return buf;
}


int main(){

  char address[100];
  char *res_addr;
  
  printf("Allow or disallow colon in address? Type 1 or 0 respectively:\n");
  scanf("%d", &ColonOkInAddr);  /* allow colon in address */

  MustQuoteChars = "@,;:\\()[].'";


  printf("Enter email address:\n");
  scanf("%99s", address);
  
  res_addr = crackaddr(address);
  printf("result = %s\n", res_addr);
  printf("buf len = %d\n", strlen(res_addr)); 

  return 0;
}

/*

</source>

*/

