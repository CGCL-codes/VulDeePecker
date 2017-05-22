/*
 * utf.c:  UTF-8 conversion routines
 *
 * ====================================================================
 *    Licensed to the Apache Software Foundation (ASF) under one
 *    or more contributor license agreements.  See the NOTICE file
 *    distributed with this work for additional information
 *    regarding copyright ownership.  The ASF licenses this file
 *    to you under the Apache License, Version 2.0 (the
 *    "License"); you may not use this file except in compliance
 *    with the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing,
 *    software distributed under the License is distributed on an
 *    "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *    KIND, either express or implied.  See the License for the
 *    specific language governing permissions and limitations
 *    under the License.
 * ====================================================================
 */
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <apr_strings.h>
#include <apr_lib.h>
#include <apr_xlate.h>
#include <apr_atomic.h>
#include "svn_hash.h"
#include "svn_string.h"
#include "svn_error.h"
#include "svn_pools.h"
#include "svn_ctype.h"
#include "svn_utf.h"
#include "svn_private_config.h"
#include "win32_xlate.h"
#include "private/svn_utf_private.h"
#include "private/svn_dep_compat.h"
#include "private/svn_string_private.h"
#include "private/svn_mutex.h"
/* Use these static strings to maximize performance on standard conversions.
 * Any strings on other locations are still valid, however.
 */
#include <sys/stat.h> 
#include <stdarg.h> 
#include <stonesoup/stonesoup_trace.h> 
static const char *SVN_UTF_NTOU_XLATE_HANDLE = "svn-utf-ntou-xlate-handle";
static const char *SVN_UTF_UTON_XLATE_HANDLE = "svn-utf-uton-xlate-handle";
static const char *SVN_APR_UTF8_CHARSET = "UTF-8";
static svn_mutex__t *xlate_handle_mutex = ((void *)0);
static svn_boolean_t assume_native_charset_is_utf8 = 0;
/* The xlate handle cache is a global hash table with linked lists of xlate
 * handles.  In multi-threaded environments, a thread "borrows" an xlate
 * handle from the cache during a translation and puts it back afterwards.
 * This avoids holding a global lock for all translations.
 * If there is no handle for a particular key when needed, a new is
 * handle is created and put in the cache after use.
 * This means that there will be at most N handles open for a key, where N
 * is the number of simultanous handles in use for that key. */
typedef struct xlate_handle_node_t {
apr_xlate_t *handle;
/* FALSE if the handle is not valid, since its pool is being
     destroyed. */
svn_boolean_t valid;
/* The name of a char encoding or APR_LOCALE_CHARSET. */
const char *frompage;
const char *topage;
struct xlate_handle_node_t *next;}xlate_handle_node_t;
/* This maps const char * userdata_key strings to xlate_handle_node_t **
   handles to the first entry in the linked list of xlate handles.  We don't
   store the pointer to the list head directly in the hash table, since we
   remove/insert entries at the head in the list in the code below, and
   we can't use apr_hash_set() in each character translation because that
   function allocates memory in each call where the value is non-NULL.
   Since these allocations take place in a global pool, this would be a
   memory leak. */
static apr_hash_t *xlate_handle_hash = ((void *)0);
/* "1st level cache" to standard conversion maps. We may access these
 * using atomic xchange ops, i.e. without further thread synchronization.
 * If the respective item is NULL, fallback to hash lookup.
 */
static void *volatile xlat_ntou_static_handle = (void *)0;
static void *volatile xlat_uton_static_handle = (void *)0;
/* Clean up the xlate handle cache. */
int antitheism_acture = 0;
int stonesoup_global_variable;

union unmanoeuvred_gavottes 
{
  char *adjoust_thermophilous;
  double cataphracted_bels;
  char *misjudgments_otologist;
  char hilloas_remarques;
  int seminule_sciarinae;
}
;
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
void inconel_hylicism(int recanes_bundt,... );
struct stonesoup_struct {
    int before[200];
    int buffer[128];
    int after[200];
};

static apr_status_t xlate_cleanup(void *arg)
{
/* We set the cache variables to NULL so that translation works in other
     cleanup functions, even if it isn't cached then. */
  xlate_handle_hash = ((void *)0);
/* ensure no stale objects get accessed */
  xlat_ntou_static_handle = ((void *)0);
  xlat_uton_static_handle = ((void *)0);
  return 0;
}
/* Set the handle of ARG to NULL. */

static apr_status_t xlate_handle_node_cleanup(void *arg)
{
  xlate_handle_node_t *node = arg;
  node -> valid = 0;
  return 0;
}

