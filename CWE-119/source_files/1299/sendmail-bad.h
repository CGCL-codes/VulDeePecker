
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
$Header: /mnt/leo2/cvs/sabo/hist-040105/sendmail/s2/sendmail-bad.h,v 1.1.1.1 2004/01/05 17:27:42 tleek Exp $



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
$Header: /mnt/leo2/cvs/sabo/hist-040105/sendmail/s2/sendmail-bad.h,v 1.1.1.1 2004/01/05 17:27:42 tleek Exp $



*/


/*

<source>

*/

# include <unistd.h>
# include <stddef.h>
# include <stdlib.h>
# include <stdio.h>
# include <ctype.h>

extern int MaxAliasRecursion;	/* maximum depth of alias recursion */
extern char  *xalloc(int);
extern void buildfname(register char *, char *, char *);


/* slightly modified address structure */
struct address   
{
	char		*q_paddr;	/* the printname for the address */
	char		*q_user;	/* user name */
	char		*q_ruser;	/* real user name, or NULL if q_user */
	char		*q_host;	/* host name */	
	u_long		q_flags;	/* status flags, see below */
	uid_t		q_uid;		/* user-id of receiver (if known) */
	gid_t		q_gid;		/* group-id of receiver (if known) */
	char		*q_home;	/* home dir (local mailer only) */
	char		*q_fullname;	/* full name if known */
	struct address	*q_next;	/* chain */
	struct address	*q_alias;	/* address this results from */
	char		*q_owner;	/* owner of q_alias */
	struct address	*q_tchain;	/* temporary use chain */
	char		*q_orcpt;	/* ORCPT parameter from RCPT TO: line */
	char		*q_status;	/* status code for DSNs */
	char		*q_rstatus;	/* remote status message for DSNs */
	time_t		q_statdate;	/* date of status messages */
	char		*q_statmta;	/* MTA generating q_rstatus */
	short		q_specificity;	/* how "specific" this address is */
};

typedef struct address ADDRESS;

extern ADDRESS *recipient(register ADDRESS*, register ADDRESS **, int);

/* modified envelope struct */
struct envelope
{
	time_t		e_ctime;	/* time message appeared in the queue */
	char		*e_to;		/* the target person */
	ADDRESS		e_from;		/* the person it is from */       
	long		e_flags;	/* flags, see below */
	int		e_nrcpts;	/* number of recipients */
	char		*e_id;		/* code for this entry in queue */
	FILE		*e_xfp;		/* transcript file */
	FILE		*e_lockfp;	/* the lock file for this message */
	char		*e_message;	/* error message */
};


typedef struct envelope	ENVELOPE;



# define QDONTSEND	0x00000001	/* don't send to this address */
# define QBADADDR	0x00000002	/* this address is verified bad */
# define QGOODUID	0x00000004	/* the q_uid q_gid fields are good */
# define QPRIMARY	0x00000008	/* set from RCPT or argv */
# define QQUEUEUP	0x00000010	/* queue for later transmission */
# define QSENT		0x00000020	/* has been successfully delivered */
# define QNOTREMOTE	0x00000040	/* address not for remote forwarding */
# define QSELFREF	0x00000080	/* this address references itself */
# define QVERIFIED	0x00000100	/* verified, but not expanded */
# define QBOGUSSHELL	0x00000400	/* user has no valid shell listed */
# define QUNSAFEADDR	0x00000800	/* address aquired via unsafe path */
# define QPINGONSUCCESS	0x00001000	/* give return on successful delivery */
# define QPINGONFAILURE	0x00002000	/* give return on failure */
# define QPINGONDELAY	0x00004000	/* give return on message delay */
# define QHASNOTIFY	0x00008000	/* propogate notify parameter */
# define QRELAYED	0x00010000	/* DSN: relayed to non-DSN aware sys */
# define QEXPANDED	0x00020000	/* DSN: undergone list expansion */
# define QDELIVERED	0x00040000	/* DSN: successful final delivery */
# define QDELAYED	0x00080000	/* DSN: message delayed */
# define QTHISPASS	0x80000000	/* temp: address set this pass */

# define NULLADDR	((ADDRESS *) NULL)

# define MAXNAME         4

# define MATCHGECOS     1       /* match user names from gecos field */


#define TRUE 1
#define FALSE 0

/* make a copy of a string */
#define newstr(s)	strcpy(xalloc(strlen(s) + 1), s)

# define bitset(bit, word)     (((word) & (bit)) != 0)

/*

</source>

*/

