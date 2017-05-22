/*
 * This is the portion of the openssl code which creates legitimate
 * heartbeat requests, and responds to them.   It is redacted to
 * plugin to the minimal framework of the simulator.
 *
 * Please see SSL_COPYRIGHT for the full copyright.
 */

#include "red.h"

/* ssl/s3_both.c */
/* Copyright (C) 1995-1998 Eric Young (eay@cryptsoft.com)
 * All rights reserved.
 *
 * This package is an SSL implementation written
 * by Eric Young (eay@cryptsoft.com).
 * The implementation was written so as to conform with Netscapes SSL.
 * 
 */
/* ====================================================================
 * Copyright (c) 1998-2002 The OpenSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * ====================================================================
 *
 * This product includes cryptographic software written by Eric Young
 * (eay@cryptsoft.com).  This product includes software written by Tim
 * Hudson (tjh@cryptsoft.com).
 *
 */
/* ====================================================================
 * Copyright 2002 Sun Microsystems, Inc. ALL RIGHTS RESERVED.
 * ECC cipher suite support in OpenSSL originally developed by 
 * SUN MICROSYSTEMS, INC., and contributed to the OpenSSL project.
 */

#ifndef OPENSSL_NO_BUF_FREELISTS
/* On some platforms, malloc() performance is bad enough that you can't just
 * free() and malloc() buffers all the time, so we need to use freelists from
 * unused buffers.  Currently, each freelist holds memory chunks of only a
 * given size (list->chunklen); other sized chunks are freed and malloced.
 * This doesn't help much if you're using many different SSL option settings
 * with a given context.  (The options affecting buffer size are
 * max_send_fragment, read buffer vs write buffer,
 * SSL_OP_MICROSOFT_BIG_WRITE_BUFFER, SSL_OP_NO_COMPRESSION, and
 * SSL_OP_DONT_INSERT_EMPTY_FRAGMENTS.)  Using a separate freelist for every
 * possible size is not an option, since max_send_fragment can take on many
 * different values.
 *
 * If you are on a platform with a slow malloc(), and you're using SSL
 * connections with many different settings for these options, and you need to
 * use the SSL_MOD_RELEASE_BUFFERS feature, you have a few options:
 *    - Link against a faster malloc implementation.
 *    - Use a separate SSL_CTX for each option set.
 *    - Improve this code.
 */
static void *
freelist_extract(SSL_CTX *ctx, int for_read, int sz)
	{
	SSL3_BUF_FREELIST *list;
	SSL3_BUF_FREELIST_ENTRY *ent = NULL;
	void *result = NULL;

	CRYPTO_w_lock(CRYPTO_LOCK_SSL_CTX);
	list = for_read ? ctx->rbuf_freelist : ctx->wbuf_freelist;
	if (list != NULL && sz == (int)list->chunklen)
		ent = list->head;
	if (ent != NULL)
		{
		list->head = ent->next;
		result = ent;
		if (--list->len == 0)
			list->chunklen = 0;
		}
	CRYPTO_w_unlock(CRYPTO_LOCK_SSL_CTX);
	if (!result)
		result = OPENSSL_malloc(sz);
	return result;
}

static void
freelist_insert(SSL_CTX *ctx, int for_read, size_t sz, void *mem)
	{
	SSL3_BUF_FREELIST *list;
	SSL3_BUF_FREELIST_ENTRY *ent;

	CRYPTO_w_lock(CRYPTO_LOCK_SSL_CTX);
	list = for_read ? ctx->rbuf_freelist : ctx->wbuf_freelist;
	if (list != NULL &&
	    (sz == list->chunklen || list->chunklen == 0) &&
	    list->len < ctx->freelist_max_len &&
	    sz >= sizeof(*ent))
		{
		list->chunklen = sz;
		ent = mem;
		ent->next = list->head;
		list->head = ent;
		++list->len;
		mem = NULL;
		}

	CRYPTO_w_unlock(CRYPTO_LOCK_SSL_CTX);
	if (mem)
		OPENSSL_free(mem);
	}
