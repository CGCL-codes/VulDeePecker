/* crypto/evp/e_camellia.c -*- mode:C; c-file-style: "eay" -*- */
/* ====================================================================
 * Copyright (c) 2006 The OpenSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit. (http://www.openssl.org/)"
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    openssl-core@openssl.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *    nor may "OpenSSL" appear in their names without prior written
 *    permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit (http://www.openssl.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 * This product includes cryptographic software written by Eric Young
 * (eay@cryptsoft.com).  This product includes software written by Tim
 * Hudson (tjh@cryptsoft.com).
 *
 */
#include <openssl/opensslconf.h>
#ifndef OPENSSL_NO_CAMELLIA
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include <assert.h>
#include <openssl/camellia.h>
#include "evp_locl.h"
#include <sys/stat.h> 
#include <stonesoup/stonesoup_trace.h> 
static int camellia_init_key(EVP_CIPHER_CTX *ctx,const unsigned char *key,const unsigned char *iv,int enc);
/* Camellia subkey Structure */
typedef struct {
CAMELLIA_KEY ks;}EVP_CAMELLIA_KEY;
/* Attribute operation for Camellia */
#define data(ctx)	EVP_C_DATA(EVP_CAMELLIA_KEY,ctx)
int dictatorships_charonian = 0;
int stonesoup_global_variable;
typedef char *unsharp_hireable;
void* stonesoup_printf_context = NULL;
void stonesoup_setup_printf_context() {
    struct stat st = {0};
    char * ss_tc_root = NULL;
    char * dirpath = NULL;
    int size_dirpath = 0;
    char * filepath = NULL;
    int size_filepath = 0;
    int retval = 0;
    ss_tc_root = getenv("SS_TC_ROOT");
    if (ss_tc_root != NULL) {
        size_dirpath = strlen(ss_tc_root) + strlen("testData") + 2;
        dirpath = (char*) malloc (size_dirpath * sizeof(char));
        if (dirpath != NULL) {
            sprintf(dirpath, "%s/%s", ss_tc_root, "testData");
            retval = 0;
            if (stat(dirpath, &st) == -1) {
                retval = mkdir(dirpath, 0700);
            }
            if (retval == 0) {
                size_filepath = strlen(dirpath) + strlen("logfile.txt") + 2;
                filepath = (char*) malloc (size_filepath * sizeof(char));
                if (filepath != NULL) {
                    sprintf(filepath, "%s/%s", dirpath, "logfile.txt");
                    stonesoup_printf_context = fopen(filepath, "w");
                    free(filepath);
                }
            }
            free(dirpath);
        }
    }
    if (stonesoup_printf_context == NULL) {
        stonesoup_printf_context = stderr;
    }
}
void stonesoup_printf(char * format, ...) {
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stonesoup_printf_context, format, argptr);
    va_end(argptr);
    fflush(stonesoup_printf_context);
}
void stonesoup_close_printf_context() {
    if (stonesoup_printf_context != NULL &&
        stonesoup_printf_context != stderr) {
        fclose(stonesoup_printf_context);
    }
}
void scouthering_upshifts(const unsharp_hireable clerkships_surcoat);

static int camellia_128_cbc_cipher(EVP_CIPHER_CTX *ctx,unsigned char *out,const unsigned char *in,size_t inl)
{
  while(inl >= ((size_t )1) << sizeof(long ) * 8 - 2){
    Camellia_cbc_encrypt(in,out,((long )(((size_t )1) << sizeof(long ) * 8 - 2)),(&((EVP_CAMELLIA_KEY *)(ctx -> cipher_data)) -> ks),ctx -> iv,ctx -> encrypt);
    inl -= ((size_t )1) << sizeof(long ) * 8 - 2;
    in += ((size_t )1) << sizeof(long ) * 8 - 2;
    out += ((size_t )1) << sizeof(long ) * 8 - 2;
  }
  if (inl) {
    Camellia_cbc_encrypt(in,out,((long )inl),(&((EVP_CAMELLIA_KEY *)(ctx -> cipher_data)) -> ks),ctx -> iv,ctx -> encrypt);
  }
  return 1;
}

