
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
$Date: 2004/01/05 17:27:41 $
$Header: /mnt/leo2/cvs/sabo/hist-040105/bind/b1/ns_defs.h,v 1.1.1.1 2004/01/05 17:27:41 tleek Exp $



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
$Date: 2004/01/05 17:27:41 $
$Header: /mnt/leo2/cvs/sabo/hist-040105/bind/b1/ns_defs.h,v 1.1.1.1 2004/01/05 17:27:41 tleek Exp $



*/


/*

<source>

*/

/*
 * Global definitions for the name server.
 */

/*
 * Effort has been expended here to make all structure members 32 bits or
 * larger land on 32-bit boundaries; smaller structure members have been
 * deliberately shuffled and smaller integer sizes chosen where possible
 * to make sure this happens.  This is all meant to avoid structure member
 * padding which can cost a _lot_ of memory when you have hundreds of 
 * thousands of entries in your cache.
 */

/*
 * Timeout time should be around 1 minute or so.  Using the
 * the current simplistic backoff strategy, the sequence
 * retrys after 4, 8, and 16 seconds.  With 3 servers, this
 * dies out in a little more than a minute.
 * (sequence RETRYBASE, 2*RETRYBASE, 4*RETRYBASE... for MAXRETRY)
 */
#define	NEWZONES	64	/* must be a power of two. */
#define MINROOTS	2	/* min number of root hints */
#define NSMAX		16	/* max number of NS addrs to try ([0..255]) */
#define RETRYBASE	4 	/* base time between retries */
#define	MAXCLASS	255	/* XXX - may belong elsewhere */
#define MAXRETRY	3	/* max number of retries per addr */
#define MAXCNAMES	8	/* max # of CNAMES tried per addr */
#define MAXQUERIES	20	/* max # of queries to be made */
#define	MAXQSERIAL	4	/* max # of outstanding QSERIAL's */
				/* (prevent "recursive" loops) */
#define	INIT_REFRESH	600	/* retry time for initial secondary */
				/* contact (10 minutes) */
#define MIN_REFRESH	2	/* never refresh more frequently than once */
				/* every MIN_REFRESH seconds */
#define MIN_RETRY	1	/* never retry more frequently than once */
				/* every MIN_RETRY seconds */
#define NADDRECS	20	/* max addt'l rr's per resp */

#define XFER_TIMER	120	/* named-xfer's connect timeout */
#define MAX_XFER_TIME	60*60*2	/* default max seconds for an xfer */
#define XFER_TIME_FUDGE	10	/* MAX_XFER_TIME fudge */
#define MAX_XFERS_RUNNING 20	/* max value of transfers_in */
#define DEFAULT_XFERS_RUNNING 10  /* default value of transfers_in */
#define DEFAULT_XFERS_PER_NS 2	  /* default # of xfers per peer nameserver */
#define	XFER_BUFSIZE	(16*1024) /* arbitrary but bigger than most MTU's */

				  /* maximum time to cache negative answers */
#define DEFAULT_MAX_NCACHE_TTL (3*60*60)

#define ALPHA    0.7	/* How much to preserve of old response time */
#define	BETA	 1.2	/* How much to penalize response time on failure */
#define	GAMMA	 0.98	/* How much to decay unused response times */


	/* What global options are set? */
#define	OPTION_NORECURSE	0x0001	/* Don't recurse even if asked. */
#define	OPTION_NOFETCHGLUE	0x0002	/* Don't fetch missing glue. */
#define	OPTION_FORWARD_ONLY	0x0004	/* Don't use NS RR's, just forward. */
#define	OPTION_FAKE_IQUERY	0x0008	/* Fake up bogus response to IQUERY. */
#define	OPTION_NONOTIFY		0x0010	/* Turn off notify */
#define	OPTION_NONAUTH_NXDOMAIN	0x0020	/* Generate non-auth NXDOMAINs? */
#define	OPTION_MULTIPLE_CNAMES	0x0040	/* Allow a name to have multiple
					 * CNAME RRs */
#define OPTION_HOSTSTATS	0x0080	/* Maintain per-host statistics? */
#define OPTION_DEALLOC_ON_EXIT	0x0100	/* Deallocate everything on exit? */
#define OPTION_USE_IXFR		0x0110	/* Use by delault ixfr in zone transfer */
#define OPTION_MAINTAIN_IXFR_BASE 0x0120 
#define OPTION_NODIALUP		0x0200	/* Turn off dialup support */
#define OPTION_NORFC2308_TYPE1	0x0400	/* Prevent type1 respones (RFC 2308)
					 * to cached negative respones */
