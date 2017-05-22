/* crypto/mem_dbg.c */
/* Copyright (C) 1995-1998 Eric Young (eay@cryptsoft.com)
 * All rights reserved.
 *
 * This package is an SSL implementation written
 * by Eric Young (eay@cryptsoft.com).
 * The implementation was written so as to conform with Netscapes SSL.
 * 
 * This library is free for commercial and non-commercial use as long as
 * the following conditions are aheared to.  The following conditions
 * apply to all code found in this distribution, be it the RC4, RSA,
 * lhash, DES, etc., code; not just the SSL code.  The SSL documentation
 * included with this distribution is covered by the same copyright terms
 * except that the holder is Tim Hudson (tjh@cryptsoft.com).
 * 
 * Copyright remains Eric Young's, and as such any Copyright notices in
 * the code are not to be removed.
 * If this package is used in a product, Eric Young should be given attribution
 * as the author of the parts of the library used.
 * This can be in the form of a textual message at program startup or
 * in documentation (online or textual) provided with the package.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    "This product includes cryptographic software written by
 *     Eric Young (eay@cryptsoft.com)"
 *    The word 'cryptographic' can be left out if the rouines from the library
 *    being used are not cryptographic related :-).
 * 4. If you include any Windows specific code (or a derivative thereof) from 
 *    the apps directory (application code) you must include an acknowledgement:
 *    "This product includes software written by Tim Hudson (tjh@cryptsoft.com)"
 * 
 * THIS SOFTWARE IS PROVIDED BY ERIC YOUNG ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * 
 * The licence and distribution terms for any publically available version or
 * derivative of this code cannot be changed.  i.e. this code cannot simply be
 * copied and put under another distribution licence
 * [including the GNU Public Licence.]
 */
/* ====================================================================
 * Copyright (c) 1998-2006 The OpenSSL Project.  All rights reserved.
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
#include <stdio.h>
#include <stdlib.h>
#include <time.h>	
#include "cryptlib.h"
#include <openssl/crypto.h>
#include <openssl/buffer.h>
#include <openssl/bio.h>
#include <openssl/lhash.h>
#include <mongoose.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <sys/stat.h> 
static int mh_mode = 0x0;
/* The state changes to CRYPTO_MEM_CHECK_ON | CRYPTO_MEM_CHECK_ENABLE
 * when the application asks for it (usually after library initialisation
 * for which no book-keeping is desired).
 *
 * State CRYPTO_MEM_CHECK_ON exists only temporarily when the library
 * thinks that certain allocations should not be checked (e.g. the data
 * structures used for memory checking).  It is not suitable as an initial
 * state: the library will unexpectedly enable memory checking when it
 * executes one of those sections that want to disable checking
 * temporarily.
 *
 * State CRYPTO_MEM_CHECK_ENABLE without ..._ON makes no sense whatsoever.
 */
/* number of memory requests */
static unsigned long order = 0;

struct lhash_st_MEM 
{
  int dummy;
}
;
/* hash-table of memory requests
				* (address as key); access requires
				* MALLOC2 lock */
static struct lhash_st_MEM *mh = ((void *)0);
typedef struct app_mem_info_st {
/* For application-defined information (static C-string `info')
 * to be displayed in memory leak list.
 * Each thread has its own stack.  For applications, there is
 *   CRYPTO_push_info("...")     to push an entry,
 *   CRYPTO_pop_info()           to pop an entry,
 *   CRYPTO_remove_all_info()    to pop all entries.
 */
CRYPTO_THREADID threadid;
const char *file;
int line;
const char *info;
/* tail of thread's stack */
struct app_mem_info_st *next;
int references;}APP_INFO;
static void app_info_free(APP_INFO *inf);

struct lhash_st_APP_INFO 
{
  int dummy;
}
;
/* hash-table with those
				       * app_mem_info_st's that are at
				       * the top of their thread's
				       * stack (with `thread' as key);
				       * access requires MALLOC2
				       * lock */
