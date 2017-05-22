#ifndef _RED_SSL_H_
#define _RED_SSL_H_

/*
 * HeartBit -- the HeartBeat Simulator Framework
 *
 * Copyright (C) 2014 Bolo -- Josef T. Burger
 *
 * All original work here is distributed via a BSD license terms.
 * Basically you are free to use it and you can't take credit for it.
 *
 * Portions of this may reflect code structure and comments from
 * from the openssl codebase.  Please check the SSL copyright if you
 * create deriviative works that would conflict with any copyrights
 * on that code.
 *
 * The SSL copyright is not present because extracts of code are not
 * copyright violations, and fall under the fair use clause of the
 * copyright act.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

/* There is a bunch of SSL support structure simplified and re-implemented
   WITH IDENTICAL NAMES.  This is done so that some of the relevant SSL
   code can be dropped into place and used without modification in
   this demonstration infrastructure.   The reimplentation exists so that
   all the SSL pieces can drop into place and work without modification.  */

/* Due to the nature of the beast, this thing is quite simplified and 
   does not do typical error checking that a normal SSL implementation
   would do. */

/*
 * Marshall and unmarshal 16 bit data to/from network format and
 * advance the marshal pointer.
 */
/* These are the "real" ssl versions */
#define n2s(c,s)	((s=(((unsigned int)(c[0]))<< 8)| \
			    (((unsigned int)(c[1]))    )),c+=2)
#define s2n(s,c)	((c[0]=(unsigned char)(((s)>> 8)&0xff), \
			  c[1]=(unsigned char)(((s)    )&0xff)),c+=2)


/* Need these DTLS1 stuff for real buffer allocation code */
#define DTLS1_VERSION			0xFEFF
#define DTLS1_BAD_VER			0x0100
#define DTLS1_RT_HEADER_LENGTH		13




/*
 * The on the wire format of an ssl packet is:
 *	char	type;
 *	char	major;
 *	char	ssl_minor
 *	short	length
 *	== 5
 *	length bytes of data
 *
 * You need to read this 5 byte unencrypted header to know how much data
 * to read to get a full SSL packet.  SSL packets are limited to
 * around 16K (variable due to complications).
 */

#define	SSL_HDR_LEN	5

#define	SSL_VER_MAJOR	'U'
#define SSL_VER_MINOR	'W'

/* We simulate SSL data handling, but don't actually encrypt */

/* Not so random padding -- 128 bits == 16 bytes! */
#define	PADDING		"|RANDOM-PADDING|"
#define	PADDING_SZ	strlen(PADDING)

/* And a MAC that never changes -- 64 bits == 8 bytes */
#define	MAC		"|MACMAC|"
#define	MAC_SZ		strlen(MAC)

/* HB packet types inside HB packet */
#define	TLS1_HB_REQUEST		1
#define	TLS1_HB_RESPONSE	2

/* Normal data that applications want */
#define SSL3_RT_APPLICATION_DATA	23

/* Message Type RT of packet */
#define	TLS1_RT_HEARTBEAT	24

/* Xxx needed for real heartbeat generator so it compiles */
#define	SSL_TLSEXT_HB_ENABLED			0x1
#define	SSL_TLSEXT_HB_DONT_SEND_REQUESTS	0x2	/* notused here */

/* XXX code grab actual #defs */
#define	SSL_F_TLS1_HEARTBEAT	1
#define	SSL_R_TLS_HEARTBEAT_PEER_DOESNT_ACCEPT	1
#define	SSL_R_TLS_HEARTBEAT_PENDING		2
#define	SSL_R_UNEXPECTED_MESSAGE		3

#define SSL_OP_MICROSOFT_BIG_SSLV3_BUFFER	0x00000020L
#define SSL_OP_NO_COMPRESSION			0x00020000L
#define SSL_OP_DONT_INSERT_EMPTY_FRAGMENTS	0x00000800L /* added in 0.9.6e */


#define SSL3_RT_HEADER_LENGTH			5
#define SSL3_ALIGN_PAYLOAD			8
#define SSL3_RT_MAX_PLAIN_LENGTH		16384
#define SSL3_RT_MAX_MD_SIZE			64
#define SSL3_RT_MAX_ENCRYPTED_OVERHEAD		(256 + SSL3_RT_MAX_MD_SIZE)
#define SSL3_RT_MAX_EXTRA			(16384)
#define SSL3_RT_MAX_COMPRESSED_OVERHEAD		1024
#define SSL_RT_MAX_CIPHER_BLOCK_SIZE		16