#define	OPTION_USE_ID_POOL	0x0800	/* Use the memory hogging query ID */

#define	DEFAULT_OPTION_FLAGS	(OPTION_NODIALUP|OPTION_NONAUTH_NXDOMAIN|\
				 OPTION_USE_ID_POOL|OPTION_NORFC2308_TYPE1)

#ifdef BIND_UPDATE
#define SOAINCRINTVL 300 /* default value for the time after which
			  * the zone serial number must be incremented
			  * after a successful update has occurred */
#define DUMPINTVL 3600   /* default interval at which to dump changed zones
			  * randomized, not exact */
#define DEFERUPDCNT	100	/* default number of updates that can happen
				 * before the zone serial number will be
				 * incremented */
#define UPDATE_TIMER XFER_TIMER
#endif /* BIND_UPDATE */

#define	USE_MINIMUM	0xffffffff
#define	MAXIMUM_TTL	0x7fffffff

#define CLEAN_TIMER		0x01
#define INTERFACE_TIMER		0x02
#define STATS_TIMER		0x04
#define HEARTBEAT_TIMER		0x08


enum severity { ignore, warn, fail, not_set };

/* Flags for ip_match_element */
#define IP_MATCH_NEGATE			0x01	/* match means deny access */

/* these fields are ordered to maintain word-alignment;
 * be careful about changing them.
 */
struct zoneinfo {
	char		*z_origin;	/* root domain name of zone */
	time_t		z_time;		/* time for next refresh */
	time_t		z_lastupdate;	/* time of last soa serial increment */
	u_int32_t	z_refresh;	/* refresh interval */
	u_int32_t	z_retry;	/* refresh retry interval */
	u_int32_t	z_expire;	/* expiration time for cached info */
	u_int32_t	z_minimum;	/* minimum TTL value */
	u_int32_t	z_serial;	/* changes if zone modified */
	char		*z_source;	/* source location of data */
	time_t		z_ftime;	/* modification time of source file */
	struct in_addr	z_axfr_src;	/* bind() the axfr socket to this */
	struct in_addr	z_xaddr;	/* override server for next xfer */
	struct in_addr	z_addr[NSMAX];	/* list of master servers for zone */
	u_char		z_addrcnt;	/* number of entries in z_addr[] */
	u_char		z_type;		/* type of zone; see below */
	u_int32_t	z_flags;	/* state bits; see below */
	pid_t		z_xferpid;	/* xfer child pid */
	u_int		z_options;	/* options set specific to this zone */
	u_int		z_optset;	/* which opts override global opts */
	int		z_class;	/* class of zone */
	int		z_numxfrs;	/* Ref count of concurrent xfrs. */
	enum severity	z_checknames;	/* How to handle non-RFC-compliant names */
#ifdef BIND_UPDATE
	time_t		z_dumptime;	/* randomized time for next zone dump
					 * if Z_NEED_DUMP is set */
	u_int32_t	z_dumpintvl;	/* time interval between zone dumps */
	time_t		z_soaincrintvl; /* interval for updating soa serial */
	time_t		z_soaincrtime;	/* time for soa increment */
	u_int32_t	z_deferupdcnt;  /* max number of updates before SOA
					 * serial number incremented */
	u_int32_t	z_updatecnt;    /* number of update requests processed
					 * since the last SOA serial update */
	char 		*z_updatelog;	/* log file for updates */
#endif	

	struct in_addr	*z_also_notify; /* More nameservers to notify */
	int		z_notify_count;
	char            *z_ixfr_base;	/* where to find the history of the zone */
	char            *z_ixfr_tmp;    /* tmp file for the ixfr */
	int		z_maintain_ixfr_base;
	int		z_log_size_ixfr;
	int		z_max_log_size_ixfr;
	
};

	/* zone types (z_type) */
enum zonetype { z_nil, z_master, z_slave, z_hint, z_stub, z_forward, z_any };
#define	Z_NIL		z_nil		/* XXX */
#define Z_MASTER	z_master	/* XXX */
#define Z_PRIMARY	z_master	/* XXX */
#define Z_SLAVE		z_slave		/* XXX */
#define Z_SECONDARY	z_slave		/* XXX */
#define Z_HINT		z_hint		/* XXX */
#define Z_CACHE		z_hint		/* XXX */
#define Z_STUB		z_stub		/* XXX */
#define Z_FORWARD	z_forward	/* XXX */
#define Z_ANY		z_any		/* XXX*2 */

	/* zone state bits (32 bits) */