#else
#define freelist_extract(c,fr,sz) OPENSSL_malloc(sz)
#define freelist_insert(c,fr,sz,m) OPENSSL_free(m)
#endif

int ssl3_setup_read_buffer(SSL *s)
	{
	unsigned char *p;
	size_t len,align=0,headerlen;
	
	if (SSL_version(s) == DTLS1_VERSION || SSL_version(s) == DTLS1_BAD_VER)
		headerlen = DTLS1_RT_HEADER_LENGTH;
	else
		headerlen = SSL3_RT_HEADER_LENGTH;

#if defined(SSL3_ALIGN_PAYLOAD) && SSL3_ALIGN_PAYLOAD!=0
	align = (-SSL3_RT_HEADER_LENGTH)&(SSL3_ALIGN_PAYLOAD-1);
#endif

	if (s->s3->rbuf.buf == NULL)
		{
		len = SSL3_RT_MAX_PLAIN_LENGTH
			+ SSL3_RT_MAX_ENCRYPTED_OVERHEAD
			+ headerlen + align;
		if (s->options & SSL_OP_MICROSOFT_BIG_SSLV3_BUFFER)
			{
			s->s3->init_extra = 1;
			len += SSL3_RT_MAX_EXTRA;
			}
#ifndef OPENSSL_NO_COMP
		if (!(s->options & SSL_OP_NO_COMPRESSION))
			len += SSL3_RT_MAX_COMPRESSED_OVERHEAD;
#endif
		if ((p=freelist_extract(s->ctx, 1, len)) == NULL)
			goto err;
		s->s3->rbuf.buf = p;
		s->s3->rbuf.len = len;
		}

	s->packet= &(s->s3->rbuf.buf[0]);
	return 1;

err:
	SSLerr(SSL_F_SSL3_SETUP_READ_BUFFER,ERR_R_MALLOC_FAILURE);
	return 0;
	}

int ssl3_setup_write_buffer(SSL *s)
	{
	unsigned char *p;
	size_t len,align=0,headerlen;

	if (SSL_version(s) == DTLS1_VERSION || SSL_version(s) == DTLS1_BAD_VER)
		headerlen = DTLS1_RT_HEADER_LENGTH + 1;
	else
		headerlen = SSL3_RT_HEADER_LENGTH;

#if defined(SSL3_ALIGN_PAYLOAD) && SSL3_ALIGN_PAYLOAD!=0
	align = (-SSL3_RT_HEADER_LENGTH)&(SSL3_ALIGN_PAYLOAD-1);
#endif

	if (s->s3->wbuf.buf == NULL)
		{
		len = s->max_send_fragment
			+ SSL3_RT_SEND_MAX_ENCRYPTED_OVERHEAD
			+ headerlen + align;
#ifndef OPENSSL_NO_COMP
		if (!(s->options & SSL_OP_NO_COMPRESSION))
			len += SSL3_RT_MAX_COMPRESSED_OVERHEAD;
#endif
		if (!(s->options & SSL_OP_DONT_INSERT_EMPTY_FRAGMENTS))
			len += headerlen + align
				+ SSL3_RT_SEND_MAX_ENCRYPTED_OVERHEAD;

		if ((p=freelist_extract(s->ctx, 0, len)) == NULL)
			goto err;
		s->s3->wbuf.buf = p;
		s->s3->wbuf.len = len;
		}

	return 1;

err:
	SSLerr(SSL_F_SSL3_SETUP_WRITE_BUFFER,ERR_R_MALLOC_FAILURE);
	return 0;
	}


int ssl3_release_write_buffer(SSL *s)
	{
	if (s->s3->wbuf.buf != NULL)
		{
		freelist_insert(s->ctx, 0, s->s3->wbuf.len, s->s3->wbuf.buf);
		s->s3->wbuf.buf = NULL;
		}
	return 1;
	}

int ssl3_release_read_buffer(SSL *s)
	{
	if (s->s3->rbuf.buf != NULL)
		{
		freelist_insert(s->ctx, 1, s->s3->rbuf.len, s->s3->rbuf.buf);
		s->s3->rbuf.buf = NULL;
		}
	return 1;
	}
