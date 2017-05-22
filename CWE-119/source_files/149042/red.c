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


#include "red.h"


#define	NONAME	"<noname>"
static char *_name = NONAME;

/* Xxx print addresses in buffers, not offsets */
static int print_addrs = 0;

/* Xxx print more details about unprintable chars */
static int print_detail = 0;

/*  print the contents of a "buffer" to see stolen data */
void print_buffer(FILE *fp, unsigned char *buf, int len);

/* TYPE + LEN part of heartbeat packet before payload */
#define	HB_HDR_LEN	3

FILE	*hbout;


void fill_buffer(unsigned char *s, int len, char *pat, int patlen)
{
	int	l = patlen ? patlen : strlen(pat);

	while (len) {
		if (l > len)
			l = len;
		memcpy(s, pat, l);
		s += l;
		len -= l;
	}
}


void init_rec_buf(SSL3_RECORD *r, SSL3_BUFFER *b)
{
	r->data = r->input = &b->buf[0];
	r->type = 0;
	r->length = b->len;
	r->off = 0;
}


void init_ssl3(SSL3_STATE *s3, SSL *s)
{
	ssl3_setup_read_buffer(s);
	fprintf(hbout,
		"%s: SSL exact read buffer len %d\n",
		_name, s3->rbuf.len);
	ssl3_setup_write_buffer(s);
	fprintf(hbout,
		"%s: SSL exact write buffer len %d\n",
		_name, s3->wbuf.len);

	init_rec_buf(&s3->rrec, &s3->rbuf);
	init_rec_buf(&s3->wrec, &s3->wbuf);

	/* XXX simulate tasty data from prior uses of the read buffer */
	fill_buffer(&s3->rbuf.buf[0], s3->rbuf.len,
		    "|READBUF-PRIVATE-CONFIDENTIAL-SECRET", 0);
	fill_buffer(&s3->wbuf.buf[0], s3->wbuf.len,
		    "|writebuf-private-confidential-secret", 0);

	s3->init_extra = 0;

	s3->major = 0;
	s3->minor = 0;
}



/* Takes a chunk of memory and initializes it */
void init_ssl(SSL *s)
{
	int	i;

	s->packet = 0;
	s->packet_length = 0;

	s->msg_callback = 0;
	s->msg_callback_arg = 0;

	s->tlsext_heartbeat = SSL_TLSEXT_HB_ENABLED;
	s->tlsext_hb_seq = 0;
	s->tlsext_hb_pending = 0;
	s->in_handshake = 0;

	s->version = (SSL_VER_MAJOR << 8) | SSL_VER_MINOR;
	s->max_send_fragment = SSL3_RT_MAX_PLAIN_LENGTH;
	s->options = 0;

	s->bio_should_retry = 0;

	s->s3 = &s->_s3;
	init_ssl3(s->s3, s);

	/* Add some extra juicy memory to try and have nice stuff to look
	   at instead of a bunch of binary junk */
	for (i = 0; i < NUM_RAND_ALLOCS; i++) {
		char		text[80];
		unsigned char	*p;
		int		pl;

		sprintf(text, "|OTHER-PRIVATE-CONFIDENTIAL-SECRET-MEMORY-%02d",
			i);
		pl = strlen(text) * 32 * (i+1);
		p = malloc(pl);
		fill_buffer(p, pl, text, 0);

		s->random_memory[i] = p;
	}
}


/* Guarantee memory munging of data to write back so it looks valid ;
   this is similar pattern to what openssl uses */

void encrypt_decrypt(unsigned char *ptr, int len)
{
	int	i;

	for (i = 0; i < len; i++)
		ptr[i] ^= 0xff;
}


/* Get the bytes for a SSL record from the I/O device */
/* Combines ssl_get_record and ssl_read..() */
/* XXX remember, grossly simplified */