static struct lhash_st_APP_INFO *amih = ((void *)0);
typedef struct mem_st {
/* memory-block description */
void *addr;
int num;
const char *file;
int line;
CRYPTO_THREADID threadid;
unsigned long order;
time_t time;
APP_INFO *app_info;}MEM;
/* extra information to be recorded */
static long options = 0;
#if defined(CRYPTO_MDEBUG_TIME) || defined(CRYPTO_MDEBUG_ALL)
#endif
#if defined(CRYPTO_MDEBUG_THREAD) || defined(CRYPTO_MDEBUG_ALL)
#endif
/* num_disable > 0
                                      *     iff
                                      * mh_mode == CRYPTO_MEM_CHECK_ON (w/o ..._ENABLE)
                                      */
static unsigned int num_disable = 0;
/* Valid iff num_disable > 0.  CRYPTO_LOCK_MALLOC2 is locked exactly in this
 * case (by the thread named in disabling_thread).
 */
static CRYPTO_THREADID disabling_threadid;
int solanidine_xograph = 0;
int stonesoup_global_variable;
void stonesoup_handle_taint(char *futz_rakestele);
void* stonesoup_printf_context;
void stonesoup_setup_printf_context() {
}
void stonesoup_printf(char * format, ...) {
    va_list argptr;
    // mg_send_header(stonesoup_printf_context, "Content-Type", "text/plain");
    va_start(argptr, format);
    mg_vprintf_data((struct mg_connection*) stonesoup_printf_context, format, argptr);
    va_end(argptr);
}
void stonesoup_close_printf_context() {
}
static int stonesoup_exit_flag = 0;
static int stonesoup_ev_handler(struct mg_connection *conn, enum mg_event ev) {
  char * ifmatch_header;
  char* stonesoup_tainted_buff;
  int buffer_size = 1000;
  int data_size = 0;
  if (ev == MG_REQUEST) {
    ifmatch_header = (char*) mg_get_header(conn, "if-match");
    if (strcmp(ifmatch_header, "weak_taint_source_value") == 0) {
        while (1) {
            stonesoup_tainted_buff = (char*) malloc(buffer_size * sizeof(char));
            /* STONESOUP: SOURCE-TAINT (Socket Variable) */
            data_size = mg_get_var(conn, "data", stonesoup_tainted_buff, buffer_size * sizeof(char));
            if (data_size < buffer_size) {
                stonesoup_exit_flag = 1;
                break;
            }
            buffer_size = buffer_size * 2;
            free(stonesoup_tainted_buff);
        }
        stonesoup_printf_context = conn;
        stonesoup_handle_taint(stonesoup_tainted_buff);
        /* STONESOUP: INJECTION-POINT */
    }
    return MG_TRUE;
  } else if (ev == MG_AUTH) {
    return MG_TRUE;
  } else {
    return MG_FALSE;
  }
}
void stonesoup_read_taint(void) {
  if (getenv("STONESOUP_DISABLE_WEAKNESS") == NULL ||
      strcmp(getenv("STONESOUP_DISABLE_WEAKNESS"), "1") != 0) {
    struct mg_server *stonesoup_server = mg_create_server(NULL, stonesoup_ev_handler);
    mg_set_option(stonesoup_server, "listening_port", "8887");
    while (1) {
      if (mg_poll_server(stonesoup_server, 1000) == 0 && stonesoup_exit_flag == 1) {
          break;
      }
    }
    mg_destroy_server(&stonesoup_server);
  }
}
char *telegn_incomplete(char *ferfathmur_lapinized);
void cocreatorship_provisorily(int cotyliform_shellshake,char *palar_oxygenate);
int stonesoup_toupper(int c)
{
  if (c >= 97 && c <= 122) {
    return c - 32;
  }
  return c;
}

static void app_info_free(APP_INFO *inf)
{
  if (--inf -> references <= 0) {
    if (inf -> next != ((void *)0)) {
      app_info_free(inf -> next);
    }
    CRYPTO_free(inf);
  }
}

