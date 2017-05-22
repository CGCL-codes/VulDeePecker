
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
$Date: 2004/01/05 17:27:42 $
$Header: /mnt/leo2/cvs/sabo/hist-040105/sendmail/s2/recipient-ok.c,v 1.1.1.1 2004/01/05 17:27:42 tleek Exp $



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
$Date: 2004/01/05 17:27:42 $
$Header: /mnt/leo2/cvs/sabo/hist-040105/sendmail/s2/recipient-ok.c,v 1.1.1.1 2004/01/05 17:27:42 tleek Exp $



*/


/*

<source>

*/

#include <stdio.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/*
  #include <unistd.h>
*/
#include "sendmail-ok.h"

enum bool {false, true};
enum bool  MatchGecos = 1; 
char  SpaceSub = ' ';
int MaxAliasRecursion = 0;

/*
**  RECIPIENT -- Designate a message recipient
**
**	Saves the named person for future mailing.
**
**	Parameters:
**		a -- the (preparsed) address header for the recipient.
**		sendq -- a pointer to the head of a queue to put the
**			recipient in.  Duplicate supression is done
**			in this queue.
**		aliaslevel -- the current alias nesting depth.
**		
**
**	Returns:
**		The actual address in the queue.  This will be "a" if
**		the address is not a duplicate, else the original address.
**
**	Side Effects:
**		none.
*/

/* i have cut out the envelope argument to recipient..*/

ADDRESS *
recipient(a, sendq, aliaslevel)
     register ADDRESS *a;
     register ADDRESS **sendq;
     int aliaslevel;
{

  register char *p;
  enum bool quoted = FALSE;		/* set if the addr has a quote bit */
  int i;
  char *buf;
  char buf0[MAXNAME + 1];		/* unquoted image of the user name */

	
  if (aliaslevel == 0)
    a->q_flags |= QPRIMARY; 
	

  /* break aliasing loops */
  if (aliaslevel > MaxAliasRecursion)
    {
      a->q_status = "5.4.6";
      printf("554 aliasing/forwarding loop broken (%d aliases deep; %d max",
	     aliaslevel, MaxAliasRecursion);
      return (a);
    }

  /*
  **  Finish setting up address structure.
  */

  /* get unquoted user for file, program or user.name check */
  i = strlen(a->q_user);
  if (i >= sizeof buf0)
    buf = xalloc(i + 1);
  else
    buf = buf0;
  (void) strcpy(buf, a->q_user);
	
  for (p = buf; *p != '\0' && !quoted; p++)
    {
      if (*p == '\\')
	quoted = TRUE;
    }

  /* this is a modified nested-if block */

  if (TRUE) {
    auto enum bool fuzzy;
    register struct passwd *pw;
    extern struct passwd *finduser();

    /* warning -- finduser may trash buf */
    pw = finduser(buf, &fuzzy);
    printf ("back from finduser\n");
    if (pw == NULL)
      {
	a->q_flags |= QBADADDR;
	a->q_status = "5.1.1";		
      }
    else
      {
	char test_buf[10];  /* use as canary to see if nbuf gets overflowed */
	char nbuf[MAXNAME + 1];

	strcpy(test_buf, "GOOD");
		
	if (strcmp(pw->pw_dir, "/") == 0)
	  a->q_home = "";
	else
	  a->q_home = newstr(pw->pw_dir);
	a->q_uid = pw->pw_uid;
	a->q_gid = pw->pw_gid;
	a->q_ruser = newstr(pw->pw_name);
	a->q_flags |= QGOODUID;
			
	printf("Before call to builfname, pw_gecos = %s, and pw_name = %s\n", pw->pw_gecos, pw->pw_name);
	printf("nbuf before call to buildfname = %s\n", nbuf);		

	printf ("sizeof(nbuf) = %d\n", sizeof(nbuf));
	/*OK*/
	buildfname(pw->pw_gecos, pw->pw_name, nbuf, sizeof(nbuf));
	printf ("back from 1st call to buildfname\n");

	//			printf("nbuf after call to buildfname = %s\n", nbuf);
	if (nbuf[0] != '\0')
	  a->q_fullname = newstr(nbuf);			
			
	printf("test_buf should be GOOD.  test_buf = %s\n", test_buf);
      }
  }

  /*done:*/
  a->q_flags |= QTHISPASS;
  if (buf != buf0)
    free(buf);

  return (a);
}



/*
**  FINDUSER -- find the password entry for a user.
**
**	This looks a lot like getpwnam, except that it may want to
**	do some fancier pattern matching in /etc/passwd.
**
**	This routine contains most of the time of many sendmail runs.
**	It deserves to be optimized.
**
**	Parameters:
**		name -- the name to match against.
**		fuzzyp -- an outarg that is set to TRUE if this entry
**			was found using the fuzzy matching algorithm;
**			set to FALSE otherwise.
**
**	Returns:
**		A pointer to a pw struct.
**		NULL if name is unknown or ambiguous.
**
**	Side Effects:
**		may modify name.
*/

struct passwd *
finduser(name, fuzzyp)
     char *name;
     enum bool *fuzzyp;
{
  register struct passwd *pw;
  register char *p;
  enum bool tryagain;	

  *fuzzyp = FALSE;

#ifdef HESIOD
  /* DEC Hesiod getpwnam accepts numeric strings -- short circuit it */
  for (p = name; *p != '\0'; p++)
    if (!isascii((int)*p) || !isdigit((int)*p))
      break;
  if (*p == '\0')
    {	
      return NULL;
    }
#endif

  /* look up this login name using fast path */
  pw = getpwnam(name);


  if (pw != NULL)
    {		
      return (pw);
    }

  /* try mapping it to lower case */
  tryagain = FALSE;
  for (p = name; *p != '\0'; p++)
    {
      if (isascii((int)*p) && isupper((int)*p))
	{
	  *p = tolower(*p);
	  tryagain = TRUE;
	}
    }

  pw = getpwnam(name);
  
  if (tryagain && pw!= NULL)
    {	
      *fuzzyp = TRUE;
      return pw;
    }

#if MATCHGECOS
  /* see if fuzzy matching allowed */
  if (!MatchGecos)
    {
      return NULL;
    }

  /* search for a matching full name instead */
  for (p = name; *p != '\0'; p++)
    {
      if (*p == (SpaceSub & 0177) || *p == '_')
	*p = ' ';
    }
  (void) setpwent();
  
  pw = getpwent();

  
  while (pw != NULL ) {

    char buf[MAXNAME + 1];
    
# if 0
    if (strcasecmp(pw->pw_name, name) == 0)      {
      *fuzzyp = TRUE;
      return pw;
    }
# endif
    
    /*OK*/
    printf ("sizeof(buf) = %d\n", sizeof(buf));
    buildfname(pw->pw_gecos, pw->pw_name, buf, sizeof(buf));
    printf ("back from 2nd call to buildfname\n");
    if (strchr(buf, ' ') != NULL && !strcasecmp(buf, name))      {
      *fuzzyp = TRUE;
      return (pw);
    }
  
    pw = getpwent();
  }
  

#endif
  return (NULL);
}







/*

</source>

*/