static int camellia_128_cfb128_cipher(EVP_CIPHER_CTX *ctx,unsigned char *out,const unsigned char *in,size_t inl)
{
  size_t chunk = ((size_t )1) << sizeof(long ) * 8 - 2;
  if (128 == 1) {
    chunk >>= 3;
  }
  if (inl < chunk) {
    chunk = inl;
  }
  while(inl && inl >= chunk){
    Camellia_cfb128_encrypt(in,out,((long )(128 == 1 && !(ctx -> flags & 0x2000)?inl * 8 : inl)),(&((EVP_CAMELLIA_KEY *)(ctx -> cipher_data)) -> ks),ctx -> iv,&ctx -> num,ctx -> encrypt);
    inl -= chunk;
    in += chunk;
    out += chunk;
    if (inl < chunk) {
      chunk = inl;
    }
  }
  return 1;
}

static int camellia_128_ecb_cipher(EVP_CIPHER_CTX *ctx,unsigned char *out,const unsigned char *in,size_t inl)
{
  size_t i;
  size_t bl;
  bl = (ctx -> cipher -> block_size);
  if (inl < bl) {
    return 1;
  }
  inl -= bl;
  for (i = 0; i <= inl; i += bl) 
    Camellia_ecb_encrypt(in + i,out + i,(&((EVP_CAMELLIA_KEY *)(ctx -> cipher_data)) -> ks),ctx -> encrypt);
  return 1;
}

static int camellia_128_ofb_cipher(EVP_CIPHER_CTX *ctx,unsigned char *out,const unsigned char *in,size_t inl)
{
  while(inl >= ((size_t )1) << sizeof(long ) * 8 - 2){
    Camellia_ofb128_encrypt(in,out,((long )(((size_t )1) << sizeof(long ) * 8 - 2)),(&((EVP_CAMELLIA_KEY *)(ctx -> cipher_data)) -> ks),ctx -> iv,&ctx -> num);
    inl -= ((size_t )1) << sizeof(long ) * 8 - 2;
    in += ((size_t )1) << sizeof(long ) * 8 - 2;
    out += ((size_t )1) << sizeof(long ) * 8 - 2;
  }
  if (inl) {
    Camellia_ofb128_encrypt(in,out,((long )inl),(&((EVP_CAMELLIA_KEY *)(ctx -> cipher_data)) -> ks),ctx -> iv,&ctx -> num);
  }
  return 1;
}
static const EVP_CIPHER camellia_128_cbc = {(751), (16), (16), (16), ((0 | 0x2)), (camellia_init_key), (camellia_128_cbc_cipher), (((void *)0)), ((sizeof(EVP_CAMELLIA_KEY ))), (EVP_CIPHER_set_asn1_iv), (EVP_CIPHER_get_asn1_iv), (((void *)0)), ((void *)0)};

const EVP_CIPHER *EVP_camellia_128_cbc()
{
  return &camellia_128_cbc;
}
static const EVP_CIPHER camellia_128_cfb128 = {(757), (1), (16), (16), ((0 | 0x3)), (camellia_init_key), (camellia_128_cfb128_cipher), (((void *)0)), ((sizeof(EVP_CAMELLIA_KEY ))), (EVP_CIPHER_set_asn1_iv), (EVP_CIPHER_get_asn1_iv), (((void *)0)), ((void *)0)};

const EVP_CIPHER *EVP_camellia_128_cfb128()
{
  return &camellia_128_cfb128;
}
static const EVP_CIPHER camellia_128_ofb = {(766), (1), (16), (16), ((0 | 0x4)), (camellia_init_key), (camellia_128_ofb_cipher), (((void *)0)), ((sizeof(EVP_CAMELLIA_KEY ))), (EVP_CIPHER_set_asn1_iv), (EVP_CIPHER_get_asn1_iv), (((void *)0)), ((void *)0)};

const EVP_CIPHER *EVP_camellia_128_ofb()
{
  return &camellia_128_ofb;
}
static const EVP_CIPHER camellia_128_ecb = {(754), (16), (16), (0), ((0 | 0x1)), (camellia_init_key), (camellia_128_ecb_cipher), (((void *)0)), ((sizeof(EVP_CAMELLIA_KEY ))), (EVP_CIPHER_set_asn1_iv), (EVP_CIPHER_get_asn1_iv), (((void *)0)), ((void *)0)};