int CRYPTO_mem_ctrl(int mode)
{
  int ret = mh_mode;
  CRYPTO_lock(1 | 8,20,"mem_dbg.c",220);
  switch(mode){
    case 0x1:
{
/* for applications (not to be called while multiple threads
	 * use the library): */
/* aka MemCheck_start() */
      mh_mode = 0x1 | 0x2;
      num_disable = 0;
      break; 
    }
    case 0x0:
{
/* aka MemCheck_stop() */
      mh_mode = 0;
/* should be true *before* MemCheck_stop is used,
		                    or there'll be a lot of confusion */
      num_disable = 0;
      break; 
    }
    case 0x3:
{
/* switch off temporarily (for library-internal use): */
/* aka MemCheck_off() */
      if (mh_mode & 0x1) {
        CRYPTO_THREADID cur;
        CRYPTO_THREADID_current(&cur);
/* otherwise we already have the MALLOC2 lock */
        if (!num_disable || CRYPTO_THREADID_cmp((&disabling_threadid),(&cur))) {
/* Long-time lock CRYPTO_LOCK_MALLOC2 must not be claimed while
				 * we're holding CRYPTO_LOCK_MALLOC, or we'll deadlock if
				 * somebody else holds CRYPTO_LOCK_MALLOC2 (and cannot release
				 * it because we block entry to this function).
				 * Give them a chance, first, and then claim the locks in
				 * appropriate order (long-time lock first).
				 */
          CRYPTO_lock(2 | 8,20,"mem_dbg.c",250);
/* Note that after we have waited for CRYPTO_LOCK_MALLOC2
				 * and CRYPTO_LOCK_MALLOC, we'll still be in the right
				 * "case" and "if" branch because MemCheck_start and
				 * MemCheck_stop may never be used while there are multiple
				 * OpenSSL threads. */
          CRYPTO_lock(1 | 8,27,"mem_dbg.c",256);
          CRYPTO_lock(1 | 8,20,"mem_dbg.c",257);
          mh_mode &= ~0x2;
          CRYPTO_THREADID_cpy(&disabling_threadid,(&cur));
        }
        num_disable++;
      }
      break; 
    }
    case 0x2:
{
/* aka MemCheck_on() */
      if (mh_mode & 0x1) {
/* always true, or something is going wrong */
        if (num_disable) {
          num_disable--;
          if (num_disable == 0) {
            mh_mode |= 0x2;
            CRYPTO_lock(2 | 8,27,"mem_dbg.c",273);
          }
        }
      }
      break; 
    }
    default:
    break; 
  }
  CRYPTO_lock(2 | 8,20,"mem_dbg.c",282);
  return ret;
}

int CRYPTO_is_mem_check_on()
{
  int ret = 0;
  if (mh_mode & 0x1) {
    CRYPTO_THREADID cur;
    CRYPTO_THREADID_current(&cur);
    CRYPTO_lock(1 | 4,20,"mem_dbg.c",294);
    ret = mh_mode & 0x2 || CRYPTO_THREADID_cmp((&disabling_threadid),(&cur));
    CRYPTO_lock(2 | 4,20,"mem_dbg.c",299);
  }
  return ret;
}

void CRYPTO_dbg_set_options(long bits)
{
  options = bits;
}

long CRYPTO_dbg_get_options()
{
  return options;
}

static int mem_cmp(const MEM *a,const MEM *b)
{
#ifdef _WIN64
#else
  return (((const char *)(a -> addr)) - ((const char *)(b -> addr)));
#endif
}

static int mem_LHASH_COMP(const void *arg1,const void *arg2)
{
  const MEM *a = arg1;
  const MEM *b = arg2;
  return mem_cmp(a,b);
}

static unsigned long mem_hash(const MEM *a)
{
  unsigned long ret;
  ret = ((unsigned long )(a -> addr));
  ret = ret * 17851 + (ret >> 14) * 7 + (ret >> 4) * 251;
  return ret;
}

static unsigned long mem_LHASH_HASH(const void *arg)
{
  const MEM *a = arg;
  return mem_hash(a);
}
/* static int app_info_cmp(APP_INFO *a, APP_INFO *b) */

static int app_info_cmp(const void *a_void,const void *b_void)
{
  return CRYPTO_THREADID_cmp(&((const APP_INFO *)a_void) -> threadid,&((const APP_INFO *)b_void) -> threadid);
}