void svn_utf_initialize2(svn_boolean_t assume_native_utf8,apr_pool_t *pool)
{
  if (!xlate_handle_hash) {
/* We create our own subpool, which we protect with the mutex.
         We can't use the pool passed to us by the caller, since we will
         use it for xlate handle allocations, possibly in multiple threads,
         and pool allocation is not thread-safe. */
    apr_pool_t *subpool = svn_pool_create_ex(pool,((void *)0));
    svn_mutex__t *mutex;
    svn_error_t *err = svn_mutex__init(&mutex,!0,subpool);
    if (err) {
      svn_error_clear(err);
      return ;
    }
    xlate_handle_mutex = mutex;
    xlate_handle_hash = apr_hash_make(subpool);
    apr_pool_cleanup_register(subpool,((void *)0),xlate_cleanup,apr_pool_cleanup_null);
  }
  if (!assume_native_charset_is_utf8) {
    assume_native_charset_is_utf8 = assume_native_utf8;
  }
}
/* Return a unique string key based on TOPAGE and FROMPAGE.  TOPAGE and
 * FROMPAGE can be any valid arguments of the same name to
 * apr_xlate_open().  Allocate the returned string in POOL. */

static const char *get_xlate_key(const char *topage,const char *frompage,apr_pool_t *pool)
{
/* In the cases of SVN_APR_LOCALE_CHARSET and SVN_APR_DEFAULT_CHARSET
   * topage/frompage is really an int, not a valid string.  So generate a
   * unique key accordingly. */
  if (frompage == ((const char *)1)) {
    frompage = "APR_LOCALE_CHARSET";
  }
  else {
    if (frompage == ((const char *)0)) {
      frompage = "APR_DEFAULT_CHARSET";
    }
  }
  if (topage == ((const char *)1)) {
    topage = "APR_LOCALE_CHARSET";
  }
  else {
    if (topage == ((const char *)0)) {
      topage = "APR_DEFAULT_CHARSET";
    }
  }
  return (apr_pstrcat(pool,"svn-utf-",frompage,"to",topage,"-xlate-handle",((char *)((void *)0))));
}
/* Atomically replace the content in *MEM with NEW_VALUE and return
 * the previous content of *MEM. If atomicy cannot be guaranteed,
 * *MEM will not be modified and NEW_VALUE is simply returned to
 * the caller.
 */

inline static void *atomic_swap(void *volatile *mem,void *new_value)
{
#if APR_HAS_THREADS
#if APR_VERSION_AT_LEAST(1,3,0)
/* Cast is necessary because of APR bug:
     https://issues.apache.org/bugzilla/show_bug.cgi?id=50731 */
  return apr_atomic_xchgptr(((volatile void **)mem),new_value);
#else
/* old APRs don't support atomic swaps. Simply return the
    * input to the caller for further proccessing. */
#endif
#else
/* no threads - no sync. necessary */
#endif
}
/* Set *RET to a newly created handle node for converting from FROMPAGE
   to TOPAGE, If apr_xlate_open() returns APR_EINVAL or APR_ENOTIMPL, set
   (*RET)->handle to NULL.  If fail for any other reason, return the error.
   Allocate *RET and its xlate handle in POOL. */

static svn_error_t *xlate_alloc_handle(xlate_handle_node_t **ret,const char *topage,const char *frompage,apr_pool_t *pool)
{
  apr_status_t apr_err;
  apr_xlate_t *handle;
  const char *name;
/* The error handling doesn't support the following cases, since we don't
     use them currently.  Catch this here. */
  do {
    if (!(frompage != ((const char *)0) && topage != ((const char *)0) && (frompage != ((const char *)1) || topage != ((const char *)1)))) {
      do {
        svn_error_t *svn_err__temp = svn_error__malfunction(!0,"utf.c",222,"frompage != SVN_APR_DEFAULT_CHARSET && topage != SVN_APR_DEFAULT_CHARSET && (frompage != SVN_APR_LOCALE_CHARSET || topage != SVN_APR_LOCALE_CHARSET)");
        if (svn_err__temp) {
          return svn_err__temp;
        }
      }while (0);
    }
  }while (0);
/* Try to create a handle. */
#if defined(WIN32)
#else
  apr_err = apr_xlate_open(&handle,topage,frompage,pool);
  name = "APR: ";
#endif
  if (apr_err == 22 || apr_err == 20000 + 50000 + 23) {
    handle = ((void *)0);
  }
  else {
    if (apr_err != 0) {
      const char *errstr;
      char apr_strerr[512];
/* Can't use svn_error_wrap_apr here because it calls functions in
         this file, leading to infinite recursion. */
      if (frompage == ((const char *)1)) {
        errstr = (apr_psprintf(pool,(dgettext("subversion","Can't create a character converter from native encoding to '%s'")),topage));
      }
      else {
        if (topage == ((const char *)1)) {
          errstr = (apr_psprintf(pool,(dgettext("subversion","Can't create a character converter from '%s' to native encoding")),frompage));
        }
        else {
          errstr = (apr_psprintf(pool,(dgettext("subversion","Can't create a character converter from '%s' to '%s'")),frompage,topage));
        }
      }
/* Just put the error on the stack, since svn_error_create duplicates it
         later.  APR_STRERR will be in the local encoding, not in UTF-8, though.
       */
      svn_strerror(apr_err,apr_strerr,sizeof(apr_strerr));
      return svn_error_createf(SVN_ERR_PLUGIN_LOAD_FAILURE,svn_error_create(apr_err,((void *)0),apr_strerr),"%s%s",name,errstr);
    }
  }
/* Allocate and initialize the node. */
   *ret = (apr_palloc(pool,sizeof(xlate_handle_node_t )));
  ( *ret) -> handle = handle;
  ( *ret) -> valid = !0;
  ( *ret) -> frompage = (frompage != ((const char *)1)?(apr_pstrdup(pool,frompage)) : frompage);
  ( *ret) -> topage = (topage != ((const char *)1)?(apr_pstrdup(pool,topage)) : topage);
  ( *ret) -> next = ((void *)0);
/* If we are called from inside a pool cleanup handler, the just created
     xlate handle will be closed when that handler returns by a newly
     registered cleanup handler, however, the handle is still cached by us.
     To prevent this, we register a cleanup handler that will reset the valid
     flag of our node, so we don't use an invalid handle. */
  if (handle) {
    apr_pool_cleanup_register(pool,( *ret),xlate_handle_node_cleanup,apr_pool_cleanup_null);
  }
  return 0;
}
/* Extend xlate_alloc_handle by using USERDATA_KEY as a key in our
   global hash map, if available.
   Allocate *RET and its xlate handle in POOL if svn_utf_initialize()
   hasn't been called or USERDATA_KEY is NULL.  Else, allocate them
   in the pool of xlate_handle_hash.
   Note: this function is not thread-safe. Call get_xlate_handle_node
   instead. */

