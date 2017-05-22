
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
$Date: 2004/01/05 17:27:47 $
$Header: /mnt/leo2/cvs/sabo/hist-040105/sendmail/s7/txt-dns-file-ok.c,v 1.1.1.1 2004/01/05 17:27:47 tleek Exp $



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
$Date: 2004/01/05 17:27:47 $
$Header: /mnt/leo2/cvs/sabo/hist-040105/sendmail/s7/txt-dns-file-ok.c,v 1.1.1.1 2004/01/05 17:27:47 tleek Exp $



*/


/*

<source>

*/

#include "txt-dns.h"

/* I believe these are needed for dn_expand to work */ 
#include <netinet/in.h> 
#include <arpa/nameser.h>
#include <resolv.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

char *DNSFILE = "dns-file";
int DNS_REC_LEN;

#if DNSMAP   /* this is the type variable specifying the type of DNS queries we're interested in */
             /* setting DNSMAP to TXT would lead to a potential buffer overflow */
void *xalloc(size_t sz) ;

size_t strlcpy(char *, const char *, size_t); 
int read_record_from_file(unsigned char *, int);

/*
**  DNS_FREE_DATA -- free all components of a DNS_REPLY_T
**
**	Parameters:
**		r -- pointer to DNS_REPLY_T
**
**	Returns:
**		none.
*/

/* modified version of dns_free_data...calls to sm_free() were replaced with free() */

void
dns_free_data(r)
	DNS_REPLY_T *r;
{
	RESOURCE_RECORD_T *rr;

	if (r->dns_r_q.dns_q_domain != NULL)
		free(r->dns_r_q.dns_q_domain);
	for (rr = r->dns_r_head; rr != NULL; )
	{
		RESOURCE_RECORD_T *tmp = rr;

		if (rr->rr_domain != NULL)
			free(rr->rr_domain);
		if (rr->rr_u.rr_data != NULL)
			free(rr->rr_u.rr_data);
		rr = rr->rr_next;
		free(tmp);
	}
	free(r);
}

/*
**  PARSE_DNS_REPLY -- parse DNS reply data.
**
**	Parameters:
**		data -- pointer to dns data
**		len -- len of data
**
**	Returns:
**		pointer to DNS_REPLY_T if succeeded.
**		NULL otherwise.
*/

/* Misha: I've replaced sm_strdup with strdup and sm_srlcpy with strlcpy */ 