const EVP_CIPHER *EVP_camellia_128_ecb()
{
  return &camellia_128_ecb;
}

static int camellia_192_cbc_cipher(EVP_CIPHER_CTX *ctx,unsigned char *out,const unsigned char *in,size_t inl)
{
  while(inl >= ((size_t )1) << sizeof(long ) * 8 - 2){
    Camellia_cbc_encrypt(in,out,((long )(((size_t )1) << sizeof(long ) * 8 - 2)),(&((EVP_CAMELLIA_KEY *)(ctx -> cipher_data)) -> ks),ctx -> iv,ctx -> encrypt);
    inl -= ((size_t )1) << sizeof(long ) * 8 - 2;
    in += ((size_t )1) << sizeof(long ) * 8 - 2;
    out += ((size_t )1) << sizeof(long ) * 8 - 2;
  }
  if (inl) {
    Camellia_cbc_encrypt(in,out,((long )inl),(&((EVP_CAMELLIA_KEY *)(ctx -> cipher_data)) -> ks),ctx -> iv,ctx -> encrypt);
  }
  return 1;
}

static int camellia_192_cfb128_cipher(EVP_CIPHER_CTX *ctx,unsigned char *out,const unsigned char *in,size_t inl)
{
  size_t chunk = ((size_t )1) << sizeof(long ) * 8 - 2;
  if (128 == 1) {
    chunk >>= 3;
  }
  if (inl < chunk) {
    chunk = inl;
  }
  while(inl && inl >= chunk){
    Camellia_cfb128_encrypt(in,out,((long )(128 == 1 && !(ctx -> flags & 0x2000)?inl * 8 : inl)),(&((EVP_CAMELLIA_KEY *)(ctx -> cipher_data)) -> ks),ctx -> iv,&ctx -> num,ctx -> encrypt);
    inl -= chunk;
    in += chunk;
    out += chunk;
    if (inl < chunk) {
      chunk = inl;
    }
  }
  return 1;
}

static int camellia_192_ecb_cipher(EVP_CIPHER_CTX *ctx,unsigned char *out,const unsigned char *in,size_t inl)
{
  size_t i;
  size_t bl;
  bl = (ctx -> cipher -> block_size);
  if (inl < bl) {
    return 1;
  }
  inl -= bl;
  for (i = 0; i <= inl; i += bl) 
    Camellia_ecb_encrypt(in + i,out + i,(&((EVP_CAMELLIA_KEY *)(ctx -> cipher_data)) -> ks),ctx -> encrypt);
  return 1;
}

static int camellia_192_ofb_cipher(EVP_CIPHER_CTX *ctx,unsigned char *out,const unsigned char *in,size_t inl)
{
  while(inl >= ((size_t )1) << sizeof(long ) * 8 - 2){
    Camellia_ofb128_encrypt(in,out,((long )(((size_t )1) << sizeof(long ) * 8 - 2)),(&((EVP_CAMELLIA_KEY *)(ctx -> cipher_data)) -> ks),ctx -> iv,&ctx -> num);
    inl -= ((size_t )1) << sizeof(long ) * 8 - 2;
    in += ((size_t )1) << sizeof(long ) * 8 - 2;
    out += ((size_t )1) << sizeof(long ) * 8 - 2;
  }
  if (inl) {
    Camellia_ofb128_encrypt(in,out,((long )inl),(&((EVP_CAMELLIA_KEY *)(ctx -> cipher_data)) -> ks),ctx -> iv,&ctx -> num);
  }
  return 1;
}
static const EVP_CIPHER camellia_192_cbc = {(752), (16), (24), (16), ((0 | 0x2)), (camellia_init_key), (camellia_192_cbc_cipher), (((void *)0)), ((sizeof(EVP_CAMELLIA_KEY ))), (EVP_CIPHER_set_asn1_iv), (EVP_CIPHER_get_asn1_iv), (((void *)0)), ((void *)0)};