void ssl3_get_record(SSL *s)
{
	SSL3_RECORD	*rr = &s->s3->rrec;
	SSL3_BUFFER	*rb = &s->s3->rbuf;
	unsigned char	*p = &rb->buf[0];
	int		n;
	int		version;

	n = fread(p, 1, SSL_HDR_LEN, stdin);
	if (n == 0) {
		exit(0);
	}
	if (n != SSL_HDR_LEN) {
		perror("read SSL_HDR_LEN");
		fprintf(stderr, "%s: ssl3_get_rec: read %d vs %d\n",
			_name,
			n, SSL_HDR_LEN);
		exit(1);
	}

	/* unmarshal the header */
	rr->type = *p++;
	s->s3->major = *p++;
	s->s3->minor = *p++;
	version = (s->s3->major << 8) | s->s3->minor;
	if (s->version != version)
		fprintf(stderr, "%s: version mismatch: ssl %x  packet %x\n",
			_name,
			s->version, version);
	n2s(p, rr->length);

	/* XXXX data length --- need to research it */
	if (rr->length > s->s3->rbuf.len - SSL_HDR_LEN)  {
		fprintf(stderr, "%s: packet length %d > max %d\n", 
			_name,
			rr->length, s->s3->rbuf.len - SSL_HDR_LEN);
		exit(1);
	}

	n = fread(p, 1, rr->length, stdin);
	if (n == 0) {
		fprintf(stderr, "%s: done\n", _name);
		exit(1);
	}
	if (n != rr->length) {
		perror("fread");
		fprintf(stderr, "%s: packet short read length %d != %d\n", 
			_name,
			n, rr->length);
		exit(1);
	}

	/* point record to buffer and setup packet */
	s->packet = &(s->s3->rbuf.buf[0]);
	s->packet_length = SSL_HDR_LEN + rr->length;
	rr->input = &s->packet[SSL_HDR_LEN];

	/* not certain about these semantics */
	rb->offset = 0;
	rb->left = s->packet_length;

	/* this is where it all is */
	rr->data = rr->input;

	/* VERIFY MAC  ... which isn't encrypted */
	p = rr->data + rr->length - MAC_SZ;
	if (memcmp(p, MAC, MAC_SZ) != 0) {
		fprintf(stderr, "%s: MAC broken\n", _name);
		exit(1);
	}
	/* Skip the mac, by adjusting the length to remove it */
	rr->length -= MAC_SZ;

	/* decrypt data in place */

	/* touch all the packet bytes just like SSL does */
	encrypt_decrypt(rr->data, rr->length);
}

/* XXX the problem here is that the buffer has to be output
   as a bunch of packets.  This makes things more difficult.
   Do the "simple version" first and then do the full version.
   XXX that drives up the cost more, because you then have to 
   reassemble the packets into a record. */


/* Encode traffic and write ssl packet with the content and type. */

int ssl3_write_bytes(SSL *s, int type, unsigned char *buf, int len)
{
	/* This prints "Stolen" data w/out having to send it out somehere,
	   reassemble it, and do all that extra work */

	fprintf(hbout, "HB PACKET  type %d  len %d:\n", type, len);
	print_buffer(hbout, buf, len);

	return len;
}

void print_buffer(FILE *fp, unsigned char *buf, int len)
{
	int	i;
	int	c;
	int	addr = print_addrs;


	for (i = 0; i < len; i++) {
		if ((i % 64) == 0) {
			if (addr)
				fprintf(fp, "\n  %09lx: ", (long)(buf + i));
			else
				fprintf(fp, "\n\t%04x: ", i);
		}

		c = buf[i];
		if (isprint(c))
			fprintf(fp, "%c", c);
		else if (print_detail) {
			if (c & 0x80) {
				fprintf(fp, "M-");
				c = c & 0x7f;
			}
			if (c < 32) {
				/* do C-@ ... C-Z */
				/* or could do ascii NUL .. US */
				fprintf(fp, "C-");
				c += '@';
			}
			fprintf(fp, "%c", c);
		}
		else
			fprintf(fp, ".");
		/* or if a big chunk of binary stuff ....[count] like strace */
	}
	if (i != len && (i % 64)) {
		if (addr)
			fprintf(fp, "\n  %09lx: ", (long)(buf + i));
		else
			fprintf(fp, "\n\t%04x: ", i);
	}
	else
		fprintf(fp, "\n");
}


