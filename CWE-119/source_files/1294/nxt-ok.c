
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
$Date: 2004/02/05 16:47:21 $
$Header: /mnt/leo2/cvs/sabo/hist-040105/bind/b1/nxt-ok.c,v 1.2 2004/02/05 16:47:21 tleek Exp $



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
$Date: 2004/02/05 16:47:21 $
$Header: /mnt/leo2/cvs/sabo/hist-040105/bind/b1/nxt-ok.c,v 1.2 2004/02/05 16:47:21 tleek Exp $



*/


/*

<source>

*/

/* This is rrextract taken from BIND 8.2 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <sys/un.h>

#include <netinet/in.h>
#include <arpa/nameser.h>
#include <arpa/inet.h>

#include <errno.h>
#include <limits.h>
#include <resolv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <time.h>
#include <assert.h>
#include "ns_defs.h"


#define NS_SIG_SIGNER 18 /* Domain name of who signed it */

#define BOUNDS_CHECK(ptr, count) \
	do { \
		if ((ptr) + (count) > eom) { \
			hp->rcode = FORMERR; \
			return (-1); \
		} \
	} while (0)

const char *transport_strings[] = { "primary", "secondary", "response", NULL };

#define PERIOD 0x2e
#define	hyphenchar(c) ((c) == 0x2d)
#define bslashchar(c) ((c) == 0x5c)
#define periodchar(c) ((c) == PERIOD)
#define asterchar(c) ((c) == 0x2a)
#define alphachar(c) (((c) >= 0x41 && (c) <= 0x5a) \
		   || ((c) >= 0x61 && (c) <= 0x7a))
#define digitchar(c) ((c) >= 0x30 && (c) <= 0x39)

#define borderchar(c) (alphachar(c) || digitchar(c))
#define middlechar(c) (borderchar(c) || hyphenchar(c))
#define domainchar(c) ((c) > 0x20 && (c) < 0x7f)

/*
 * This function is quite liberal, since RFC 1034's character sets are only
 * recommendations.
 */
int
res_dnok(const char *dn) {
	int ch;

	while ((ch = *dn++) != '\0')
		if (!domainchar(ch))
			return (0);
	return (1);
}

int
res_hnok(const char *dn) {
	int ppch = '\0', pch = PERIOD, ch = *dn++;

	while (ch != '\0') {
		int nch = *dn++;

		if (periodchar(ch)) {
			(void)NULL;
		} else if (periodchar(pch)) {
			if (!borderchar(ch))
				return (0);
		} else if (periodchar(nch) || nch == '\0') {
			if (!borderchar(ch))
				return (0);
		} else {
			if (!middlechar(ch))
				return (0);
		}
		ppch = pch, pch = ch, ch = nch;
	}
	return (1);
}


enum context
ns_ownercontext(type, transport)
	int type;
	enum transport transport;
{
	enum context context = domain_ctx;

	switch (type) {
	case T_A:
	case T_WKS:
	case T_MX:
		switch (transport) {
		case update_trans:
		case primary_trans:
		case secondary_trans:
			context = owner_ctx;
			break;
		case response_trans:
			context = hostname_ctx;
			break;
		default:
			printf("impossible condition in ns_ownercontext()"); 
			      
		}
		break;
	case T_MB:
	case T_MG:
		context = mailname_ctx;
		break;
	default:
		/* Nothing to do. */
		break;
	}
	return (context);
}




/*MZ: modified newstr function */

/*
 * Return a counted string buffer big enough for a string of length 'len'.
 */
char *
newstr(size_t len, int needpanic) {
	u_char *buf, *bp;

	assert(len <= 65536);

	buf = (u_char *)malloc(2 + len + 1);
	if (buf == NULL) {
		if (needpanic)
			printf("savestr: malloc failed (%s)", strerror(errno));
		else
			return (NULL);
	}
	bp = buf;
	NS_PUT16(len, bp);
	return ((char *)bp);
}


/*MZ: Modified ns_nameok.. we only deal with some context cases */