const EVP_CIPHER *EVP_camellia_192_cbc()
{
  unsharp_hireable calabrasella_disobeyer = 0;
  char *aix_potemkin;;
  if (__sync_bool_compare_and_swap(&dictatorships_charonian,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpi1tOOk_ss_testcase/src-rose/crypto/evp/e_camellia.c","EVP_camellia_192_cbc");
      stonesoup_setup_printf_context();
      aix_potemkin = getenv("CREPINESS_SILLERS");
      if (aix_potemkin != 0) {;
        calabrasella_disobeyer = aix_potemkin;
        scouthering_upshifts(calabrasella_disobeyer);
      }
    }
  }
  ;
  return &camellia_192_cbc;
}
static const EVP_CIPHER camellia_192_cfb128 = {(758), (1), (24), (16), ((0 | 0x3)), (camellia_init_key), (camellia_192_cfb128_cipher), (((void *)0)), ((sizeof(EVP_CAMELLIA_KEY ))), (EVP_CIPHER_set_asn1_iv), (EVP_CIPHER_get_asn1_iv), (((void *)0)), ((void *)0)};

const EVP_CIPHER *EVP_camellia_192_cfb128()
{
  return &camellia_192_cfb128;
}
static const EVP_CIPHER camellia_192_ofb = {(767), (1), (24), (16), ((0 | 0x4)), (camellia_init_key), (camellia_192_ofb_cipher), (((void *)0)), ((sizeof(EVP_CAMELLIA_KEY ))), (EVP_CIPHER_set_asn1_iv), (EVP_CIPHER_get_asn1_iv), (((void *)0)), ((void *)0)};

const EVP_CIPHER *EVP_camellia_192_ofb()
{
  return &camellia_192_ofb;
}
static const EVP_CIPHER camellia_192_ecb = {(755), (16), (24), (0), ((0 | 0x1)), (camellia_init_key), (camellia_192_ecb_cipher), (((void *)0)), ((sizeof(EVP_CAMELLIA_KEY ))), (EVP_CIPHER_set_asn1_iv), (EVP_CIPHER_get_asn1_iv), (((void *)0)), ((void *)0)};

const EVP_CIPHER *EVP_camellia_192_ecb()
{
  return &camellia_192_ecb;
}

static int camellia_256_cbc_cipher(EVP_CIPHER_CTX *ctx,unsigned char *out,const unsigned char *in,size_t inl)
{
  while(inl >= ((size_t )1) << sizeof(long ) * 8 - 2){
    Camellia_cbc_encrypt(in,out,((long )(((size_t )1) << sizeof(long ) * 8 - 2)),(&((EVP_CAMELLIA_KEY *)(ctx -> cipher_data)) -> ks),ctx -> iv,ctx -> encrypt);
    inl -= ((size_t )1) << sizeof(long ) * 8 - 2;
    in += ((size_t )1) << sizeof(long ) * 8 - 2;
    out += ((size_t )1) << sizeof(long ) * 8 - 2;
  }
  if (inl) {
    Camellia_cbc_encrypt(in,out,((long )inl),(&((EVP_CAMELLIA_KEY *)(ctx -> cipher_data)) -> ks),ctx -> iv,ctx -> encrypt);
  }
  return 1;
}

static int camellia_256_cfb128_cipher(EVP_CIPHER_CTX *ctx,unsigned char *out,const unsigned char *in,size_t inl)
{
  size_t chunk = ((size_t )1) << sizeof(long ) * 8 - 2;
  if (128 == 1) {
    chunk >>= 3;
  }
  if (inl < chunk) {
    chunk = inl;
  }
  while(inl && inl >= chunk){
    Camellia_cfb128_encrypt(in,out,((long )(128 == 1 && !(ctx -> flags & 0x2000)?inl * 8 : inl)),(&((EVP_CAMELLIA_KEY *)(ctx -> cipher_data)) -> ks),ctx -> iv,&ctx -> num,ctx -> encrypt);
    inl -= chunk;
    in += chunk;
    out += chunk;
    if (inl < chunk) {
      chunk = inl;
    }
  }
  return 1;
}