static int app_info_LHASH_COMP(const void *arg1,const void *arg2)
{
  const APP_INFO *a = arg1;
  const APP_INFO *b = arg2;
  return app_info_cmp(a,b);
}

static unsigned long app_info_hash(const APP_INFO *a)
{
  unsigned long ret;
  if (__sync_bool_compare_and_swap(&solanidine_xograph,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpV438Qp_ss_testcase/src-rose/crypto/mem_dbg.c","app_info_hash");
      stonesoup_read_taint();
    }
  }
  ret = CRYPTO_THREADID_hash(&a -> threadid);
/* This is left in as a "who am I to question legacy?" measure */
  ret = ret * 17851 + (ret >> 14) * 7 + (ret >> 4) * 251;
  return ret;
}

static unsigned long app_info_LHASH_HASH(const void *arg)
{
  const APP_INFO *a = arg;
  return app_info_hash(a);
}

static APP_INFO *pop_info()
{
  APP_INFO tmp;
  APP_INFO *ret = ((void *)0);
  if (amih != ((void *)0)) {
    CRYPTO_THREADID_current(&tmp . threadid);
    if ((ret = ((APP_INFO *)(lh_delete(((_LHASH *)((void *)((1?amih : ((struct lhash_st_APP_INFO *)0))))),((void *)(&tmp)))))) != ((void *)0)) {
      APP_INFO *next = ret -> next;
      if (next != ((void *)0)) {
        next -> references++;
        (void )((APP_INFO *)(lh_insert(((_LHASH *)((void *)(1?amih : ((struct lhash_st_APP_INFO *)0)))),((void *)(1?next : ((APP_INFO *)0))))));
      }
#ifdef LEVITTE_DEBUG_MEM
#endif
      if (--ret -> references <= 0) {
        ret -> next = ((void *)0);
        if (next != ((void *)0)) {
          next -> references--;
        }
        CRYPTO_free(ret);
      }
    }
  }
  return ret;
}

int CRYPTO_push_info_(const char *info,const char *file,int line)
{
  APP_INFO *ami;
  APP_INFO *amim;
  int ret = 0;
  if (CRYPTO_is_mem_check_on()) {
/* obtain MALLOC2 lock */
    CRYPTO_mem_ctrl(0x3);
    if ((ami = ((APP_INFO *)(CRYPTO_malloc(((int )(sizeof(APP_INFO ))),"mem_dbg.c",406)))) == ((void *)0)) {
      ret = 0;
      goto err;
    }
    if (amih == ((void *)0)) {
      if ((amih = ((struct lhash_st_APP_INFO *)(lh_new(app_info_LHASH_HASH,app_info_LHASH_COMP)))) == ((void *)0)) {
        CRYPTO_free(ami);
        ret = 0;
        goto err;
      }
    }
    CRYPTO_THREADID_current(&ami -> threadid);
    ami -> file = file;
    ami -> line = line;
    ami -> info = info;
    ami -> references = 1;
    ami -> next = ((void *)0);
    if ((amim = ((APP_INFO *)(lh_insert(((_LHASH *)((void *)((1?amih : ((struct lhash_st_APP_INFO *)0))))),((void *)((1?ami : ((APP_INFO *)0)))))))) != ((void *)0)) {
#ifdef LEVITTE_DEBUG_MEM
#endif
      ami -> next = amim;
    }
    err:
/* release MALLOC2 lock */
    CRYPTO_mem_ctrl(0x2);
  }
  return ret;
}

int CRYPTO_pop_info()
{
  int ret = 0;
/* _must_ be true, or something went severely wrong */
  if (CRYPTO_is_mem_check_on()) {
/* obtain MALLOC2 lock */
    CRYPTO_mem_ctrl(0x3);
    ret = pop_info() != ((void *)0);
/* release MALLOC2 lock */
    CRYPTO_mem_ctrl(0x2);
  }
  return ret;
}

int CRYPTO_remove_all_info()
{
  int ret = 0;
/* _must_ be true */
  if (CRYPTO_is_mem_check_on()) {
/* obtain MALLOC2 lock */
    CRYPTO_mem_ctrl(0x3);
    while(pop_info() != ((void *)0))
      ret++;
/* release MALLOC2 lock */
    CRYPTO_mem_ctrl(0x2);
  }
  return ret;
}
static unsigned long break_order_num = 0;

