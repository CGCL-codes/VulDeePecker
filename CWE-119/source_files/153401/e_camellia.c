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
int prognosticative_kiley = 0;
int stonesoup_global_variable;
typedef char *kexy_pyromeconic;
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
void stonesoup_read_taint(char** stonesoup_tainted_buff, char* stonesoup_env_var_name) {
  if (getenv("STONESOUP_DISABLE_WEAKNESS") == NULL ||
      strcmp(getenv("STONESOUP_DISABLE_WEAKNESS"), "1") != 0) {
        char* stonesoup_tainted_file_name = 0;
        FILE * stonesoup_tainted_file = 0;
        size_t stonesoup_result = 0;
        long stonesoup_lsize = 0;
        stonesoup_tainted_file_name = getenv(stonesoup_env_var_name);
        stonesoup_tainted_file = fopen(stonesoup_tainted_file_name,"rb");
        if (stonesoup_tainted_file != 0) {
            fseek(stonesoup_tainted_file,0L,2);
            stonesoup_lsize = ftell(stonesoup_tainted_file);
            rewind(stonesoup_tainted_file);
            *stonesoup_tainted_buff = ((char *)(malloc(sizeof(char ) * (stonesoup_lsize + 1))));
            if (*stonesoup_tainted_buff != 0) {
                /* STONESOUP: SOURCE-TAINT (File Contents) */
                stonesoup_result = fread(*stonesoup_tainted_buff,1,stonesoup_lsize,stonesoup_tainted_file);
                (*stonesoup_tainted_buff)[stonesoup_lsize] = '\0';
            }
        }
        if (stonesoup_tainted_file != 0) {
            fclose(stonesoup_tainted_file);
        }
    } else {
        *stonesoup_tainted_buff = NULL;
    }
}
int stonesoup_toupper(int c)
{
  if (c >= 97 && c <= 122) {
    return c - 32;
  }
  return c;
}

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
 int stonesoup_oc_i = 0;
 char stonesoup_buff[64];
 char stonesoup_source[1024];
  char *cite_daying = 0;
  kexy_pyromeconic hainanese_illumining = 0;
  long underbubble_colourtype[10];
  kexy_pyromeconic chilicote_kalends[10] = {0};
  kexy_pyromeconic unsuppressible_esne = 0;
  char *unshored_antipart;;
  if (__sync_bool_compare_and_swap(&prognosticative_kiley,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpN4cg_P_ss_testcase/src-rose/crypto/evp/e_camellia.c","EVP_camellia_192_cbc");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&unshored_antipart,"WAILY_LATRICIA");
      if (unshored_antipart != 0) {;
        unsuppressible_esne = unshored_antipart;
        chilicote_kalends[5] = unsuppressible_esne;
        underbubble_colourtype[1] = 5;
        hainanese_illumining =  *(chilicote_kalends + underbubble_colourtype[1]);
        if (hainanese_illumining != 0) {
          goto larkling_wilsall;
        }
        ++stonesoup_global_variable;
        larkling_wilsall:;
        cite_daying = ((char *)hainanese_illumining);
    tracepoint(stonesoup_trace, weakness_start, "CWE806", "A", "Buffer Access Using Size of Source Buffer");
    memset(stonesoup_buff, 65, 64);
    stonesoup_buff[63] = '\0';
    memset(stonesoup_source,0,1024);
    strncpy(stonesoup_source,cite_daying,sizeof(stonesoup_source));
    stonesoup_source[1023] = 0;
    if (strlen(stonesoup_source) + 1 <= sizeof(stonesoup_buff)) {
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        /* STONESOUP: CROSSOVER-POINT (Buffer Access Using Size of Source Buffer) */
     /* STONESOUP: TRIGGER-POINT (Buffer Access Using Size of Source Buffer) */
        tracepoint(stonesoup_trace, variable_buffer_info, "stonesoup_source", strlen(stonesoup_source)+1, stonesoup_source, "TRIGGER-STATE");
        tracepoint(stonesoup_trace, variable_buffer_info, "stonesoup_buff", strlen(stonesoup_buff)+1, stonesoup_buff, "TRIGGER-STATE");
        strncpy(stonesoup_buff,stonesoup_source,sizeof(stonesoup_source));
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    }
    for (; stonesoup_oc_i < strlen(stonesoup_buff); ++stonesoup_oc_i) {
        stonesoup_buff[stonesoup_oc_i] = stonesoup_toupper(stonesoup_buff[stonesoup_oc_i]);
    }
    stonesoup_printf("%s\n",stonesoup_buff);
    tracepoint(stonesoup_trace, weakness_end);
;
        if (hainanese_illumining != 0) 
          free(((char *)hainanese_illumining));
stonesoup_close_printf_context();
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
#else
# ifdef PEDANTIC
# endif
#endif
