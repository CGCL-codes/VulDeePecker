
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
$Header: /mnt/leo2/cvs/sabo/hist-040105/sendmail/s7/txt-dns.h,v 1.1.1.1 2004/01/05 17:27:47 tleek Exp $



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
$Header: /mnt/leo2/cvs/sabo/hist-040105/sendmail/s7/txt-dns.h,v 1.1.1.1 2004/01/05 17:27:47 tleek Exp $



*/


/*

<source>

*/

#include <arpa/nameser.h>

#define DNSMAP 1  

typedef struct
{
	char		*dns_q_domain;
	unsigned int	dns_q_type;
	unsigned int	dns_q_class;
} DNS_QUERY_T;

typedef struct
{
	unsigned int	mx_r_preference;
	char		mx_r_domain[1];
} MX_RECORD_T;

typedef struct
{
	unsigned int	srv_r_priority;
	unsigned int	srv_r_weight;
	unsigned int	srv_r_port;
	char		srv_r_target[1];
} SRV_RECORDT_T;


typedef struct resource_record RESOURCE_RECORD_T; /* from sm_resolve.h */

struct resource_record
{
	char			*rr_domain;
	unsigned int		rr_type;
	unsigned int		rr_class;
	unsigned int		rr_ttl;
	unsigned int		rr_size;
	union
	{
		void		*rr_data;
		MX_RECORD_T	*rr_mx;
		MX_RECORD_T	*rr_afsdb; /* mx and afsdb are identical */
		SRV_RECORDT_T	*rr_srv;
#  if NETINET
		struct in_addr	*rr_a;
#  endif /* NETINET */
#  if NETINET6
		struct in6_addr *rr_aaaa;
#  endif /* NETINET6 */
		char		*rr_txt;
	} rr_u;
	RESOURCE_RECORD_T        *rr_next;
};

typedef struct
{
	HEADER			dns_r_h;
	DNS_QUERY_T		dns_r_q;
	RESOURCE_RECORD_T	*dns_r_head;
} DNS_REPLY_T;





/*

</source>

*/