void examine_hb_packet(SSL *s)
{
	SSL3_RECORD	*rr = &s->s3->rrec;
	unsigned char	*p = &rr->data[0];
	int		hblen;
	int		hbtype;
	int		seqno = -1;

	hbtype = *p++;
	n2s(p, hblen);
	if (hblen >= 2)	/* grab a seqno if it might be there */
		n2s(p, seqno);

	if (hblen + HB_HDR_LEN > rr->length) {
		fprintf(hbout, "%s: heartbleed detected pkt len %d hblen %d\n",
			_name,
			rr->length, hblen);
	}
	else {
		fprintf(hbout,
			"%s: heartbeat likely  pkt len %d  hblen %d  seqno %d\n",
			_name,
			rr->length, hblen, seqno);
	}
}


/* Make this thing look _just_like the interface that is used, to prove
   a point ... */

int ssl3_read_bytes(SSL *s, int type, unsigned char *buf, int len, int peek)
{
	/* It's simple ... nothing changes at all */
	SSL3_RECORD	*rr = &s->s3->rrec;

	ssl3_get_record(s);

	s->bio_should_retry = 0;

	/* If we are waiting for this kind of packet, return it
	   juast as SSL does to simulate passing results back
	   to user */
	if (rr->type == type) {
		int	l = len;
		if (l > rr->length)
			l = rr->length;
		memcpy(buf, rr->data, l);
		return l;
	}

	/* Otherwise we have to deal with other packets */

	switch (rr->type) {
	case TLS1_RT_HEARTBEAT:
		fprintf(hbout,
			"%s: received packet type TLS1_RT_HEARTBEAT  length %d\n",
			_name,
			rr->length);
		/* examine it for details */
		examine_hb_packet(s);
		(void) tls1_process_heartbeat(s);
		s->bio_should_retry++;
		return -1;
		break;

	default:
		fprintf(stderr,
			"%s: process_traffic: unknown: type %d length %d\n",
			_name,
			rr->type, rr->length);
		exit(1);
	}
	return 0;
}


/* This shows how a typical application isn't really aware of the heartbeat
   packets at all.  All it sees is "read again" from ssl.   See how the
   application just gets "read again" when a heartbeat comes through,
   same as a non-blocking I/O, etc  */

void process_ssl_traffic(SSL *s)
{
	/* this thing just goes on forever until something goes wrong,
	   and then it aborts from there. */

	unsigned char	buf[80];
	int		n;

	for (;;) {
		n = ssl3_read_bytes(s, SSL3_RT_APPLICATION_DATA,
				    buf, sizeof(buf), 0);
		/* Did we get something useful? */
		if (n > 0) {
			/* Print something useful */
			fprintf(hbout, "%s: data %d: '", _name, n);
			print_buffer(hbout, buf, n);
			fprintf(hbout, "'\n");
			continue;
		}

		/* Keep on reading if there was a retryable error */
		if (n == -1 && !s->bio_should_retry)
			break;

		/* Just simulate retryable errors in this case to
		   prove the point of how applications tend to work
		   between non-blocking I/O, short reads, etc */
	}

	fprintf(stderr, "%s: give-up: non retryable error\n", _name);
}



int main(int argc, char **argv)
{
	char	namebuf[128];
	SSL	*s = 0;

	hbout = stdout;

	sprintf(namebuf, "red-blood");
	_name = namebuf;

	s = malloc(sizeof(SSL));
	if (!s) {
		perror("malloc SSL");
		return(1);
	}
	init_ssl(s);


	fprintf(hbout, "%s: starting heartbeat reader\n", _name);
	process_ssl_traffic(s);

	/* NO ATTEMPT MADE TO CLEAN UP ANYTHING FOR SIMPLICITY */

	_name = NONAME;

	return 0;
}