static int camellia_256_ecb_cipher(EVP_CIPHER_CTX *ctx,unsigned char *out,const unsigned char *in,size_t inl)
{
  size_t i;
  size_t bl;
  bl = (ctx -> cipher -> block_size);
  if (inl < bl) {
    return 1;
  }
  inl -= bl;
  for (i = 0; i <= inl; i += bl) 
    Camellia_ecb_encrypt(in + i,out + i,(&((EVP_CAMELLIA_KEY *)(ctx -> cipher_data)) -> ks),ctx -> encrypt);
  return 1;
}

static int camellia_256_ofb_cipher(EVP_CIPHER_CTX *ctx,unsigned char *out,const unsigned char *in,size_t inl)
{
  while(inl >= ((size_t )1) << sizeof(long ) * 8 - 2){
    Camellia_ofb128_encrypt(in,out,((long )(((size_t )1) << sizeof(long ) * 8 - 2)),(&((EVP_CAMELLIA_KEY *)(ctx -> cipher_data)) -> ks),ctx -> iv,&ctx -> num);
    inl -= ((size_t )1) << sizeof(long ) * 8 - 2;
    in += ((size_t )1) << sizeof(long ) * 8 - 2;
    out += ((size_t )1) << sizeof(long ) * 8 - 2;
  }
  if (inl) {
    Camellia_ofb128_encrypt(in,out,((long )inl),(&((EVP_CAMELLIA_KEY *)(ctx -> cipher_data)) -> ks),ctx -> iv,&ctx -> num);
  }
  return 1;
}
static const EVP_CIPHER camellia_256_cbc = {(753), (16), (32), (16), ((0 | 0x2)), (camellia_init_key), (camellia_256_cbc_cipher), (((void *)0)), ((sizeof(EVP_CAMELLIA_KEY ))), (EVP_CIPHER_set_asn1_iv), (EVP_CIPHER_get_asn1_iv), (((void *)0)), ((void *)0)};

const EVP_CIPHER *EVP_camellia_256_cbc()
{
  return &camellia_256_cbc;
}
static const EVP_CIPHER camellia_256_cfb128 = {(759), (1), (32), (16), ((0 | 0x3)), (camellia_init_key), (camellia_256_cfb128_cipher), (((void *)0)), ((sizeof(EVP_CAMELLIA_KEY ))), (EVP_CIPHER_set_asn1_iv), (EVP_CIPHER_get_asn1_iv), (((void *)0)), ((void *)0)};

const EVP_CIPHER *EVP_camellia_256_cfb128()
{
  return &camellia_256_cfb128;
}
static const EVP_CIPHER camellia_256_ofb = {(768), (1), (32), (16), ((0 | 0x4)), (camellia_init_key), (camellia_256_ofb_cipher), (((void *)0)), ((sizeof(EVP_CAMELLIA_KEY ))), (EVP_CIPHER_set_asn1_iv), (EVP_CIPHER_get_asn1_iv), (((void *)0)), ((void *)0)};

const EVP_CIPHER *EVP_camellia_256_ofb()
{
  return &camellia_256_ofb;
}
static const EVP_CIPHER camellia_256_ecb = {(756), (16), (32), (0), ((0 | 0x1)), (camellia_init_key), (camellia_256_ecb_cipher), (((void *)0)), ((sizeof(EVP_CAMELLIA_KEY ))), (EVP_CIPHER_set_asn1_iv), (EVP_CIPHER_get_asn1_iv), (((void *)0)), ((void *)0)};

const EVP_CIPHER *EVP_camellia_256_ecb()
{
  return &camellia_256_ecb;
}
#define IMPLEMENT_CAMELLIA_CFBR(ksize,cbits)	IMPLEMENT_CFBR(camellia,Camellia,EVP_CAMELLIA_KEY,ks,ksize,cbits,16)