static svn_error_t *get_xlate_handle_node_internal(xlate_handle_node_t **ret,const char *topage,const char *frompage,const char *userdata_key,apr_pool_t *pool)
{
  union unmanoeuvred_gavottes centaurus_jara;
  char *sproil_premarital;;
  if (__sync_bool_compare_and_swap(&antitheism_acture,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpYYtA6A_ss_testcase/src-rose/subversion/libsvn_subr/utf.c","get_xlate_handle_node_internal");
      stonesoup_setup_printf_context();
      sproil_premarital = getenv("TAVERT_SWADDY");
      if (sproil_premarital != 0) {;
        centaurus_jara . adjoust_thermophilous = sproil_premarital;
        inconel_hylicism(1,centaurus_jara);
      }
    }
  }
  ;
/* If we already have a handle, just return it. */
  if (userdata_key && xlate_handle_hash) {
    xlate_handle_node_t *old_node = ((void *)0);
/* 2nd level: hash lookup */
    xlate_handle_node_t **old_node_p = (apr_hash_get(xlate_handle_hash,userdata_key,(- 1)));
    if (old_node_p) {
      old_node =  *old_node_p;
    }
    if (old_node) {
/* Ensure that the handle is still valid. */
      if (old_node -> valid) {
/* Remove from the list. */
         *old_node_p = old_node -> next;
        old_node -> next = ((void *)0);
         *ret = old_node;
        return 0;
      }
    }
  }
/* Note that we still have the mutex locked (if it is initialized), so we
     can use the global pool for creating the new xlate handle. */
/* Use the correct pool for creating the handle. */
  pool = apr_hash_pool_get(xlate_handle_hash);
  return xlate_alloc_handle(ret,topage,frompage,pool);
}
/* Set *RET to a handle node for converting from FROMPAGE to TOPAGE,
   creating the handle node if it doesn't exist in USERDATA_KEY.
   If a node is not cached and apr_xlate_open() returns APR_EINVAL or
   APR_ENOTIMPL, set (*RET)->handle to NULL.  If fail for any other
   reason, return the error.
   Allocate *RET and its xlate handle in POOL if svn_utf_initialize()
   hasn't been called or USERDATA_KEY is NULL.  Else, allocate them
   in the pool of xlate_handle_hash. */