#define	Z_AUTH		0x00000001	/* zone is authoritative */
#define	Z_NEED_XFER	0x00000002	/* waiting to do xfer */
#define	Z_XFER_RUNNING	0x00000004	/* asynch. xfer is running */
#define	Z_NEED_RELOAD	0x00000008	/* waiting to do reload */
#define	Z_SYSLOGGED	0x00000010	/* have logged timeout */
#define	Z_QSERIAL	0x00000020	/* sysquery()'ing for serial number */
#define	Z_FOUND		0x00000040	/* found in boot file when reloading */
#define	Z_INCLUDE	0x00000080	/* set if include used in file */
#define	Z_DB_BAD	0x00000100	/* errors when loading file */
#define	Z_TMP_FILE	0x00000200	/* backup file for xfer is temporary */
#ifdef BIND_UPDATE
#define	Z_DYNAMIC	0x00000400	/* allow dynamic updates */
#define	Z_NEED_DUMP	0x00000800	/* zone has changed, needs a dump */
#define	Z_NEED_SOAUPDATE 0x00001000	/* soa serial number needs increment */
#endif /* BIND_UPDATE */
#define	Z_XFER_ABORTED	0x00002000	/* zone transfer has been aborted */
#define	Z_XFER_GONE	0x00004000	/* zone transfer process is gone */
#define	Z_TIMER_SET	0x00008000	/* z_timer contains a valid id */
#define	Z_NEED_QSERIAL  0x00020000	/* we need to re-call qserial() */

	/* named_xfer exit codes */
#define	XFER_UPTODATE	0		/* zone is up-to-date */
#define	XFER_SUCCESS	1		/* performed transfer successfully */
#define	XFER_TIMEOUT	2		/* no server reachable/xfer timeout */
#define	XFER_FAIL	3		/* other failure, has been logged */
#define XFER_SUCCESSAXFR 4              /* named-xfr recived a xfr */
#define XFER_SUCCESSIXFR 5              /* named-xfr recived a ixfr */
#define XFER_SUCCESSAXFRIXFRFILE 6      /* named-xfr received AXFR for IXFR */
#define XFER_ISAXFR     -1              /* the last XFR is AXFR */
#define XFER_ISIXFR     -2              /* the last XFR is IXFR */
#define XFER_ISAXFRIXFR	-3		/* the last XFR is AXFR but we must create IXFR base */

/* q_flags bits (8 bits) */
#define	Q_SYSTEM	0x01		/* is a system query */
#define	Q_PRIMING	0x02		/* generated during priming phase */
#define	Q_ZSERIAL	0x04		/* getting zone serial for xfer test */
#define	Q_USEVC		0x08		/* forward using tcp not udp */


#define	RETRY_TIMEOUT	45

/*
 * Return codes from ns_forw:
 */
#define	FW_OK		0
#define	FW_DUP		1
#define	FW_NOSERVER	2
#define	FW_SERVFAIL	3






#define SXL_GLUING	0x01
#define SXL_ZONECUT	0x02

	/* flags */
#define STREAM_MALLOC		0x01
#define STREAM_WRITE_EV		0x02
#define STREAM_READ_EV		0x04
#define STREAM_CONNECT_EV	0x08
#define STREAM_DONE_CLOSE	0x10
#define STREAM_AXFR		0x20

#define ALLOW_NETS	0x0001
#define	ALLOW_HOSTS	0x0002
#define	ALLOW_ALL	(ALLOW_NETS | ALLOW_HOSTS)

		
enum transport { primary_trans, secondary_trans, response_trans, update_trans,
		 num_trans };

/* types used by the parser or config routines */

typedef struct zone_config {
	void *opaque;
} zone_config;

enum context { domain_ctx, owner_ctx, mailname_ctx, hostname_ctx };

typedef struct options {
	enum severity check_names[num_trans];
	
} *options;


#define NOERROR_NODATA   6	/* only used internally by the server, used for
				 * -ve $ing non-existence of records. 6 is not 
				 * a code used as yet anyway. anant@isi.edu
				 */

#define NTTL		600 /* ttl for negative data: 10 minutes? */

#define VQEXPIRY	900 /* a VQ entry expires in 15*60 = 900 seconds */

#ifdef BIND_UPDATE
enum req_action { Finish, Refuse, Return };
#endif

#define MAXDATA               (2*MAXDNAME + 5*INT32SZ)

#define       CLASS_BITS      8

#define       CLASS_MAX       ((1<<CLASS_BITS)-1)


/*

</source>

*/