int
ns_nameok(const char *name, int class, struct zoneinfo *zp,
	  enum transport transport,
	  enum context context,
	  const char *owner)
{

	enum severity severity = not_set;
	int ok = 1;

	if (zp != NULL)
		severity = zp->z_checknames;

	/*
	if (severity == not_set)
	  {
	    printf("severity must be set!\n");
	    return (0);
	  }     
	*/

	if (severity == ignore)
		return (1);
	switch (context) {
	case domain_ctx:
		ok = (class != C_IN) || res_dnok(name);
		break;
	case hostname_ctx:
		ok = res_hnok(name);
		break;
	default:
	  {
	    printf("unexpected context %d in ns_nameok", (int)context);
	    return 0;
	  }
	  
	  return (ok);
	}

	return (ok);
  }	
/* rrp points to the resource record */

static int
rrextract(u_char *msg, int msglen, u_char *rrp, u_char *dname, int namelen)
{
	u_char *eom, *cp, *cp1, *rdatap;
	u_int class, type, dlen;
	int n, n1, n2, len=0;
	u_int32_t ttl;
	u_char data[MAXDATA*2];   /* sizeof data = 2 (2 * 1025 + 5*4) = 4140 */
	HEADER *hp = (HEADER *)msg;
		
	cp = rrp;
	eom = msg + msglen;

	printf("Trying to do dn_expand..\n");
	printf("msg = %s, msglen = %d, rrp = %s, namelen = %d\n", (char *) msg, msglen, (char *)rrp, namelen);

	if ((n = dn_expand(msg, eom, cp, (char *) dname, namelen)) < 0) {
	  printf("dn_expand returned %d\n", n);
	  hp->rcode = FORMERR;
	  return (-1);
	}
	
	printf("First dn_expand returned n = %d\n", n);

	cp += n;
	len += n;
	len += sizeof(HEADER);

	BOUNDS_CHECK(cp, 2*INT16SZ + INT32SZ + INT16SZ);
	GETSHORT(type, cp);
	cp += 2;
	len += 2;
	printf("type = %d\n", type);
	GETSHORT(class, cp);
	cp += 2;
	len += 2;
	
	if (class > CLASS_MAX) {
	  printf("bad class in rrextract");
	  hp->rcode = FORMERR;
	  return (-1);
	}
	GETLONG(ttl, cp);
	printf("ttl = %d\n",ttl); 
	cp += 4;
	len += 4;

	if (ttl > MAXIMUM_TTL) {
	  printf("%s: converted TTL > %u to 0",
		 dname, MAXIMUM_TTL);		

		ttl = 0;
	}
	GETSHORT(dlen, cp);
	cp += 2; 
	len += 2;

	printf("msglen - len - 12 = %d, dlen = %d\n", msglen - len - 12, dlen);
	BOUNDS_CHECK(cp, dlen);
	printf("bounds check succeeded\n");
	rdatap = cp;

	if (!ns_nameok((char *)dname, class, NULL, response_trans,
		       ns_ownercontext(type, response_trans),
		      (char *)dname)) {
		hp->rcode = REFUSED;
		return (-1);
	}
	printf("rrextract: dname %s type %d class %d ttl %d\n",
		 dname, type, class, ttl);
	/*
	 * Convert the resource record data into the internal
	 * database format.
	 *
	 * On entry to the switch:
	 *   CP points to the RDATA section of the wire-format RR.
	 *   DLEN is its length.
	 *   The memory area at DATA is available for processing.
	 * 
	 * On exit from the switch:
	 *   CP has been incremented past the RR.
	 *   CP1 points to the RDATA section of the database-format RR.
	 *   N contains the length of the RDATA section of the dbase-format RR.
	 *
	 * The new data at CP1 for length N will be copied into the database,
	 * so it need not be in any particular storage location.
	 */
	switch (type) {
	case T_A:
	  if (dlen != INT32SZ) {
		  hp->rcode = FORMERR;
		  return (-1);
	  }
	  /*FALLTHROUGH*/
	  break;
		
		
	case T_NAPTR:
	  printf("NOT HANDLING T_NAPTR RECORDS\n");
	  break;

	  /* Many cases were cut out from this program for simplicity's
	     sake 
	  */
	  
	case T_MX:
	case T_AFSDB:
	case T_RT:
	case T_SRV:
	  printf("NOT HANDLING T_SRV RECORDS\n");
	  break;
	  
	case T_PX:
	  printf("NOT HANDLING T_PX RECORDS\n");
	  break;
	  
	
	case T_NXT:
	       /* put domain name into data buf */
	  printf("Inside NXT block!\n");
	  	n = dn_expand(msg, eom, cp, (char *)data, sizeof data);
		printf("dn_expand returned n = %d\n", n);
		printf("Compressed name = %s, Expanded name = %s\n", cp, data);

		/*
		 * By testing if n >= dlen, we are requiring that the type
		 * bitmap be at least one octet.  This is reasonable
		 * because we always have to look at the 0 bit to see if
		 * this is a "different format" NXT or not.
		 */
		if (n < 0 || n >= dlen) {
		  printf("n=%d >= dlen = %d\n", n, dlen);
		  printf("dlen is FAKE!!\n");
		  hp->rcode = FORMERR;
		  return (-1);
		}
		if (!ns_nameok((char *) data, class, NULL, response_trans, domain_ctx, (char *) dname)) {
		  printf("Name not good!\n");
		  hp->rcode = FORMERR;
		  return (-1);
		}
		cp += n;
		n1 = strlen((char *)data) + 1;
		cp1 = data + n1;
		/*
		 * We don't need to BOUNDS_CHECK() cp here because we've
		 * previously checked that 'dlen' bytes are in bounds, and
		 * we know that n < dlen.
		 */
		n2 = dlen - n;
		
		printf("n2 = %d\n", n2);
		/*
		 * The first bit of the first octet determines the format
		 * of the NXT record.  A format for types >= 128 has not
		 * yet been defined, so if bit zero is set, we just copy
		 * what's there because we don't understand it.
		 */

		printf("*cp = %d\n", *cp);
		if ((*cp & 0x01) == 0) {
			/*
			 * Bit zero is not set; this is an ordinary NXT
			 * record.  The bitmap must be at least 4 octets
			 * because the NXT bit should be set.  It should be
			 * less than or equal to 16 octets because this NXT
			 * format is only defined for types < 128.
			 */
			if (n2 < 4 || n2 > 16) {
			  printf("n2<4 || n2 > 16\n");
			  hp->rcode = FORMERR;
			  return (-1);
			}
		}
		
		/* check to see if the number of bytes */ 
		 /* remaining to be copied fit */

		printf("sizeof data - n1 = %d\n", sizeof data - n1); 
		if (n2 > sizeof data - n1) 
		  {
		    printf("Want to copy %d bytes, but can only fit %d\n", n2, sizeof data - n1);
		    hp->rcode = FORMERR;
			return (-1);
		  }
		
		printf("Copying %d bytes from cp to cp1\n", n2);
		printf("Cp1 has room for %d bytes\n", sizeof data - n1);
		/* OK */           
		memcpy(cp1, cp, n2);
		cp += n2;

		/* compute size of data */
		n = cp1 - (u_char *)data;
		cp1 = (u_char *)data;
	  
	  break; 


	default:
	  printf("unknown type %d", type);
	  return ((cp - rrp) + dlen);
	}

	if (cp > eom) {
		hp->rcode = FORMERR;
		return (-1);
	}
	if ((u_int)(cp - rdatap) != dlen) {
	  
	  printf("encoded rdata length is %u, but actual length was %u",
		 dlen, (u_int)(cp - rdatap));
	  hp->rcode = FORMERR;
	  
	  return (-1);
	}
	if (n > MAXDATA) {
	  printf("update type %d: %d bytes is too much data",
		 type, n);
	  
	  hp->rcode = FORMERR;
	  return (-1);
	}

	
	/* We don't need to save it for our test program */
	/* *dpp = savedata(class, type, ttl, cp1, n); */
	return (cp - rrp);
}


int create_msg(u_char *buf) {

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
   
  return (len);
}




int main(){
  
  int msglen, ret;
  u_char *dp;
  u_char *name = (u_char *) malloc(100*sizeof(u_char));
  u_char *msg = (u_char *) malloc(1000 * sizeof(u_char));

  printf("Calling create_msg:\n");
  msglen = create_msg(msg);

  printf("msglen = %d\n", msglen);

  dp = msg + sizeof(HEADER); /* point dp past the header (i.e to the hostname)*/ 

  printf("Calling rrextract!\n");

  res_init();
  ret = rrextract(msg, msglen, dp, name, 100);
  
  printf("rrextract returned %d\n", ret); 
  
  return 0;
  
}

/*

</source>

*/