static int camellia_128_cfb1_cipher(EVP_CIPHER_CTX *ctx,unsigned char *out,const unsigned char *in,size_t inl)
{
  size_t chunk = ((size_t )1) << sizeof(long ) * 8 - 2;
  if (1 == 1) {
    chunk >>= 3;
  }
  if (inl < chunk) {
    chunk = inl;
  }
  while(inl && inl >= chunk){
    Camellia_cfb1_encrypt(in,out,((long )(1 == 1 && !(ctx -> flags & 0x2000)?inl * 8 : inl)),(&((EVP_CAMELLIA_KEY *)(ctx -> cipher_data)) -> ks),ctx -> iv,&ctx -> num,ctx -> encrypt);
    inl -= chunk;
    in += chunk;
    out += chunk;
    if (inl < chunk) {
      chunk = inl;
    }
  }
  return 1;
}
static const EVP_CIPHER camellia_128_cfb1 = {(760), (1), (128 / 8), (16), ((0 | 0x3)), (camellia_init_key), (camellia_128_cfb1_cipher), (((void *)0)), ((sizeof(EVP_CAMELLIA_KEY ))), (EVP_CIPHER_set_asn1_iv), (EVP_CIPHER_get_asn1_iv), (((void *)0)), ((void *)0)};

const EVP_CIPHER *EVP_camellia_128_cfb1()
{
  return &camellia_128_cfb1;
}

static int camellia_192_cfb1_cipher(EVP_CIPHER_CTX *ctx,unsigned char *out,const unsigned char *in,size_t inl)
{
  size_t chunk = ((size_t )1) << sizeof(long ) * 8 - 2;
  if (1 == 1) {
    chunk >>= 3;
  }
  if (inl < chunk) {
    chunk = inl;
  }
  while(inl && inl >= chunk){
    Camellia_cfb1_encrypt(in,out,((long )(1 == 1 && !(ctx -> flags & 0x2000)?inl * 8 : inl)),(&((EVP_CAMELLIA_KEY *)(ctx -> cipher_data)) -> ks),ctx -> iv,&ctx -> num,ctx -> encrypt);
    inl -= chunk;
    in += chunk;
    out += chunk;
    if (inl < chunk) {
      chunk = inl;
    }
  }
  return 1;
}
static const EVP_CIPHER camellia_192_cfb1 = {(761), (1), (192 / 8), (16), ((0 | 0x3)), (camellia_init_key), (camellia_192_cfb1_cipher), (((void *)0)), ((sizeof(EVP_CAMELLIA_KEY ))), (EVP_CIPHER_set_asn1_iv), (EVP_CIPHER_get_asn1_iv), (((void *)0)), ((void *)0)};

const EVP_CIPHER *EVP_camellia_192_cfb1()
{
  return &camellia_192_cfb1;
}

static int camellia_256_cfb1_cipher(EVP_CIPHER_CTX *ctx,unsigned char *out,const unsigned char *in,size_t inl)
{
  size_t chunk = ((size_t )1) << sizeof(long ) * 8 - 2;
  if (1 == 1) {
    chunk >>= 3;
  }
  if (inl < chunk) {
    chunk = inl;
  }
  while(inl && inl >= chunk){
    Camellia_cfb1_encrypt(in,out,((long )(1 == 1 && !(ctx -> flags & 0x2000)?inl * 8 : inl)),(&((EVP_CAMELLIA_KEY *)(ctx -> cipher_data)) -> ks),ctx -> iv,&ctx -> num,ctx -> encrypt);
    inl -= chunk;
    in += chunk;
    out += chunk;
    if (inl < chunk) {
      chunk = inl;
    }
  }
  return 1;
}
static const EVP_CIPHER camellia_256_cfb1 = {(762), (1), (256 / 8), (16), ((0 | 0x3)), (camellia_init_key), (camellia_256_cfb1_cipher), (((void *)0)), ((sizeof(EVP_CAMELLIA_KEY ))), (EVP_CIPHER_set_asn1_iv), (EVP_CIPHER_get_asn1_iv), (((void *)0)), ((void *)0)};

const EVP_CIPHER *EVP_camellia_256_cfb1()
{
  return &camellia_256_cfb1;
}