static DNS_REPLY_T *
parse_dns_reply(data, len)
	unsigned char *data;
	int len;
{
	unsigned char *p;
	int status, txtlen;
	size_t l;
	char host[MAXHOSTNAMELEN];
	DNS_REPLY_T *r;
	RESOURCE_RECORD_T **rr;

	printf("parse_dns_reply called..\n");

	r = (DNS_REPLY_T *) xalloc(sizeof(*r));
	
	memset(r, 0, sizeof(*r));
	if (r == NULL)
		return NULL;

	p = data;

	/* doesn't work on Crays? */
	memcpy(&r->dns_r_h, p, sizeof(HEADER));
	p += sizeof(HEADER);

	/* status = dn_expand(data, data + len, p, host, sizeof host); */
	strcpy(host, "LL.MIT.EDU");  /* create "compressed domain name" manually */
	status = strlen(host);       /* define status to be len of compressed host name */

        if (status < 0)
	{
		dns_free_data(r);
		return NULL;
	}
	r->dns_r_q.dns_q_domain = (char *) strdup(host);
	if (r->dns_r_q.dns_q_domain == NULL)
	{
		dns_free_data(r);
		return NULL;
	}
	p += status;
	GETSHORT(r->dns_r_q.dns_q_type, p);
        printf("Record type queried = %d\n",r->dns_r_q.dns_q_type);
	GETSHORT(r->dns_r_q.dns_q_class, p);
	rr = &r->dns_r_head;
	while (p < data + len)
	{
		int type, class, ttl, size;

		strcpy(host,"BLAH.MIT.EDU");
		/*status = dn_expand(data, data + len, p, host, sizeof host);*/
                status = strlen(host);
   
		printf("status returned = %d\n", status);
		if (status < 0)
		{
			dns_free_data(r);
			return NULL;
		}
		p += status;
		GETSHORT(type, p);
		GETSHORT(class, p);
		GETLONG(ttl, p);
		GETSHORT(size, p);
		*rr = (RESOURCE_RECORD_T *) xalloc(sizeof(RESOURCE_RECORD_T));
		if (*rr == NULL)
		{
			dns_free_data(r);
			return NULL;
		}
		(*rr)->rr_domain = (char *) strdup(host);
		if ((*rr)->rr_domain == NULL)
		{
			dns_free_data(r);
			return NULL;
		}
		(*rr)->rr_type = type;
		(*rr)->rr_class = class;
		(*rr)->rr_ttl = ttl;
		(*rr)->rr_size = size;
		
		printf("query type = %d\n", type);
		
		switch (type)
		{
		  case T_NS:
		  case T_CNAME:
		  case T_PTR:
			status = dn_expand(data, data + len, p, host,
					   sizeof host);
			if (status < 0)
			{
				dns_free_data(r);
				return NULL;
			}
			(*rr)->rr_u.rr_txt = (char *) strdup(host);
			if ((*rr)->rr_u.rr_txt == NULL)
			{
				dns_free_data(r);
				return NULL;
			}
			break;

		  case T_MX:
		  case T_AFSDB:
			status = dn_expand(data, data + len, p + 2, host,
					   sizeof host);
			if (status < 0)
			{
				dns_free_data(r);
				return NULL;
			}
			l = strlen(host) + 1;
			(*rr)->rr_u.rr_mx = (MX_RECORD_T *)
				xalloc(sizeof(MX_RECORD_T) + l);
			if ((*rr)->rr_u.rr_mx == NULL)
			{
				dns_free_data(r);
				return NULL;
			}
			(*rr)->rr_u.rr_mx->mx_r_preference = (p[0] << 8) | p[1];
			(void) strlcpy((*rr)->rr_u.rr_mx->mx_r_domain,
					  host, l);
			break;

		  case T_SRV:
			status = dn_expand(data, data + len, p + 6, host,
					   sizeof host);
			if (status < 0)
			{
				dns_free_data(r);
				return NULL;
			}
			l = strlen(host) + 1;
			(*rr)->rr_u.rr_srv = (SRV_RECORDT_T*)
				xalloc(sizeof(SRV_RECORDT_T) + l);
			if ((*rr)->rr_u.rr_srv == NULL)
			{
				dns_free_data(r);
				return NULL;
			}
			(*rr)->rr_u.rr_srv->srv_r_priority = (p[0] << 8) | p[1];
			(*rr)->rr_u.rr_srv->srv_r_weight = (p[2] << 8) | p[3];
			(*rr)->rr_u.rr_srv->srv_r_port = (p[4] << 8) | p[5];
			(void) strlcpy((*rr)->rr_u.rr_srv->srv_r_target,
					  host, l);
			break;

		  case T_TXT:
		    
		        printf("We are in the T_TXT switch block\n");
		       
 			/**  The TXT record contains the length as
 			**  leading byte, hence the value is restricted
 			**  to 255, which is less than the maximum value
 			**  of RDLENGTH (size). Nevertheless, txtlen
 			**  must be less than size because the latter
 			**  specifies the length of the entire TXT
 			**  record.
 			*/

		        txtlen = *p;

			(*rr)->rr_u.rr_txt = (char *) xalloc(txtlen + 1);  /* alloc txtlen + 1 instead of size + 1 */
			if ((*rr)->rr_u.rr_txt == NULL)
			{
				dns_free_data(r);
				return NULL;
			}
			printf("Copying TXT record!!!\n");
			printf("Trying to copy %d bytes into a buffer of size %d...\n", *p, size+1);
				
			if (txtlen >= size)
 			{
			  printf("An error occurred!: DNS TXT record size = %d <= txtlen=%d\n", size, txtlen);
		      	  /* dns_free_data(r); */
			  return NULL;
 			}
			
			/* OK */
			(void) strncpy((*rr)->rr_u.rr_txt, (char*) p + 1, *p);  /* this should be safe */
			/*OK*/
			(*rr)->rr_u.rr_txt[*p] = 0;
			break;

		  default:
			(*rr)->rr_u.rr_data = (unsigned char*) xalloc(size);
			if (size != 0 && (*rr)->rr_u.rr_data == NULL)
			{
				dns_free_data(r);
				return NULL;
			}
			(void) memcpy((*rr)->rr_u.rr_data, p, size);
		}
		p += size;
		rr = &(*rr)->rr_next;
	}
	*rr = NULL;
	return r;
}


