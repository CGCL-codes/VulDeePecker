
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
$Date: 2004/02/05 15:19:49 $
$Header: /mnt/leo2/cvs/sabo/hist-040105/sendmail/s2/util-bad.c,v 1.2 2004/02/05 15:19:49 tleek Exp $



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
$Date: 2004/02/05 15:19:49 $
$Header: /mnt/leo2/cvs/sabo/hist-040105/sendmail/s2/util-bad.c,v 1.2 2004/02/05 15:19:49 tleek Exp $



*/


/*

<source>

*/

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "sendmail-bad.h"

#define SPACELEFT(buf, bp)   (sizeof buf - ((bp) - buf))

/*
**  XALLOC -- Allocate memory and bitch wildly on failure.
**
**	THIS IS A CLUDGE.  This should be made to give a proper
**	error -- but after all, what can we do?
**
**	Parameters:
**		sz -- size of area to allocate.
**
**	Returns:
**		pointer to data region.
**
**	Side Effects:
**		Memory is allocated.
*/

char *
xalloc(sz)
	register int sz;
{
	register char *p;

	/* some systems can't handle size zero mallocs */
	if (sz <= 0)
		sz = 1;

	p = malloc((unsigned) sz);
	if (p == NULL)
	{
		perror("Out of memory!!");	
	}
	return (p);
}

/*
**  BUILDFNAME -- build full name from gecos style entry.
**
**	This routine interprets the strange entry that would appear
**	in the GECOS field of the password file.
**
**	Parameters:
**		p -- name to build.
**		login -- the login name of this user (for &).
**		buf -- place to put the result.
**
**	Returns:
**		none.
**
**	Side Effects:
**		none.
*/


/*
**  BUILDFNAME -- build full name from gecos style entry.
**
**	This routine interprets the strange entry that would appear
**	in the GECOS field of the password file.
**
**	Parameters:
**		p -- name to build.
**		login -- the login name of this user (for &).
**		buf -- place to put the result.
**
**	Returns:
**		none.
**
**	Side Effects:
**		none.
*/

void
buildfname(gecos, login, buf)
	register char *gecos;
	char *login;
	char *buf;
{
	register char *p;
	register char *bp = buf;
	int l;

	if (*gecos == '*')
		gecos++;

	/* find length of final string */
	l = 0;
	for (p = gecos; *p != '\0' && *p != ',' && *p != ';' && *p != '%'; p++)
	{
		if (*p == '&')
			l += strlen(login);
		else
			l++;
	}

	/* now fill in buf */
	for (p = gecos; *p != '\0' && *p != ',' && *p != ';' && *p != '%'; p++)
	{
	  if (*p == '&')
	    {

	      printf ("strcpy(bp,login)\n");
	      printf ("strlen(bp) = %d strlen(login) = %d\n", 
		      strlen(bp), strlen(login));
	      /*BAD*/
	      (void) strcpy(bp, login);
	      *bp = toupper(*bp);
	      while (*bp != '\0')
		bp++;
	    }
	  else {
	    /*BAD*/
	    *bp++ = *p;
	    printf ("bp-buf = %d\n", (bp-buf));
	  }	    
	}
	*bp = '\0';

	printf("buf can store at most %d bytes; strlen(buf) = %d\n", MAXNAME+1, strlen(buf));
}

/*

</source>

*/