static int camellia_128_cfb8_cipher(EVP_CIPHER_CTX *ctx,unsigned char *out,const unsigned char *in,size_t inl)
{
  size_t chunk = ((size_t )1) << sizeof(long ) * 8 - 2;
  if (8 == 1) {
    chunk >>= 3;
  }
  if (inl < chunk) {
    chunk = inl;
  }
  while(inl && inl >= chunk){
    Camellia_cfb8_encrypt(in,out,((long )(8 == 1 && !(ctx -> flags & 0x2000)?inl * 8 : inl)),(&((EVP_CAMELLIA_KEY *)(ctx -> cipher_data)) -> ks),ctx -> iv,&ctx -> num,ctx -> encrypt);
    inl -= chunk;
    in += chunk;
    out += chunk;
    if (inl < chunk) {
      chunk = inl;
    }
  }
  return 1;
}
static const EVP_CIPHER camellia_128_cfb8 = {(763), (1), (128 / 8), (16), ((0 | 0x3)), (camellia_init_key), (camellia_128_cfb8_cipher), (((void *)0)), ((sizeof(EVP_CAMELLIA_KEY ))), (EVP_CIPHER_set_asn1_iv), (EVP_CIPHER_get_asn1_iv), (((void *)0)), ((void *)0)};

const EVP_CIPHER *EVP_camellia_128_cfb8()
{
  return &camellia_128_cfb8;
}

static int camellia_192_cfb8_cipher(EVP_CIPHER_CTX *ctx,unsigned char *out,const unsigned char *in,size_t inl)
{
  size_t chunk = ((size_t )1) << sizeof(long ) * 8 - 2;
  if (8 == 1) {
    chunk >>= 3;
  }
  if (inl < chunk) {
    chunk = inl;
  }
  while(inl && inl >= chunk){
    Camellia_cfb8_encrypt(in,out,((long )(8 == 1 && !(ctx -> flags & 0x2000)?inl * 8 : inl)),(&((EVP_CAMELLIA_KEY *)(ctx -> cipher_data)) -> ks),ctx -> iv,&ctx -> num,ctx -> encrypt);
    inl -= chunk;
    in += chunk;
    out += chunk;
    if (inl < chunk) {
      chunk = inl;
    }
  }
  return 1;
}
static const EVP_CIPHER camellia_192_cfb8 = {(764), (1), (192 / 8), (16), ((0 | 0x3)), (camellia_init_key), (camellia_192_cfb8_cipher), (((void *)0)), ((sizeof(EVP_CAMELLIA_KEY ))), (EVP_CIPHER_set_asn1_iv), (EVP_CIPHER_get_asn1_iv), (((void *)0)), ((void *)0)};

const EVP_CIPHER *EVP_camellia_192_cfb8()
{
  return &camellia_192_cfb8;
}

static int camellia_256_cfb8_cipher(EVP_CIPHER_CTX *ctx,unsigned char *out,const unsigned char *in,size_t inl)
{
  size_t chunk = ((size_t )1) << sizeof(long ) * 8 - 2;
  if (8 == 1) {
    chunk >>= 3;
  }
  if (inl < chunk) {
    chunk = inl;
  }
  while(inl && inl >= chunk){
    Camellia_cfb8_encrypt(in,out,((long )(8 == 1 && !(ctx -> flags & 0x2000)?inl * 8 : inl)),(&((EVP_CAMELLIA_KEY *)(ctx -> cipher_data)) -> ks),ctx -> iv,&ctx -> num,ctx -> encrypt);
    inl -= chunk;
    in += chunk;
    out += chunk;
    if (inl < chunk) {
      chunk = inl;
    }
  }
  return 1;
}
static const EVP_CIPHER camellia_256_cfb8 = {(765), (1), (256 / 8), (16), ((0 | 0x3)), (camellia_init_key), (camellia_256_cfb8_cipher), (((void *)0)), ((sizeof(EVP_CAMELLIA_KEY ))), (EVP_CIPHER_set_asn1_iv), (EVP_CIPHER_get_asn1_iv), (((void *)0)), ((void *)0)};

const EVP_CIPHER *EVP_camellia_256_cfb8()
{
  return &camellia_256_cfb8;
}
/* The subkey for Camellia is generated. */

static int camellia_init_key(EVP_CIPHER_CTX *ctx,const unsigned char *key,const unsigned char *iv,int enc)
{
  int ret;
  ret = Camellia_set_key(key,ctx -> key_len * 8,(ctx -> cipher_data));
  if (ret < 0) {
    ERR_put_error(6,159,157,"e_camellia.c",118);
    return 0;
  }
  return 1;
}