#define SSL3_RT_SEND_MAX_ENCRYPTED_OVERHEAD \
			(SSL_RT_MAX_CIPHER_BLOCK_SIZE + SSL3_RT_MAX_MD_SIZE)


/* Disable these for now, need more emulation to use them. */
#define	OPENSSL_NO_BUF_FREELISTS



#define	SSL_in_init(s)	0
#define	SSL_version(s)	(s->version)


#define	OPENSSL_assert(x)	assert(x)

#define	SSLerr(x,y)

/* XXX error handling  .. just used by process_heartbeat & buffer code*/
#define	OPENSSL_malloc(n)	malloc(n)
#define	OPENSSL_free(p)		free(p)

/* This is always 16 for heartbeat */
#define	RAND_pseudo_bytes(p, len)	memcpy(p, PADDING, strlen(PADDING));


/* The SSL code uses "buffers" to hold data.   Since data can 
   have multiple things inside a buffer, there is a offset/left
   information to look at the area of interest in the buffer.
   offset + left < len, since not all of the buffer space may be used
   by an individual transaction using that buffer space. */

/* XXX not the exact ssl value, use exact code */
#define	MAX_SSL_BUF	16384

typedef struct {
	unsigned char	*buf;
	int	len;			/* length of pointed-to buffer */
	int	offset;			/* "pointer" into the buffer */
	int	left;			/* bytes left after the pointer */
} SSL3_BUFFER;

/* A "record" is basically a pointer to some data that is someplace else,
   typically part of a buffer.
   In addition to the exact SSL contents, I've added a few extra
   to make the "simulator" work out easier, since I am NOT implenting
   all the guts of SSL3, and need / want some data around, to simulate
   the memory behavior and dependencies of openssl. */

	/* SSL says ... */
     /* So at this point the following is true
         * ssl->s3->rrec.type   is the type of record
         * ssl->s3->rrec.length == number of bytes in record
         * ssl->s3->rrec.off    == offset to first valid byte
         * ssl->s3->rrec.data   == where to take bytes from, increment
         *                         after use :-).
         */

typedef struct {
	int		type;
	/* record info has similar semantics to the info in "buffer" above */
	unsigned	length;		/* how long it is */
	unsigned	off;		/* current pointer into a buf */
	unsigned char	*data;		/* pointed to data */
	unsigned char	*input;		/* other data ??? */
} SSL3_RECORD;


/* Sketchy version of ssl3 connection context */
typedef struct {
	SSL3_BUFFER	rbuf;
	SSL3_BUFFER	wbuf;

	SSL3_RECORD	rrec;
	SSL3_RECORD	wrec;

	int	init_extra;

	/* extra data used to park info from header somewhere */
	int	major;
	int	minor;
} SSL3_STATE;


typedef struct ssl_st SSL;
/* Sketchy version of SSL context */
struct ssl_st {
	SSL3_STATE	*s3;

	/* points to the bytes of a packet */
	unsigned char	*packet;
	unsigned	packet_length;

	/* XXX unused by simulator, needs a zero pointer and arg for 
	   heartbeat processing function to be callable. */
	/* callback that allows applications to peek at protocol messages */
	void (*msg_callback)(int write_p, int version, int content_type, const void *buf, size_t len, SSL *ssl, void *arg);
	void *msg_callback_arg;

	/* all this stuff is for the real heartbeat code to be callable */
	unsigned	tlsext_heartbeat;
	unsigned	tlsext_hb_seq;
	unsigned	tlsext_hb_pending;

        int	in_handshake;

	int	version;
	int	max_send_fragment;
	int	options;

	/* Simulate the BIO layer retry operation flag */
	int	bio_should_retry;


	/* And just toss stuff in here to reduce the malloc count */

#define	NUM_RAND_ALLOCS	10
	unsigned char	*random_memory[NUM_RAND_ALLOCS];

	SSL3_STATE	_s3;
};


/* Shim to output bytes from heartbeat (and send packet to HB) */
extern int ssl3_write_bytes(SSL *, int type, unsigned char *buf, int len);


/* The actual SSL code that does heartbeat */
extern int tls1_process_heartbeat(SSL *s);
extern int tls1_heartbeat(SSL *s);

/* The actual SSL code that does buffer creation */
extern int ssl3_setup_read_buffer(SSL *s);
extern int ssl3_setup_write_buffer(SSL *s);

#endif /* _RED_SSL_H_ */