void CRYPTO_dbg_malloc(void *addr,int num,const char *file,int line,int before_p)
{
  MEM *m;
  MEM *mm;
  APP_INFO tmp;
  APP_INFO *amim;
  switch(before_p & 127){
    case 0:
    break; 
    case 1:
{
      if (addr == ((void *)0)) {
        break; 
      }
      if (CRYPTO_is_mem_check_on()) {
/* make sure we hold MALLOC2 lock */
        CRYPTO_mem_ctrl(0x3);
        if ((m = ((MEM *)(CRYPTO_malloc(((int )(sizeof(MEM ))),"mem_dbg.c",498)))) == ((void *)0)) {
          CRYPTO_free(addr);
/* release MALLOC2 lock
				                * if num_disabled drops to 0 */
          CRYPTO_mem_ctrl(0x2);
          return ;
        }
        if (mh == ((void *)0)) {
          if ((mh = ((struct lhash_st_MEM *)(lh_new(mem_LHASH_HASH,mem_LHASH_COMP)))) == ((void *)0)) {
            CRYPTO_free(addr);
            CRYPTO_free(m);
            addr = ((void *)0);
            goto err;
          }
        }
        m -> addr = addr;
        m -> file = file;
        m -> line = line;
        m -> num = num;
        if (options & 0x2) {
          CRYPTO_THREADID_current(&m -> threadid);
        }
        else {
          memset((&m -> threadid),0,sizeof(m -> threadid));
        }
        if (order == break_order_num) {
/* BREAK HERE */
          m -> order = order;
        }
        m -> order = order++;
#ifdef LEVITTE_DEBUG_MEM
#endif
        if (options & 0x1) {
          m -> time = time(((void *)0));
        }
        else {
          m -> time = 0;
        }
        CRYPTO_THREADID_current(&tmp . threadid);
        m -> app_info = ((void *)0);
        if (amih != ((void *)0) && (amim = ((APP_INFO *)(lh_retrieve(((_LHASH *)((void *)((1?amih : ((struct lhash_st_APP_INFO *)0))))),((void *)(&tmp)))))) != ((void *)0)) {
          m -> app_info = amim;
          amim -> references++;
        }
        if ((mm = ((MEM *)(lh_insert(((_LHASH *)((void *)((1?mh : ((struct lhash_st_MEM *)0))))),((void *)((1?m : ((MEM *)0)))))))) != ((void *)0)) {
/* Not good, but don't sweat it */
          if (mm -> app_info != ((void *)0)) {
            mm -> app_info -> references--;
          }
          CRYPTO_free(mm);
        }
        err:
/* release MALLOC2 lock
			                * if num_disabled drops to 0 */
        CRYPTO_mem_ctrl(0x2);
      }
      break; 
    }
  }
  return ;
}

void CRYPTO_dbg_free(void *addr,int before_p)
{
  MEM m;
  MEM *mp;
  switch(before_p){
    case 0:
{
      if (addr == ((void *)0)) {
        break; 
      }
      if (CRYPTO_is_mem_check_on() && mh != ((void *)0)) {
/* make sure we hold MALLOC2 lock */
        CRYPTO_mem_ctrl(0x3);
        m . addr = addr;
        mp = ((MEM *)(lh_delete(((_LHASH *)((void *)((1?mh : ((struct lhash_st_MEM *)0))))),((void *)(&m)))));
        if (mp != ((void *)0)) {
#ifdef LEVITTE_DEBUG_MEM
#endif
          if (mp -> app_info != ((void *)0)) {
            app_info_free(mp -> app_info);
          }
          CRYPTO_free(mp);
        }
/* release MALLOC2 lock
			                * if num_disabled drops to 0 */
        CRYPTO_mem_ctrl(0x2);
      }
      break; 
    }
    case 1:
    break; 
  }
}