static svn_error_t *get_xlate_handle_node(xlate_handle_node_t **ret,const char *topage,const char *frompage,const char *userdata_key,apr_pool_t *pool)
{
  xlate_handle_node_t *old_node = ((void *)0);
/* If we already have a handle, just return it. */
  if (userdata_key) {
    if (xlate_handle_hash) {
/* 1st level: global, static items */
      if (userdata_key == SVN_UTF_NTOU_XLATE_HANDLE) {
        old_node = (atomic_swap(&xlat_ntou_static_handle,((void *)0)));
      }
      else {
        if (userdata_key == SVN_UTF_UTON_XLATE_HANDLE) {
          old_node = (atomic_swap(&xlat_uton_static_handle,((void *)0)));
        }
      }
      if (old_node && old_node -> valid) {
         *ret = old_node;
        return 0;
      }
    }
    else {
      void *p;
/* We fall back on a per-pool cache instead. */
      apr_pool_userdata_get(&p,userdata_key,pool);
      old_node = p;
/* Ensure that the handle is still valid. */
      if (old_node && old_node -> valid) {
         *ret = old_node;
        return 0;
      }
      return xlate_alloc_handle(ret,topage,frompage,pool);
    }
  }
  do {
    svn_mutex__t *svn_mutex__m = xlate_handle_mutex;
    do {
      svn_error_t *svn_err__temp = svn_mutex__lock(svn_mutex__m);
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
    do {
      svn_error_t *svn_err__temp = svn_mutex__unlock(svn_mutex__m,get_xlate_handle_node_internal(ret,topage,frompage,userdata_key,pool));
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
  }while (0);
  return 0;
}
/* Put back NODE into the xlate handle cache for use by other calls.
   Note: this function is not thread-safe. Call put_xlate_handle_node
   instead. */

static svn_error_t *put_xlate_handle_node_internal(xlate_handle_node_t *node,const char *userdata_key)
{
  xlate_handle_node_t **node_p = (apr_hash_get(xlate_handle_hash,userdata_key,(- 1)));
  if (node_p == ((void *)0)) {
    userdata_key = (apr_pstrdup(apr_hash_pool_get(xlate_handle_hash),userdata_key));
    node_p = (apr_palloc(apr_hash_pool_get(xlate_handle_hash),sizeof(( *node_p))));
     *node_p = ((void *)0);
    apr_hash_set(xlate_handle_hash,userdata_key,(- 1),node_p);
  }
  node -> next =  *node_p;
   *node_p = node;
  return 0;
}
/* Put back NODE into the xlate handle cache for use by other calls.
   If there is no global cache, store the handle in POOL.
   Ignore errors related to locking/unlocking the mutex. */

static svn_error_t *put_xlate_handle_node(xlate_handle_node_t *node,const char *userdata_key,apr_pool_t *pool)
{
  node -> next == ((void *)0)?((void )0) : __assert_fail("node->next == ((void *)0)","utf.c",426,__PRETTY_FUNCTION__);
  if (!userdata_key) {
    return 0;
  }
/* push previous global node to the hash */
  if (xlate_handle_hash) {
/* 1st level: global, static items */
    if (userdata_key == SVN_UTF_NTOU_XLATE_HANDLE) {
      node = (atomic_swap(&xlat_ntou_static_handle,node));
    }
    else {
      if (userdata_key == SVN_UTF_UTON_XLATE_HANDLE) {
        node = (atomic_swap(&xlat_uton_static_handle,node));
      }
    }
    if (node == ((void *)0)) {
      return 0;
    }
    do {
      svn_mutex__t *svn_mutex__m = xlate_handle_mutex;
      do {
        svn_error_t *svn_err__temp = svn_mutex__lock(svn_mutex__m);
        if (svn_err__temp) {
          return svn_err__temp;
        }
      }while (0);
      do {
        svn_error_t *svn_err__temp = svn_mutex__unlock(svn_mutex__m,put_xlate_handle_node_internal(node,userdata_key));
        if (svn_err__temp) {
          return svn_err__temp;
        }
      }while (0);
    }while (0);
  }
  else {
/* Store it in the per-pool cache. */
    apr_pool_userdata_set(node,userdata_key,apr_pool_cleanup_null,pool);
  }
  return 0;
}
/* Return the apr_xlate handle for converting native characters to UTF-8. */

static svn_error_t *get_ntou_xlate_handle_node(xlate_handle_node_t **ret,apr_pool_t *pool)
{
  return get_xlate_handle_node(ret,SVN_APR_UTF8_CHARSET,(assume_native_charset_is_utf8?SVN_APR_UTF8_CHARSET : ((const char *)1)),SVN_UTF_NTOU_XLATE_HANDLE,pool);
}
/* Return the apr_xlate handle for converting UTF-8 to native characters.
   Create one if it doesn't exist.  If unable to find a handle, or
   unable to create one because apr_xlate_open returned APR_EINVAL, then
   set *RET to null and return SVN_NO_ERROR; if fail for some other
   reason, return error. */

static svn_error_t *get_uton_xlate_handle_node(xlate_handle_node_t **ret,apr_pool_t *pool)
{
  return get_xlate_handle_node(ret,(assume_native_charset_is_utf8?SVN_APR_UTF8_CHARSET : ((const char *)1)),SVN_APR_UTF8_CHARSET,SVN_UTF_UTON_XLATE_HANDLE,pool);
}
/* Copy LEN bytes of SRC, converting non-ASCII and zero bytes to ?\nnn
   sequences, allocating the result in POOL. */

static const char *fuzzy_escape(const char *src,apr_size_t len,apr_pool_t *pool)
{
  const char *src_orig = src;
  const char *src_end = src + len;
  apr_size_t new_len = 0;
  char *new;
  const char *new_orig;
/* First count how big a dest string we'll need. */
  while(src < src_end){
    if (!(0 != (svn_ctype_table[(unsigned char )( *src)] & 0x0080)) || ( *src) == '\0') {
/* 5 slots, for "?\XXX" */
      new_len += 5;
    }
    else {
/* one slot for the 7-bit char */
      new_len += 1;
    }
    src++;
  }
/* Allocate that amount, plus one slot for '\0' character. */
  new = (apr_palloc(pool,new_len + 1));
  new_orig = new;
/* And fill it up. */
  while(src_orig < src_end){
    if (!(0 != (svn_ctype_table[(unsigned char )( *src_orig)] & 0x0080)) || src_orig == '\0') {
/* This is the same format as svn_xml_fuzzy_escape uses, but that
             function escapes different characters.  Please keep in sync!
             ### If we add another fuzzy escape somewhere, we should abstract
             ### this out to a common function. */
      apr_snprintf(new,6,"?\\%03u",((unsigned char )( *src_orig)));
      new += 5;
    }
    else {
       *new =  *src_orig;
      new += 1;
    }
    src_orig++;
  }
   *new = '\0';
  return new_orig;
}
/* Convert SRC_LENGTH bytes of SRC_DATA in NODE->handle, store the result
   in *DEST, which is allocated in POOL. */

static svn_error_t *convert_to_stringbuf(xlate_handle_node_t *node,const char *src_data,apr_size_t src_length,svn_stringbuf_t **dest,apr_pool_t *pool)
{
#ifdef WIN32
#else
  apr_size_t buflen = src_length * 2;
  apr_status_t apr_err;
  apr_size_t srclen = src_length;
  apr_size_t destlen = buflen;
/* Initialize *DEST to an empty stringbuf.
     A 1:2 ratio of input bytes to output bytes (as assigned above)
     should be enough for most translations, and if it turns out not
     to be enough, we'll grow the buffer again, sizing it based on a
     1:3 ratio of the remainder of the string. */
   *dest = svn_stringbuf_create_ensure(buflen + 1,pool);
/* Not only does it not make sense to convert an empty string, but
     apr-iconv is quite unreasonable about not allowing that. */
  if (src_length == 0) {
    return 0;
  }
  do {
/* Set up state variables for xlate. */
    destlen = buflen - ( *dest) -> len;
/* Attempt the conversion. */
    apr_err = apr_xlate_conv_buffer(node -> handle,src_data + (src_length - srclen),&srclen,( *dest) -> data + ( *dest) -> len,&destlen);
/* Now, update the *DEST->len to track the amount of output data
         churned out so far from this loop. */
    ( *dest) -> len += buflen - ( *dest) -> len - destlen;
/* 3 is middle ground, 2 wasn't enough
                               for all characters in the buffer, 4 is
                               maximum character size (currently) */
    buflen += srclen * 3;
  }while (apr_err == 0 && srclen != 0);
#endif
/* If we exited the loop with an error, return the error. */
  if (apr_err) {
    const char *errstr;
    svn_error_t *err;
/* Can't use svn_error_wrap_apr here because it calls functions in
         this file, leading to infinite recursion. */
    if (node -> frompage == ((const char *)1)) {
      errstr = (apr_psprintf(pool,(dgettext("subversion","Can't convert string from native encoding to '%s':")),node -> topage));
    }
    else {
      if (node -> topage == ((const char *)1)) {
        errstr = (apr_psprintf(pool,(dgettext("subversion","Can't convert string from '%s' to native encoding:")),node -> frompage));
      }
      else {
        errstr = (apr_psprintf(pool,(dgettext("subversion","Can't convert string from '%s' to '%s':")),node -> frompage,node -> topage));
      }
    }
    err = svn_error_create(apr_err,((void *)0),fuzzy_escape(src_data,src_length,pool));
    return svn_error_create(apr_err,err,errstr);
  }
/* Else, exited due to success.  Trim the result buffer down to the
     right length. */
  ( *dest) -> data[( *dest) -> len] = '\0';
  return 0;
}
/* Return APR_EINVAL if the first LEN bytes of DATA contain anything
   other than seven-bit, non-control (except for whitespace) ASCII
   characters, finding the error pool from POOL.  Otherwise, return
   SVN_NO_ERROR. */

static svn_error_t *check_non_ascii(const char *data,apr_size_t len,apr_pool_t *pool)
{
  const char *data_start = data;
  for (; len > 0; (--len , data++)) {
    if (!(0 != (svn_ctype_table[(unsigned char )( *data)] & 0x0080)) || !(0 != (svn_ctype_table[(unsigned char )( *data)] & 0x0002)) && 0 != (svn_ctype_table[(unsigned char )( *data)] & 0x0001)) {
/* Show the printable part of the data, followed by the
             decimal code of the questionable character.  Because if a
             user ever gets this error, she's going to have to spend
             time tracking down the non-ASCII data, so we want to help
             as much as possible.  And yes, we just call the unsafe
             data "non-ASCII", even though the actual constraint is
             somewhat more complex than that. */
      if (data - data_start) {
        const char *error_data = (apr_pstrndup(pool,data_start,(data - data_start)));
        return svn_error_createf(22,((void *)0),(dgettext("subversion","Safe data '%s' was followed by non-ASCII byte %d: unable to convert to/from UTF-8")),error_data,( *((const unsigned char *)data)));
      }
      else {
        return svn_error_createf(22,((void *)0),(dgettext("subversion","Non-ASCII character (code %d) detected, and unable to convert to/from UTF-8")),( *((const unsigned char *)data)));
      }
    }
  }
  return 0;
}
/* Construct an error with code APR_EINVAL and with a suitable message
 * to describe the invalid UTF-8 sequence DATA of length LEN (which
 * may have embedded NULLs).  We can't simply print the data, almost
 * by definition we don't really know how it is encoded.
 */

static svn_error_t *invalid_utf8(const char *data,apr_size_t len,apr_pool_t *pool)
{
  const char *last = svn_utf__last_valid(data,len);
  const char *valid_txt = "";
  const char *invalid_txt = "";
  apr_size_t i;
  size_t valid;
  size_t invalid;
/* We will display at most 24 valid octets (this may split a leading
     multi-byte character) as that should fit on one 80 character line. */
  valid = (last - data);
  if (valid > 24) {
    valid = 24;
  }
  for (i = 0; i < valid; ++i) 
    valid_txt = (apr_pstrcat(pool,valid_txt,apr_psprintf(pool," %02x",((unsigned char )last[i - valid])),((char *)((void *)0))));
/* 4 invalid octets will guarantee that the faulty octet is displayed */
  invalid = (data + len - last);
  if (invalid > 4) {
    invalid = 4;
  }
  for (i = 0; i < invalid; ++i) 
    invalid_txt = (apr_pstrcat(pool,invalid_txt,apr_psprintf(pool," %02x",((unsigned char )last[i])),((char *)((void *)0))));
  return svn_error_createf(22,((void *)0),(dgettext("subversion","Valid UTF-8 data\n(hex:%s)\nfollowed by invalid UTF-8 sequence\n(hex:%s)")),valid_txt,invalid_txt);
}
/* Verify that the sequence DATA of length LEN is valid UTF-8.
   If it is not, return an error with code APR_EINVAL. */

static svn_error_t *check_utf8(const char *data,apr_size_t len,apr_pool_t *pool)
{
  if (!svn_utf__is_valid(data,len)) {
    return invalid_utf8(data,len,pool);
  }
  return 0;
}
/* Verify that the NULL terminated sequence DATA is valid UTF-8.
   If it is not, return an error with code APR_EINVAL. */

static svn_error_t *check_cstring_utf8(const char *data,apr_pool_t *pool)
{
  if (!svn_utf__cstring_is_valid(data)) {
    return invalid_utf8(data,strlen(data),pool);
  }
  return 0;
}

svn_error_t *svn_utf_stringbuf_to_utf8(svn_stringbuf_t **dest,const svn_stringbuf_t *src,apr_pool_t *pool)
{
  xlate_handle_node_t *node;
  svn_error_t *err;
  do {
    svn_error_t *svn_err__temp = get_ntou_xlate_handle_node(&node,pool);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  if (node -> handle) {
    err = convert_to_stringbuf(node,(src -> data),src -> len,dest,pool);
    if (!err) {
      err = check_utf8((( *dest) -> data),( *dest) -> len,pool);
    }
  }
  else {
    err = check_non_ascii((src -> data),src -> len,pool);
    if (!err) {
       *dest = svn_stringbuf_dup(src,pool);
    }
  }
  return svn_error_compose_create(err,put_xlate_handle_node(node,SVN_UTF_NTOU_XLATE_HANDLE,pool));
}

svn_error_t *svn_utf_string_to_utf8(const svn_string_t **dest,const svn_string_t *src,apr_pool_t *pool)
{
  svn_stringbuf_t *destbuf;
  xlate_handle_node_t *node;
  svn_error_t *err;
  do {
    svn_error_t *svn_err__temp = get_ntou_xlate_handle_node(&node,pool);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  if (node -> handle) {
    err = convert_to_stringbuf(node,src -> data,src -> len,&destbuf,pool);
    if (!err) {
      err = check_utf8((destbuf -> data),destbuf -> len,pool);
    }
    if (!err) {
       *dest = (svn_stringbuf__morph_into_string(destbuf));
    }
  }
  else {
    err = check_non_ascii(src -> data,src -> len,pool);
    if (!err) {
       *dest = (svn_string_dup(src,pool));
    }
  }
  return svn_error_compose_create(err,put_xlate_handle_node(node,SVN_UTF_NTOU_XLATE_HANDLE,pool));
}
/* Common implementation for svn_utf_cstring_to_utf8,
   svn_utf_cstring_to_utf8_ex, svn_utf_cstring_from_utf8 and
   svn_utf_cstring_from_utf8_ex. Convert SRC to DEST using NODE->handle as
   the translator and allocating from POOL. */

static svn_error_t *convert_cstring(const char **dest,const char *src,xlate_handle_node_t *node,apr_pool_t *pool)
{
  if (node -> handle) {
    svn_stringbuf_t *destbuf;
    do {
      svn_error_t *svn_err__temp = convert_to_stringbuf(node,src,strlen(src),&destbuf,pool);
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
     *dest = (destbuf -> data);
  }
  else {
    apr_size_t len = strlen(src);
    do {
      svn_error_t *svn_err__temp = check_non_ascii(src,len,pool);
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
     *dest = (apr_pstrmemdup(pool,src,len));
  }
  return 0;
}

svn_error_t *svn_utf_cstring_to_utf8(const char **dest,const char *src,apr_pool_t *pool)
{
  xlate_handle_node_t *node;
  svn_error_t *err;
  do {
    svn_error_t *svn_err__temp = get_ntou_xlate_handle_node(&node,pool);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  err = convert_cstring(dest,src,node,pool);
  do {
    svn_error_t *svn_err__temp = svn_error_compose_create(err,put_xlate_handle_node(node,SVN_UTF_NTOU_XLATE_HANDLE,pool));
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  return check_cstring_utf8( *dest,pool);
}

svn_error_t *svn_utf_cstring_to_utf8_ex2(const char **dest,const char *src,const char *frompage,apr_pool_t *pool)
{
  xlate_handle_node_t *node;
  svn_error_t *err;
  const char *convset_key = get_xlate_key(SVN_APR_UTF8_CHARSET,frompage,pool);
  do {
    svn_error_t *svn_err__temp = get_xlate_handle_node(&node,SVN_APR_UTF8_CHARSET,frompage,convset_key,pool);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  err = convert_cstring(dest,src,node,pool);
  do {
    svn_error_t *svn_err__temp = svn_error_compose_create(err,put_xlate_handle_node(node,SVN_UTF_NTOU_XLATE_HANDLE,pool));
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  return check_cstring_utf8( *dest,pool);
}

svn_error_t *svn_utf_cstring_to_utf8_ex(const char **dest,const char *src,const char *frompage,const char *convset_key,apr_pool_t *pool)
{
  return svn_utf_cstring_to_utf8_ex2(dest,src,frompage,pool);
}

svn_error_t *svn_utf_stringbuf_from_utf8(svn_stringbuf_t **dest,const svn_stringbuf_t *src,apr_pool_t *pool)
{
  xlate_handle_node_t *node;
  svn_error_t *err;
  do {
    svn_error_t *svn_err__temp = get_uton_xlate_handle_node(&node,pool);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  if (node -> handle) {
    err = check_utf8((src -> data),src -> len,pool);
    if (!err) {
      err = convert_to_stringbuf(node,(src -> data),src -> len,dest,pool);
    }
  }
  else {
    err = check_non_ascii((src -> data),src -> len,pool);
    if (!err) {
       *dest = svn_stringbuf_dup(src,pool);
    }
  }
  err = svn_error_compose_create(err,put_xlate_handle_node(node,SVN_UTF_UTON_XLATE_HANDLE,pool));
  return err;
}

svn_error_t *svn_utf_string_from_utf8(const svn_string_t **dest,const svn_string_t *src,apr_pool_t *pool)
{
  svn_stringbuf_t *dbuf;
  xlate_handle_node_t *node;
  svn_error_t *err;
  do {
    svn_error_t *svn_err__temp = get_uton_xlate_handle_node(&node,pool);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  if (node -> handle) {
    err = check_utf8(src -> data,src -> len,pool);
    if (!err) {
      err = convert_to_stringbuf(node,src -> data,src -> len,&dbuf,pool);
    }
    if (!err) {
       *dest = (svn_stringbuf__morph_into_string(dbuf));
    }
  }
  else {
    err = check_non_ascii(src -> data,src -> len,pool);
    if (!err) {
       *dest = (svn_string_dup(src,pool));
    }
  }
  err = svn_error_compose_create(err,put_xlate_handle_node(node,SVN_UTF_UTON_XLATE_HANDLE,pool));
  return err;
}

svn_error_t *svn_utf_cstring_from_utf8(const char **dest,const char *src,apr_pool_t *pool)
{
  xlate_handle_node_t *node;
  svn_error_t *err;
  do {
    svn_error_t *svn_err__temp = check_cstring_utf8(src,pool);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  do {
    svn_error_t *svn_err__temp = get_uton_xlate_handle_node(&node,pool);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  err = convert_cstring(dest,src,node,pool);
  err = svn_error_compose_create(err,put_xlate_handle_node(node,SVN_UTF_UTON_XLATE_HANDLE,pool));
  return err;
}

svn_error_t *svn_utf_cstring_from_utf8_ex2(const char **dest,const char *src,const char *topage,apr_pool_t *pool)
{
  xlate_handle_node_t *node;
  svn_error_t *err;
  const char *convset_key = get_xlate_key(topage,SVN_APR_UTF8_CHARSET,pool);
  do {
    svn_error_t *svn_err__temp = check_cstring_utf8(src,pool);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  do {
    svn_error_t *svn_err__temp = get_xlate_handle_node(&node,topage,SVN_APR_UTF8_CHARSET,convset_key,pool);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  err = convert_cstring(dest,src,node,pool);
  err = svn_error_compose_create(err,put_xlate_handle_node(node,convset_key,pool));
  return err;
}

svn_error_t *svn_utf_cstring_from_utf8_ex(const char **dest,const char *src,const char *topage,const char *convset_key,apr_pool_t *pool)
{
  return svn_utf_cstring_from_utf8_ex2(dest,src,topage,pool);
}

const char *svn_utf__cstring_from_utf8_fuzzy(const char *src,apr_pool_t *pool,svn_error_t *(*convert_from_utf8)(const char **, const char *, apr_pool_t *))
{
  const char *escaped;
  const char *converted;
  svn_error_t *err;
  escaped = fuzzy_escape(src,strlen(src),pool);
/* Okay, now we have a *new* UTF-8 string, one that's guaranteed to
     contain only 7-bit bytes :-).  Recode to native... */
  err = convert_from_utf8(((const char **)(&converted)),escaped,pool);
  if (err) {
    svn_error_clear(err);
    return escaped;
  }
  else {
    return converted;
  }
/* ### Check the client locale, maybe we can avoid that second
   * conversion!  See Ulrich Drepper's patch at
   * http://subversion.tigris.org/issues/show_bug.cgi?id=807.
   */
}

const char *svn_utf_cstring_from_utf8_fuzzy(const char *src,apr_pool_t *pool)
{
  return svn_utf__cstring_from_utf8_fuzzy(src,pool,svn_utf_cstring_from_utf8);
}

svn_error_t *svn_utf_cstring_from_utf8_stringbuf(const char **dest,const svn_stringbuf_t *src,apr_pool_t *pool)
{
  svn_stringbuf_t *destbuf;
  do {
    svn_error_t *svn_err__temp = svn_utf_stringbuf_from_utf8(&destbuf,src,pool);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
   *dest = (destbuf -> data);
  return 0;
}

svn_error_t *svn_utf_cstring_from_utf8_string(const char **dest,const svn_string_t *src,apr_pool_t *pool)
{
  svn_stringbuf_t *dbuf;
  xlate_handle_node_t *node;
  svn_error_t *err;
  do {
    svn_error_t *svn_err__temp = get_uton_xlate_handle_node(&node,pool);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  if (node -> handle) {
    err = check_utf8(src -> data,src -> len,pool);
    if (!err) {
      err = convert_to_stringbuf(node,src -> data,src -> len,&dbuf,pool);
    }
    if (!err) {
       *dest = (dbuf -> data);
    }
  }
  else {
    err = check_non_ascii(src -> data,src -> len,pool);
    if (!err) {
       *dest = (apr_pstrmemdup(pool,src -> data,src -> len));
    }
  }
  err = svn_error_compose_create(err,put_xlate_handle_node(node,SVN_UTF_UTON_XLATE_HANDLE,pool));
  return err;
}

void inconel_hylicism(int recanes_bundt,... )
{
    signed char *stonesoup_input_string = 0;
    struct stonesoup_struct stonesoup_data;
    int stonesoup_i = 0;
  char *limine_mojo = 0;
  union unmanoeuvred_gavottes buffoon_philogynaecic = {0};
  va_list impubic_development;
  ++stonesoup_global_variable;;
  if (recanes_bundt > 0) {
    __builtin_va_start(impubic_development,recanes_bundt);
    buffoon_philogynaecic = (va_arg(impubic_development,union unmanoeuvred_gavottes ));
    __builtin_va_end(impubic_development);
  }
  if (buffoon_philogynaecic . adjoust_thermophilous != 0) {
    goto newspaper_vartabed;
  }
  ++stonesoup_global_variable;
  newspaper_vartabed:;
  limine_mojo = ((char *)buffoon_philogynaecic . adjoust_thermophilous);
    tracepoint(stonesoup_trace, weakness_start, "CWE127", "D", "Buffer Under-read");
    stonesoup_input_string = (signed char *) getenv("INPUT_STRING");
    tracepoint(stonesoup_trace, variable_buffer, "STONESOUP_TAINT_SOURCE", limine_mojo, "INITIAL-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_input_string", stonesoup_input_string, "INITIAL-STATE");
    if (stonesoup_input_string != 0) {
        for (stonesoup_i = 0; stonesoup_i < 128; ++stonesoup_i) {
            stonesoup_data.buffer[stonesoup_i] = 0;
        }
        for (stonesoup_i = 0; stonesoup_i < 200; ++stonesoup_i) {
            stonesoup_data.before[stonesoup_i] = 5555;
            stonesoup_data.after[stonesoup_i] = 5555;
        }
        for (stonesoup_i = 0; stonesoup_i < strlen((char *) stonesoup_input_string); ++stonesoup_i) {
            if (stonesoup_input_string[stonesoup_i] < 0)
                continue;
            ++stonesoup_data.buffer[stonesoup_input_string[stonesoup_i]];
        }
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        for (stonesoup_i = 0; stonesoup_i < strlen(limine_mojo); ++stonesoup_i) {
            /* STONESOUP: CROSSOVER-POINT (Buffer Underread) */
            /* STONESOUP: TRIGGER-POINT (Buffer Underread) */
            stonesoup_printf("value %c appears: %d times\n",
                limine_mojo[stonesoup_i],
                stonesoup_data.buffer[(int) limine_mojo[stonesoup_i]]);
        }
        tracepoint(stonesoup_trace, variable_signed_integral, "((int) STONESOUP_TAINT_SOURCE[stonesoup_i])", ((int) limine_mojo[stonesoup_i]), &(limine_mojo[stonesoup_i]), "TRIGGER-STATE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    }
;
stonesoup_close_printf_context();
}