/*
** MISHA ZITSER: Instead of querying a DNS server, read info from file 
**  DNS_LOOKUP_INT -- looks up DNS record information in a file 
**
**	Parameters:
**		domain -- name to lookup
**		rr_class -- resource record class
**		rr_type -- resource record type
**		retrans -- retransmission timeout
**		retry -- number of retries
**
**	Returns:
**		result of lookup if succeeded.
**		NULL otherwise.
*/

DNS_REPLY_T *
dns_lookup_int(domain, rr_class, rr_type, retrans, retry)
	const char *domain;
	int rr_class;
	int rr_type;
	time_t retrans;
	int retry;
{
	int len;
	time_t save_retrans = 0;
	int save_retry = 0;
	DNS_REPLY_T *r = NULL;
	unsigned char reply[1024];

	if (retrans > 0)
	{
		save_retrans = _res.retrans;
		_res.retrans = retrans;
	}
	if (retry > 0)
	{
		save_retry = _res.retry;
		_res.retry = retry;
	}
	
	if (rr_type == T_TXT){
	  printf("Reading from file = %s ...\n", DNSFILE);
	  len = read_record_from_file(reply, sizeof(reply));  /* read TXT record from file */
	  printf("read_record_from_file returned len = %d\n", len);
	}
	else
	  len = res_search(domain, rr_class, rr_type, reply, sizeof reply);


	if (len >= 0)
	  r = parse_dns_reply(reply, len);

	if (retrans > 0)
	  _res.retrans = save_retrans;

  	if (retry > 0)
	  _res.retry = save_retry;

	return r;
}


int read_record_from_file(unsigned char *reply, int size){
  
  FILE *f; 
  int i;
  unsigned char *temp;  
  
  temp = reply;

  if (DNS_REC_LEN > size)
    return -1;                      /* record too big */
  else{
    f = fopen(DNSFILE, "r");

    /* read in TXT record data */

    for(i=0; i<DNS_REC_LEN; i++) 
      *temp++ = (unsigned char) fgetc(f); 
  
    fclose(f);
  }

  return i;
}


int create_dns_file(){

  FILE *f;
  unsigned char buf[200];
  unsigned char *p;
  char *temp;
  int i,len = 0;

  p = buf;

  temp = "HEADER JUNK:";
  len += strlen(temp);

  while (*temp != '\0') 
    *p++ = *temp++;
  
  temp = "LL.MIT.EDU";
  len += strlen(temp);
 
  while (*temp != '\0') 
    *p++ = *temp++;
    
  *p++ = 0;
  *p++ = 16;  /* type = T_TXT = 16 */
  *p++ = 0;
  *p++ = 1;   /* class = C_IN or 1 */

  len += 4;

  temp = "BLAH.MIT.EDU";
  len += strlen(temp);
 
  while (*temp != '\0') 
    *p++ = *temp++;
  
  *p++ = 0;
  *p++ = 16;  /* T_TXT type */
  
  *p++ = 0;
  *p++ = 1;  /* C_IN class */

  *p++ = 0;
  *p++ = 0;
  *p++ = 0;
  *p++ = 255;  /* ttl = 255 */

  *p++ = 0;
  *p++ = 30;  /* bogus size provided by bogus dns server */
  *p++ = 255;  /* txtlen = 255... this is a bogus txtlen.. txtlen should be < size */
  
  len += 11;

  temp = "This is random junk in the TXT record that will overflow (*rr)->rr_u.rr_txt";
  len += strlen(temp);

  while (*temp != '\0') 
    *p++ = *temp++;

  f = fopen(DNSFILE, "w");

  p = buf;
  for(i=0; i<len; i++, p++)
    fputc(*p, f);
  
  fclose(f);
  
  return len;
}


int main(){

  const char *domain = "LL.MIT.EDU";
  int rr_class = C_IN;      
  int rr_type = T_TXT;  /* query DNS server for a TXT record about some domain (we will instead read from a file*/
  time_t retrans = 10;
  int retry = 1;
  DNS_REPLY_T * dns_reply;
  
  DNS_REC_LEN = create_dns_file();

  res_init();

  dns_reply =  dns_lookup_int(domain, rr_class, rr_type, retrans, retry);
  
  /*dns_domain = dns_reply->dns_r_q.dns_q_domain; */
  /*printf("dns_domain = %s\n", dns_domain); */
   
  return 0;
}
#endif /*DNSMAP*/

/*

</source>

*/