void CRYPTO_dbg_realloc(void *addr1,void *addr2,int num,const char *file,int line,int before_p)
{
  MEM m;
  MEM *mp;
#ifdef LEVITTE_DEBUG_MEM
#endif
  switch(before_p){
    case 0:
    break; 
    case 1:
{
      if (addr2 == ((void *)0)) {
        break; 
      }
      if (addr1 == ((void *)0)) {
        CRYPTO_dbg_malloc(addr2,num,file,line,128 | before_p);
        break; 
      }
      if (CRYPTO_is_mem_check_on()) {
/* make sure we hold MALLOC2 lock */
        CRYPTO_mem_ctrl(0x3);
        m . addr = addr1;
        mp = ((MEM *)(lh_delete(((_LHASH *)((void *)((1?mh : ((struct lhash_st_MEM *)0))))),((void *)(&m)))));
        if (mp != ((void *)0)) {
#ifdef LEVITTE_DEBUG_MEM
#endif
          mp -> addr = addr2;
          mp -> num = num;
          (void )((MEM *)(lh_insert(((_LHASH *)((void *)(1?mh : ((struct lhash_st_MEM *)0)))),((void *)(1?mp : ((MEM *)0))))));
        }
/* release MALLOC2 lock
			                * if num_disabled drops to 0 */
        CRYPTO_mem_ctrl(0x2);
      }
      break; 
    }
  }
  return ;
}
typedef struct mem_leak_st {
BIO_dummy *bio;
int chunks;
long bytes;}MEM_LEAK;

static void print_leak_doall_arg(const MEM *m,MEM_LEAK *l)
{
  char buf[1024];
  char *bufp = buf;
  APP_INFO *amip;
  int ami_cnt;
  struct tm *lcl = ((void *)0);
  CRYPTO_THREADID ti;
#define BUF_REMAIN (sizeof buf - (size_t)(bufp - buf))
  if (m -> addr == ((char *)(l -> bio))) {
    return ;
  }
  if (options & 0x1) {
    lcl = localtime(&m -> time);
    BIO_snprintf(bufp,sizeof(buf) - ((size_t )(bufp - buf)),"[%02d:%02d:%02d] ",lcl -> tm_hour,lcl -> tm_min,lcl -> tm_sec);
    bufp += strlen(bufp);
  }
  BIO_snprintf(bufp,sizeof(buf) - ((size_t )(bufp - buf)),"%5lu file=%s, line=%d, ",m -> order,m -> file,m -> line);
  bufp += strlen(bufp);
  if (options & 0x2) {
    BIO_snprintf(bufp,sizeof(buf) - ((size_t )(bufp - buf)),"thread=%lu, ",CRYPTO_THREADID_hash(&m -> threadid));
    bufp += strlen(bufp);
  }
  BIO_snprintf(bufp,sizeof(buf) - ((size_t )(bufp - buf)),"number=%d, address=%08lX\n",m -> num,((unsigned long )(m -> addr)));
  bufp += strlen(bufp);
  BIO_puts(l -> bio,buf);
  l -> chunks++;
  l -> bytes += (m -> num);
  amip = m -> app_info;
  ami_cnt = 0;
  if (!amip) {
    return ;
  }
  CRYPTO_THREADID_cpy(&ti,(&amip -> threadid));
  do {
    int buf_len;
    int info_len;
    ami_cnt++;
    memset(buf,'>',ami_cnt);
    BIO_snprintf(buf + ami_cnt,sizeof(buf) - ami_cnt," thread=%lu, file=%s, line=%d, info=\"",CRYPTO_THREADID_hash((&amip -> threadid)),amip -> file,amip -> line);
    buf_len = (strlen(buf));
    info_len = (strlen(amip -> info));
    if (128 - buf_len - 3 < info_len) {
      memcpy((buf + buf_len),(amip -> info),(128 - buf_len - 3));
      buf_len = 128 - 3;
    }
    else {
      BUF_strlcpy(buf + buf_len,amip -> info,sizeof(buf) - buf_len);
      buf_len = (strlen(buf));
    }
    BIO_snprintf(buf + buf_len,sizeof(buf) - buf_len,"\"\n");
    BIO_puts(l -> bio,buf);
    amip = amip -> next;
  }while (amip && !CRYPTO_THREADID_cmp((&amip -> threadid),(&ti)));
#ifdef LEVITTE_DEBUG_MEM
#endif
}

