
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
$Date: 2004/02/04 20:14:32 $
$Header: /mnt/leo2/cvs/sabo/hist-040105/bind/b2/sig-ok.c,v 1.2 2004/02/04 20:14:32 tleek Exp $



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
$Date: 2004/02/04 20:14:32 $
$Header: /mnt/leo2/cvs/sabo/hist-040105/bind/b2/sig-ok.c,v 1.2 2004/02/04 20:14:32 tleek Exp $



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



#ifdef CCURED
extern char *p_secstodate (u_long s);
#pragma ccuredwrapper("p_secstodate_wrapper", of("p_secstodate"))

char *p_secstodate (u_long s) {
  char *res;

  res = p_secstodate(s);
  return (__mkptr_string(res));
}
#endif


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
	int n, len=0;
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
	printf("C_IN = %d. class = %d, CLASS_MAX = %d\n", C_IN, class, CLASS_MAX);
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

	printf("dlen = %d\n", dlen);
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
	  
	case T_SIG: {
		u_long origTTL, exptime, signtime, timetilexp, now;
		u_int8_t alg;
		u_int temp;
				
		/* Check signature time, expiration, and adjust TTL.  */
		/* This code is similar to that in db_load.c.  */

		/* Skip coveredType, save alg, skip labels */
		BOUNDS_CHECK(cp, INT16SZ + 1 + 1 + 3*INT32SZ);
		
                cp1 = cp; 		

		GETSHORT(temp, cp1);     /* get covered type*/
		printf("covered type = %d\n", temp);
		cp1 += 2;
		len += 2;

		GETSHORT(temp, cp1);   
		printf(" alg label = %d\n", temp);
		alg = temp/256;       /* save alg */
		cp1 += 2;             /* skip over alg and labels */ 
		len += 2;

		printf("alg = %d\n", alg);
	
		GETLONG(origTTL, cp1);
		cp1 += 4;
		len += 4;

		printf("origttl = %d\n", (int) origTTL);
		GETLONG(exptime, cp1);
		printf("Exptime = %d\n", (int) exptime);
		cp1 += 4;
		len += 4;

		GETLONG(signtime, cp1);
		printf("Sign time = %d\n", (int) signtime);
		cp1 += 4; 
		len += 4;
		
		GETSHORT(temp, cp1);
		printf("random footprint = %d\n", temp); 
		cp1 += 2;
		len += 2;
	
		now = time(NULL);	/* Get current time in GMT/UTC */

		printf("now = %d\n",(int) now);
		/* Don't let bogus name servers increase the signed TTL */
		if (ttl > origTTL) {
		  printf( "shrinking SIG TTL");
				 
			ttl = origTTL;
		}

		/* Don't let bogus signers "sign" in the future.  */
		if (signtime > now) {
		  printf("ignoring SIG: signature date %s is in the future", p_secstodate (signtime));
			return ((cp - rrp) + dlen);
		}
		
		/* Ignore received SIG RR's that are already expired.  */
		if (exptime <= now) {
		  printf("ignoring SIG: expiration %s is in the past",
			 p_secstodate (exptime));
		  return ((cp - rrp) + dlen);
		}

		/* Lop off the TTL at the expiration time.  */
		timetilexp = exptime - now;
		if (timetilexp < ttl) {
		  printf("shrinking expiring SIG TTL");
			
			ttl = timetilexp;
		}

		cp = cp1 - 18;

		/* The following code is copied from named-xfer.c.  */
		cp1 = (u_char *)data;

		/* first just copy over the type_covered, algorithm, */
		/* labels, orig ttl, two timestamps, and the footprint */
		BOUNDS_CHECK(cp, 18);
    		memcpy(cp1, cp, 18);
		
		cp1 += 18;
		
		/* then the signer's name */
		printf("sizeof data = %d\n", sizeof data);
		printf("comp name = %s\n", (char *) cp+18);
	
	     	n = dn_expand(msg, eom, cp+18, (char *)cp1, (sizeof data) - 18);
		
	/* n is the length of the compressed domain name as seen in msg*/

		printf("dn_expand returned:  %d, expanded name = %s\n", n, (char *)cp1);

		if (n < 0 || n + NS_SIG_SIGNER > dlen) {
		  printf("ERROR: Either n = %d < 0 or n+ns_sig_signer = %d > dlen = %d!\n", n, n+NS_SIG_SIGNER, dlen);  
		  printf("EXITING RREXTRACT!\n");
		  hp->rcode = FORMERR;
		  return (-1);
		}
		cp += n;
		cp1 += strlen((char*)cp1)+1;

		/* finally, we copy over the variable-length signature.
		   Its size is the total data length, minus what we copied. */
		n = dlen - (NS_SIG_SIGNER + n);
		printf("dlen - NS_SIG_SIGNER - n = %d\n", n);
		
		/* make sure we have room for signature */
		if (n >  (sizeof data) - (cp1 - (u_char *)data)) {
			hp->rcode = FORMERR;
			return (-1);  /* out of room! */
		}

		switch (alg) {
		    case NS_ALG_MD5RSA:
			if (n < NS_MD5RSA_MIN_SIZE || n > NS_MD5RSA_MAX_SIZE)
				hp->rcode = FORMERR;
			break;

		    case NS_ALG_DSA:
			if (n != NS_DSA_SIG_SIZE)
				hp->rcode = FORMERR;
			break;

		    default:
		      printf("DEFAULT ALG!\n");
			break;
		}

		if (hp->rcode == FORMERR)
			return (-1);

		printf("MEMCPY: Copying %u bytes into data buf!\n", n);

		/* OK */
		memcpy(cp1, cp, n);
		cp += n;
		cp1 += n;
		
		/* compute size of data */
		n = cp1 - (u_char *)data;
		cp1 = (u_char *)data;
		break;
	    }
	
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




/* create the signature file this model needs */

int createSig (u_char *buf) {
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

  PUTSHORT(24, p); /* type = T_SIG = 24 */
  p += 2;    
  
  PUTSHORT(C_IN, p);   /* class = C_IN = 1*/
  p += 2;

  PUTLONG(255, p);  /* ttl */
  p += 4;

  PUTSHORT(30, p);  /* dlen = len of everything starting with the covered byte (the length 
			of the entire resource record... we lie about it
		   */
  p += 2;

  len += 10;

  PUTSHORT(15, p);  /* covered type */
  p += 2;
    
  PUTSHORT(256*2, p);  /* algorithm and labels.. MAKE ALG = 2,i.e default ALG*/
  p += 2;
  
  PUTLONG(255, p);  /* orig ttl */
  p += 4;

  now = time(NULL);  
  
  printf("Signing at = %d\n", now);
  PUTLONG(now+20000, p);   /* expiration time */
  p += 4;
  PUTLONG(now, p);         /* time signed */  
  p += 4;

  PUTSHORT(100, p);            /* random key footprint */
  p += 2;
  
  len += 18;

  strcpy(exp_dn2, "sls.lcs.mit.edu"); /* signer */

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

  for(i=0; i<11; i++)
  {  
    PUTLONG(123, p);           /* fake signature */
    p += 4;
    len += 4;
  }
  
  return (p-buf);
 
}



int main(){
  
  int msglen, ret;
  u_char *dp;
  u_char *name = (u_char *) malloc(100*sizeof(u_char));
  u_char *msg = (u_char *) malloc(1000 * sizeof(u_char));

  printf("Calling create_msg:\n");
  msglen = createSig(msg);

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