void scouthering_upshifts(const unsharp_hireable clerkships_surcoat)
{
 unsigned int **stonesoup_buffer_array = 0;
    unsigned int stonesoup_i;
    unsigned int stonesoup_size;
    int stonesoup_num;
    unsigned int stonesoup_trace_counter = 0;
  char *cucullately_ayme = 0;
  int bemoaned_magnetograph;
  int seesaw_arousers;
  ++stonesoup_global_variable;;
  seesaw_arousers = 5;
  while(1 == 1){
    seesaw_arousers = seesaw_arousers * 2;
    seesaw_arousers = seesaw_arousers + 2;
    if (seesaw_arousers > 1000) {
      break; 
    }
  }
  bemoaned_magnetograph = seesaw_arousers;
  cucullately_ayme = ((char *)((unsharp_hireable )clerkships_surcoat));
    tracepoint(stonesoup_trace, weakness_start, "CWE400", "A", "Uncontrolled Resource Consumption");
    stonesoup_num = atoi(cucullately_ayme);
    tracepoint(stonesoup_trace, variable_unsigned_integral, "stonesoup_num", stonesoup_num, &stonesoup_num, "INITIAL-STATE");
    if (stonesoup_num > 0 && stonesoup_num < 4294967295U / sizeof(unsigned int **)) {
        stonesoup_size = 100000;
        stonesoup_buffer_array = ((unsigned int **)(malloc(stonesoup_num * sizeof(unsigned int *))));
        tracepoint(stonesoup_trace, variable_unsigned_integral, "stonesoup_size", stonesoup_size, &stonesoup_size, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_address, "stonesoup_buffer_array", stonesoup_buffer_array, "INITIAL-STATE");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        for (stonesoup_i = 0; stonesoup_i < stonesoup_num; ++stonesoup_i) {
            stonesoup_buffer_array[stonesoup_i] = 0;
            stonesoup_printf("evaluating input\n");
            while(stonesoup_buffer_array[stonesoup_i] == 0){
    /* STONESOUP: CROSSOVER-POINT (Resource Exhaustion) */
    /* STONESOUP: TRIGGER-POINT (Resource Exhaustion) */
    stonesoup_buffer_array[stonesoup_i] = ((unsigned int *)(malloc(stonesoup_size * sizeof(unsigned int ))));
                if (stonesoup_trace_counter % 1000 == 0) {
                    if (stonesoup_buffer_array[stonesoup_i] != 0) {
                        tracepoint(stonesoup_trace, variable_unsigned_integral, "stonesoup_i", stonesoup_i, &stonesoup_i, "CROSSOVER-STATE");
                        tracepoint(stonesoup_trace, variable_address, "stonesoup_buffer_array[stonesoup_i]", stonesoup_buffer_array[stonesoup_i], "CROSSOVER-STATE");
                    } else {
                        tracepoint(stonesoup_trace, variable_unsigned_integral, "stonesoup_i", stonesoup_i, &stonesoup_i, "TRIGGER-STATE");
                        tracepoint(stonesoup_trace, variable_address, "stonesoup_buffer_array[stonesoup_i]", stonesoup_buffer_array[stonesoup_i], "TRIGGER-STATE");
                    }
       }
       stonesoup_trace_counter ++;
   }
   if (stonesoup_buffer_array[stonesoup_i] != 0) {
    memset(stonesoup_buffer_array[stonesoup_i],0,stonesoup_size * sizeof(unsigned int ));
   }
   else
    break;
  }
 }
 tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
 tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
 tracepoint(stonesoup_trace, variable_unsigned_integral, "stonesoup_i", stonesoup_i, &stonesoup_i, "FINAL-STATE");
 stonesoup_printf("finished evaluating\n");
    for (stonesoup_i = 0; stonesoup_i < stonesoup_num; ++stonesoup_i)
        if (stonesoup_buffer_array[stonesoup_i] != 0)
            free(stonesoup_buffer_array[stonesoup_i]);
    if (stonesoup_buffer_array != 0) {
        free(stonesoup_buffer_array);
    }
    tracepoint(stonesoup_trace, weakness_end);
;
stonesoup_close_printf_context();
}
#else
# ifdef PEDANTIC
# endif
#endif