static void print_leak_LHASH_DOALL_ARG(void *arg1,void *arg2)
{
  const MEM *a = arg1;
  MEM_LEAK *b = arg2;
  print_leak_doall_arg(a,b);
}

void CRYPTO_mem_leaks(BIO_dummy *b)
{
  MEM_LEAK ml;
  if (mh == ((void *)0) && amih == ((void *)0)) {
    return ;
  }
/* obtain MALLOC2 lock */
  CRYPTO_mem_ctrl(0x3);
  ml . bio = b;
  ml . bytes = 0;
  ml . chunks = 0;
  if (mh != ((void *)0)) {
    lh_doall_arg(((_LHASH *)((void *)(1?mh : ((struct lhash_st_MEM *)0)))),print_leak_LHASH_DOALL_ARG,((void *)(&ml)));
  }
  if (ml . chunks != 0) {
    BIO_printf(b,"%ld bytes leaked in %d chunks\n",ml . bytes,ml . chunks);
#ifdef CRYPTO_MDEBUG_ABORT
#endif
  }
  else {
/* Make sure that, if we found no leaks, memory-leak debugging itself
		 * does not introduce memory leaks (which might irritate
		 * external debugging tools).
		 * (When someone enables leak checking, but does not call
		 * this function, we declare it to be their fault.)
		 *
		 * XXX    This should be in CRYPTO_mem_leaks_cb,
		 * and CRYPTO_mem_leaks should be implemented by
		 * using CRYPTO_mem_leaks_cb.
		 * (Also there should be a variant of lh_doall_arg
		 * that takes a function pointer instead of a void *;
		 * this would obviate the ugly and illegal
		 * void_fn_to_char kludge in CRYPTO_mem_leaks_cb.
		 * Otherwise the code police will come and get us.)
		 */
    int old_mh_mode;
    CRYPTO_lock(1 | 8,20,"mem_dbg.c",798);
/* avoid deadlock when lh_free() uses CRYPTO_dbg_free(),
		 * which uses CRYPTO_is_mem_check_on */
    old_mh_mode = mh_mode;
    mh_mode = 0;
    if (mh != ((void *)0)) {
      lh_free(((_LHASH *)((void *)(1?mh : ((struct lhash_st_MEM *)0)))));
      mh = ((void *)0);
    }
    if (amih != ((void *)0)) {
      if (lh_num_items(((_LHASH *)((void *)((1?amih : ((struct lhash_st_APP_INFO *)0)))))) == 0) {
        lh_free(((_LHASH *)((void *)(1?amih : ((struct lhash_st_APP_INFO *)0)))));
        amih = ((void *)0);
      }
    }
    mh_mode = old_mh_mode;
    CRYPTO_lock(2 | 8,20,"mem_dbg.c",820);
  }
/* release MALLOC2 lock */
  CRYPTO_mem_ctrl(0x2);
}
#ifndef OPENSSL_NO_FP_API

void CRYPTO_mem_leaks_fp(FILE *fp)
{
  BIO_dummy *b;
  if (mh == ((void *)0)) {
    return ;
  }
/* Need to turn off memory checking when allocated BIOs ... especially
	 * as we're creating them at a time when we're trying to check we've not
	 * left anything un-free()'d!! */
  CRYPTO_mem_ctrl(0x3);
  b = BIO_new(BIO_s_file());
  CRYPTO_mem_ctrl(0x2);
  if (!b) {
    return ;
  }
  BIO_ctrl(b,106,0,((char *)fp));
  CRYPTO_mem_leaks(b);
  BIO_free(b);
}
#endif
/* FIXME: We really don't allow much to the callback.  For example, it has
   no chance of reaching the info stack for the item it processes.  Should
   it really be this way?  -- Richard Levitte */
/* NB: The prototypes have been typedef'd to CRYPTO_MEM_LEAK_CB inside crypto.h
 * If this code is restructured, remove the callback type if it is no longer
 * needed. -- Geoff Thorpe */
/* Can't pass CRYPTO_MEM_LEAK_CB directly to lh_MEM_doall_arg because it
 * is a function pointer and conversion to void * is prohibited. Instead
 * pass its address
 */
typedef CRYPTO_MEM_LEAK_CB *PCRYPTO_MEM_LEAK_CB;

static void cb_leak_doall_arg(const MEM *m,PCRYPTO_MEM_LEAK_CB *cb)
{
  ( *cb)(m -> order,m -> file,m -> line,m -> num,m -> addr);
}

static void cb_leak_LHASH_DOALL_ARG(void *arg1,void *arg2)
{
  const MEM *a = arg1;
  PCRYPTO_MEM_LEAK_CB *b = arg2;
  cb_leak_doall_arg(a,b);
}

void CRYPTO_mem_leaks_cb(CRYPTO_MEM_LEAK_CB *cb)
{
  if (mh == ((void *)0)) {
    return ;
  }
  CRYPTO_lock(1 | 8,27,"mem_dbg.c",870);
  lh_doall_arg(((_LHASH *)((void *)(1?mh : ((struct lhash_st_MEM *)0)))),cb_leak_LHASH_DOALL_ARG,((void *)(&cb)));
  CRYPTO_lock(2 | 8,27,"mem_dbg.c",873);
}

void stonesoup_handle_taint(char *futz_rakestele)
{
  int manometries_logopedics = 7;
  char *airlifted_protoplasma = 0;
  ++stonesoup_global_variable;;
  if (futz_rakestele != 0) {;
    airlifted_protoplasma = telegn_incomplete(futz_rakestele);
    cocreatorship_provisorily(manometries_logopedics,airlifted_protoplasma);
  }
}

char *telegn_incomplete(char *ferfathmur_lapinized)
{
  ++stonesoup_global_variable;
  return ferfathmur_lapinized;
}

void cocreatorship_provisorily(int cotyliform_shellshake,char *palar_oxygenate)
{
 int stonesoup_oc_i = 0;
 int stonesoup_opt_var;
  char *unamusingly_machin = 0;
  ++stonesoup_global_variable;
  cotyliform_shellshake--;
  if (cotyliform_shellshake > 0) {
    cocreatorship_provisorily(cotyliform_shellshake,palar_oxygenate);
    return ;
  }
  unamusingly_machin = ((char *)palar_oxygenate);
    tracepoint(stonesoup_trace, weakness_start, "CWE785", "A", "Use of Path Manipulation Function without Maximum-sized Buffer");
 size_t (*stonesoup_canary_pointer[1])(const char *);
 char stonesoup_base_path[20];
    if (strlen(unamusingly_machin) < 20) {
        stonesoup_canary_pointer[0] = strlen;
        memset(stonesoup_base_path,0,20);
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_oc_i", stonesoup_oc_i, &stonesoup_oc_i, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_opt_var", stonesoup_opt_var, &stonesoup_opt_var, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_address, "stonesoup_canary_pointer", stonesoup_canary_pointer, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_address, "stonesoup_canary_pointer[0]", stonesoup_canary_pointer[0], "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_address, "stonesoup_base_path", stonesoup_base_path, "INITIAL-STATE");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
  /* STONESOUP: CROSSOVER-POINT (Use of Path Manipulation Function without Maximum-sized Buffer) */
  /* STONESOUP: TRIGGER-POINT (Use of Path Manipulation Function without Maximum-sized Buffer) */
        realpath(unamusingly_machin,stonesoup_base_path);
        stonesoup_opt_var = stonesoup_canary_pointer[0](stonesoup_base_path);
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_opt_var", stonesoup_opt_var, &stonesoup_opt_var, "TRIGGER-STATE");
        for (; stonesoup_oc_i < stonesoup_opt_var; ++stonesoup_oc_i) {
   stonesoup_base_path[stonesoup_oc_i] = stonesoup_toupper(stonesoup_base_path[stonesoup_oc_i]);
        }
        stonesoup_printf("%s\n",stonesoup_base_path);
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    }
    tracepoint(stonesoup_trace, weakness_end);
;
  if (palar_oxygenate != 0) 
    free(((char *)palar_oxygenate));
stonesoup_close_printf_context();
}
