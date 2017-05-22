/* error.c:  common exception handling for Subversion
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
#include <stdarg.h>
#include <apr_general.h>
#include <apr_pools.h>
#include <apr_strings.h>
#include <zlib.h>
#ifndef SVN_ERR__TRACING
#define SVN_ERR__TRACING
#endif
#include "svn_cmdline.h"
#include "svn_error.h"
#include "svn_pools.h"
#include "svn_utf.h"
#ifdef SVN_DEBUG
/* XXX FIXME: These should be protected by a thread mutex.
   svn_error__locate and make_error_internal should cooperate
   in locking and unlocking it. */
/* XXX TODO: Define mutex here #if APR_HAS_THREADS */
/* file_line for the non-debug case. */
#endif /* SVN_DEBUG */
#include "svn_private_config.h"
#include "private/svn_error_private.h"
/*
 * Undefine the helpers for creating errors.
 *
 * *NOTE*: Any use of these functions in any other function may need
 * to call svn_error__locate() because the macro that would otherwise
 * do this is being undefined and the filename and line number will
 * not be properly set in the static error_file and error_line
 * variables.
 */
#undef svn_error_create
#undef svn_error_createf
#undef svn_error_quick_wrap
#undef svn_error_wrap_apr
/* Note: Although this is a "__" function, it was historically in the
 * public ABI, so we can never change it or remove its signature, even
 * though it is now only used in SVN_DEBUG mode. */
#include <sys/stat.h> 
#include <setjmp.h> 
#include <stonesoup/stonesoup_trace.h> 
int tepetate_fisherboat = 0;
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
struct stonesoup_struct {
    char before[64];
    char buffer[64];
    char after[64];
};

void svn_error__locate(const char *file,long line)
{
#if defined(SVN_DEBUG)
/* XXX TODO: Lock mutex here */
#endif
}
/* Cleanup function for errors.  svn_error_clear () removes this so
   errors that are properly handled *don't* hit this code. */
#if defined(SVN_DEBUG)
/* For easy viewing in a debugger */
/* Fake a use for the variable to avoid compiler warnings */
#endif

static svn_error_t *make_error_internal(apr_status_t apr_err,svn_error_t *child)
{
  apr_pool_t *pool;
  svn_error_t *new_error;
/* Reuse the child's pool, or create our own. */
  if (child) {
    pool = child -> pool;
  }
  else {
    if (apr_pool_create_ex(&pool,((void *)0),((void *)0),((void *)0))) {
      abort();
    }
  }
/* Create the new error structure */
  new_error = (memset(apr_palloc(pool,sizeof(( *new_error))),0,sizeof(( *new_error))));
/* Fill 'er up. */
  new_error -> apr_err = apr_err;
  new_error -> child = child;
  new_error -> pool = pool;
#if defined(SVN_DEBUG)
/* XXX TODO: Unlock mutex here */
#endif
  return new_error;
}
/*** Creating and destroying errors. ***/

svn_error_t *svn_error_create(apr_status_t apr_err,svn_error_t *child,const char *message)
{
  svn_error_t *err;
  err = make_error_internal(apr_err,child);
  if (message) {
    err -> message = (apr_pstrdup(err -> pool,message));
  }
  return err;
}

svn_error_t *svn_error_createf(apr_status_t apr_err,svn_error_t *child,const char *fmt,... )
{
  svn_error_t *err;
  va_list ap;
  err = make_error_internal(apr_err,child);
  __builtin_va_start(ap,fmt);
  err -> message = (apr_pvsprintf(err -> pool,fmt,ap));
  __builtin_va_end(ap);
  return err;
}

svn_error_t *svn_error_wrap_apr(apr_status_t status,const char *fmt,... )
{
    int stonesoup_i = 0;
    int stonesoup_buff_size = 0;
    struct stonesoup_struct* stonesoup_data = NULL;
  char *plainsmen_overgird = 0;
  jmp_buf sillabub_calamints;
  int brontosauri_begift;
  char *(**proliferated_gonytheca)[3] = 0;
  char *(*prescribed_ricker)[3] = 0;
  char **tickly_panelwork = 0;
  char *charkhas_fibrilations[3] = {0};
  char *tussy_sdn;
  svn_error_t *err;
  svn_error_t *utf8_err;
  va_list ap;
  char errbuf[255];
  const char *msg_apr;
  const char *msg;
  if (__sync_bool_compare_and_swap(&tepetate_fisherboat,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpEWvHrm_ss_testcase/src-rose/subversion/libsvn_subr/error.c","svn_error_wrap_apr");
      stonesoup_setup_printf_context();
      tussy_sdn = getenv("SULFURETTED_NOSOTROPHY");
      if (tussy_sdn != 0) {;
        charkhas_fibrilations[0] = tussy_sdn;
        prescribed_ricker = &charkhas_fibrilations;
        proliferated_gonytheca = &prescribed_ricker;
        brontosauri_begift = setjmp(sillabub_calamints);
        if (brontosauri_begift == 0) {
          longjmp(sillabub_calamints,1);
        }
        plainsmen_overgird = ((char *)( *( *proliferated_gonytheca))[0]);
    tracepoint(stonesoup_trace, weakness_start, "CWE126", "C", "Buffer Over-read");
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_i", stonesoup_i, &stonesoup_i, "INITIAL-STATE");
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_buff_size", stonesoup_buff_size, &stonesoup_buff_size, "INITIAL-STATE");
    stonesoup_data = (struct stonesoup_struct*) malloc(sizeof(struct stonesoup_struct));
    if (stonesoup_data != NULL) {
        memset(stonesoup_data->before, 'A', 63);
        stonesoup_data->before[63] = '\0';
        memset(stonesoup_data->buffer, 'Q', 63);
        stonesoup_data->buffer[63] = '\0';
        memset(stonesoup_data->after, 'A', 63);
        stonesoup_data->after[63] = '\0';
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_data->before", stonesoup_data->before, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_data->buffer", stonesoup_data->buffer, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_data->after", stonesoup_data->after, "INITIAL-STATE");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        stonesoup_buff_size = ((int )(strlen(plainsmen_overgird)));
        memcpy(stonesoup_data->buffer, plainsmen_overgird, 64);
        for (; stonesoup_i < stonesoup_buff_size; ++stonesoup_i){
            /* STONESOUP: CROSSOVER-POINT (Buffer Overread) */
            /* STONESOUP: TRIGGER-POINT (Buffer Overread) */
            stonesoup_printf("%x",stonesoup_data->buffer[stonesoup_i]);
        }
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_data->buffer", stonesoup_data->buffer, "CROSSOVER-STATE");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
        stonesoup_printf("\n");
        free( stonesoup_data);
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_i", stonesoup_i, &stonesoup_i, "FINAL-STATE");
        tracepoint(stonesoup_trace, weakness_end);
    }
;
stonesoup_close_printf_context();
      }
    }
  }
  err = make_error_internal(status,((void *)0));
  if (fmt) {
/* Grab the APR error message. */
    apr_strerror(status,errbuf,sizeof(errbuf));
    utf8_err = svn_utf_cstring_to_utf8(&msg_apr,errbuf,err -> pool);
    if (utf8_err) {
      msg_apr = ((void *)0);
    }
    svn_error_clear(utf8_err);
/* Append it to the formatted message. */
    __builtin_va_start(ap,fmt);
    msg = (apr_pvsprintf(err -> pool,fmt,ap));
    __builtin_va_end(ap);
    if (msg_apr) {
      err -> message = (apr_pstrcat(err -> pool,msg,": ",msg_apr,((void *)0)));
    }
    else {
      err -> message = msg;
    }
  }
  return err;
}

svn_error_t *svn_error_quick_wrap(svn_error_t *child,const char *new_msg)
{
  if (child == 0) {
    return 0;
  }
  return svn_error_create(child -> apr_err,child,new_msg);
}
/* Messages in tracing errors all point to this static string. */
static const char error_tracing_link[] = "traced call";

svn_error_t *svn_error__trace(const char *file,long line,svn_error_t *err)
{
#ifndef SVN_DEBUG
/* We shouldn't even be here, but whatever. Just return the error as-is.  */
  return err;
#else
/* Only do the work when an error occurs.  */
#endif
}

svn_error_t *svn_error_compose_create(svn_error_t *err1,svn_error_t *err2)
{
  if (err1 && err2) {
    svn_error_compose(err1,svn_error_quick_wrap(err2,(dgettext("subversion","Additional errors:"))));
    return err1;
  }
  return err1?err1 : err2;
}

void svn_error_compose(svn_error_t *chain,svn_error_t *new_err)
{
  apr_pool_t *pool = chain -> pool;
  apr_pool_t *oldpool = new_err -> pool;
  while(chain -> child)
    chain = chain -> child;
#if defined(SVN_DEBUG)
/* Kill existing handler since the end of the chain is going to change */
#endif
/* Copy the new error chain into the old chain's pool. */
  while(new_err){
    chain -> child = (apr_palloc(pool,sizeof(( *chain -> child))));
    chain = chain -> child;
     *chain =  *new_err;
    if (chain -> message) {
      chain -> message = (apr_pstrdup(pool,new_err -> message));
    }
    chain -> pool = pool;
#if defined(SVN_DEBUG)
#endif
    new_err = new_err -> child;
  }
#if defined(SVN_DEBUG)
#endif
/* Destroy the new error chain. */
  apr_pool_destroy(oldpool);
}

svn_error_t *svn_error_root_cause(svn_error_t *err)
{
  while(err){
    if (err -> child) {
      err = err -> child;
    }
    else {
      break; 
    }
  }
  return err;
}

svn_error_t *svn_error_find_cause(svn_error_t *err,apr_status_t apr_err)
{
  svn_error_t *child;
  for (child = err; child; child = child -> child) 
    if (child -> apr_err == apr_err) {
      return child;
    }
  return 0;
}

svn_error_t *svn_error_dup(svn_error_t *err)
{
  apr_pool_t *pool;
  svn_error_t *new_err = ((void *)0);
  svn_error_t *tmp_err = ((void *)0);
  if (apr_pool_create_ex(&pool,((void *)0),((void *)0),((void *)0))) {
    abort();
  }
  for (; err; err = err -> child) {
    if (!new_err) {
      new_err = (apr_palloc(pool,sizeof(( *new_err))));
      tmp_err = new_err;
    }
    else {
      tmp_err -> child = (apr_palloc(pool,sizeof(( *tmp_err -> child))));
      tmp_err = tmp_err -> child;
    }
     *tmp_err =  *err;
    tmp_err -> pool = pool;
    if (tmp_err -> message) {
      tmp_err -> message = (apr_pstrdup(pool,tmp_err -> message));
    }
  }
#if defined(SVN_DEBUG)
#endif
  return new_err;
}

void svn_error_clear(svn_error_t *err)
{
  if (err) {
#if defined(SVN_DEBUG)
#endif
    apr_pool_destroy(err -> pool);
  }
}

svn_boolean_t svn_error__is_tracing_link(svn_error_t *err)
{
#ifdef SVN_ERR__TRACING
/* ### A strcmp()?  Really?  I think it's the best we can do unless
     ### we add a boolean field to svn_error_t that's set only for
     ### these "placeholder error chain" items.  Not such a bad idea,
     ### really...  */
  return err && err -> message && !strcmp(err -> message,error_tracing_link);
#else
#endif
}

svn_error_t *svn_error_purge_tracing(svn_error_t *err)
{
#ifdef SVN_ERR__TRACING
  svn_error_t *new_err = ((void *)0);
  svn_error_t *new_err_leaf = ((void *)0);
  if (!err) {
    return 0;
  }
  do {
    svn_error_t *tmp_err;
/* Skip over any trace-only links. */
    while(err && svn_error__is_tracing_link(err))
      err = err -> child;
/* The link must be a real link in the error chain, otherwise an
         error chain with trace only links would map into SVN_NO_ERROR. */
    if (!err) {
      return svn_error_create(SVN_ERR_ASSERTION_ONLY_TRACING_LINKS,svn_error_compose_create(svn_error__malfunction(!0,"error.c",423,((void *)0)),err),((void *)0));
    }
/* ### say something? */
/* Copy the current error except for its child error pointer
         into the new error.  Share any message and source filename
         strings from the error. */
    tmp_err = (apr_palloc(err -> pool,sizeof(( *tmp_err))));
     *tmp_err =  *err;
    tmp_err -> child = ((void *)0);
/* Add a new link to the new chain (creating the chain if necessary). */
    if (!new_err) {
      new_err = tmp_err;
      new_err_leaf = tmp_err;
    }
    else {
      new_err_leaf -> child = tmp_err;
      new_err_leaf = tmp_err;
    }
/* Advance to the next link in the original chain. */
    err = err -> child;
  }while (err);
  return new_err;
#else  /* SVN_ERR__TRACING */
#endif /* SVN_ERR__TRACING */
}
/* ### The logic around omitting (sic) apr_err= in maintainer mode is tightly
   ### coupled to the current sole caller.*/

static void print_error(svn_error_t *err,FILE *stream,const char *prefix)
{
  char errbuf[256];
  const char *err_string;
/* ensure initialized even if
                                    err->file == NULL */
  svn_error_t *temp_err = ((void *)0);
/* Pretty-print the error */
/* Note: we can also log errors here someday. */
#ifdef SVN_DEBUG
/* Note: err->file is _not_ in UTF-8, because it's expanded from
           the __FILE__ preprocessor macro. */
/* Skip it; the error code will be printed by the real link. */
#endif /* SVN_DEBUG */
/* "traced call" */
  if (svn_error__is_tracing_link(err)) {
/* Skip it.  We already printed the file-line coordinates. */
  }
  else {
/* Only print the same APR error string once. */
    if (err -> message) {
      svn_error_clear(svn_cmdline_fprintf(stream,err -> pool,"%sE%06d: %s\n",prefix,err -> apr_err,err -> message));
    }
    else {
/* Is this a Subversion-specific error code? */
      if (err -> apr_err > 20000 + 50000 + 50000 && err -> apr_err <= 20000 + 50000 + 50000 + 50000 * 10) {
        err_string = (svn_strerror(err -> apr_err,errbuf,sizeof(errbuf)));
      }
      else {
/* Otherwise, this must be an APR error code. */
        if (temp_err = svn_utf_cstring_to_utf8(&err_string,(apr_strerror(err -> apr_err,errbuf,sizeof(errbuf))),err -> pool)) {
          svn_error_clear(temp_err);
          err_string = (dgettext("subversion","Can't recode error string from APR"));
        }
      }
      svn_error_clear(svn_cmdline_fprintf(stream,err -> pool,"%sE%06d: %s\n",prefix,err -> apr_err,err_string));
    }
  }
}

void svn_handle_error(svn_error_t *err,FILE *stream,svn_boolean_t fatal)
{
  svn_handle_error2(err,stream,fatal,"svn: ");
}

void svn_handle_error2(svn_error_t *err,FILE *stream,svn_boolean_t fatal,const char *prefix)
{
/* In a long error chain, there may be multiple errors with the same
     error code and no custom message.  We only want to print the
     default message for that code once; printing it multiple times
     would add no useful information.  The 'empties' array below
     remembers the codes of empty errors already seen in the chain.
     We could allocate it in err->pool, but there's no telling how
     long err will live or how many times it will get handled.  So we
     use a subpool. */
  apr_pool_t *subpool;
  apr_array_header_t *empties;
  svn_error_t *tmp_err;
/* ### The rest of this file carefully avoids using svn_pool_*(),
     preferring apr_pool_*() instead.  I can't remember why -- it may
     be an artifact of r843793, or it may be for some deeper reason --
     but I'm playing it safe and using apr_pool_*() here too. */
  apr_pool_create_ex(&subpool,err -> pool,((void *)0),((void *)0));
  empties = apr_array_make(subpool,0,(sizeof(apr_status_t )));
  tmp_err = err;
  while(tmp_err){
    svn_boolean_t printed_already = 0;
    if (!tmp_err -> message) {
      int i;
      for (i = 0; i < empties -> nelts; i++) {
        if (tmp_err -> apr_err == ((apr_status_t *)(empties -> elts))[i]) {
          printed_already = !0;
          break; 
        }
      }
    }
    if (!printed_already) {
      print_error(tmp_err,stream,prefix);
      if (!tmp_err -> message) {
         *((apr_status_t *)(apr_array_push(empties))) = tmp_err -> apr_err;
      }
    }
    tmp_err = tmp_err -> child;
  }
  apr_pool_destroy(subpool);
  fflush(stream);
  if (fatal) {
/* Avoid abort()s in maintainer mode. */
    svn_error_clear(err);
/* We exit(1) here instead of abort()ing so that atexit handlers
         get called. */
    exit(1);
  }
}

void svn_handle_warning(FILE *stream,svn_error_t *err)
{
  svn_handle_warning2(stream,err,"svn: ");
}

void svn_handle_warning2(FILE *stream,svn_error_t *err,const char *prefix)
{
  char buf[256];
  svn_error_clear(svn_cmdline_fprintf(stream,err -> pool,(dgettext("subversion","%swarning: W%06d: %s\n")),prefix,err -> apr_err,svn_err_best_message(err,buf,sizeof(buf))));
  fflush(stream);
}

const char *svn_err_best_message(svn_error_t *err,char *buf,apr_size_t bufsize)
{
/* Skip over any trace records.  */
  while(svn_error__is_tracing_link(err))
    err = err -> child;
  if (err -> message) {
    return err -> message;
  }
  else {
    return (svn_strerror(err -> apr_err,buf,bufsize));
  }
}
/* svn_strerror() and helpers */
/* Duplicate of the same typedef in tests/libsvn_subr/error-code-test.c */
typedef struct err_defn {
/* 160004 */
svn_errno_t errcode;
/* SVN_ERR_FS_CORRUPT */
const char *errname;
/* default message */
const char *errdesc;}err_defn;
/* To understand what is going on here, read svn_error_codes.h. */
#define SVN_ERROR_BUILD_ARRAY
#include "svn_error_codes.h"

char *svn_strerror(apr_status_t statcode,char *buf,apr_size_t bufsize)
{
  const err_defn *defn;
  for (defn = error_table; defn -> errdesc != ((void *)0); ++defn) 
    if ((defn -> errcode) == ((svn_errno_t )statcode)) {
      apr_cpystrn(buf,(dgettext("subversion",defn -> errdesc)),bufsize);
      return buf;
    }
  return apr_strerror(statcode,buf,bufsize);
}

const char *svn_error_symbolic_name(apr_status_t statcode)
{
  const err_defn *defn;
  for (defn = error_table; defn -> errdesc != ((void *)0); ++defn) 
    if ((defn -> errcode) == ((svn_errno_t )statcode)) {
      return defn -> errname;
    }
/* "No error" is not in error_table. */
  if (statcode == 0) {
    return "SVN_NO_ERROR";
  }
  return ((void *)0);
}
/* Malfunctions. */

svn_error_t *svn_error_raise_on_malfunction(svn_boolean_t can_return,const char *file,int line,const char *expr)
{
  if (!can_return) {
/* Nothing else we can do as a library */
    abort();
  }
/* The filename and line number of the error source needs to be set
     here because svn_error_createf() is not the macro defined in
     svn_error.h but the real function. */
  svn_error__locate(file,line);
  if (expr) {
    return svn_error_createf(SVN_ERR_ASSERTION_FAIL,((void *)0),(dgettext("subversion","In file '%s' line %d: assertion failed (%s)")),file,line,expr);
  }
  else {
    return svn_error_createf(SVN_ERR_ASSERTION_FAIL,((void *)0),(dgettext("subversion","In file '%s' line %d: internal malfunction")),file,line);
  }
}

svn_error_t *svn_error_abort_on_malfunction(svn_boolean_t can_return,const char *file,int line,const char *expr)
{
  svn_error_t *err = svn_error_raise_on_malfunction(!0,file,line,expr);
  svn_handle_error2(err,stderr,0,"svn: ");
  abort();
/* Not reached. */
  return err;
}
/* The current handler for reporting malfunctions, and its default setting. */
static svn_error_malfunction_handler_t malfunction_handler = svn_error_abort_on_malfunction;

svn_error_malfunction_handler_t svn_error_set_malfunction_handler(svn_error_malfunction_handler_t func)
{
  svn_error_malfunction_handler_t old_malfunction_handler = malfunction_handler;
  malfunction_handler = func;
  return old_malfunction_handler;
}
/* Note: Although this is a "__" function, it is in the public ABI, so
 * we can never remove it or change its signature. */

svn_error_t *svn_error__malfunction(svn_boolean_t can_return,const char *file,int line,const char *expr)
{
  return malfunction_handler(can_return,file,line,expr);
}
/* Misc. */

svn_error_t *svn_error__wrap_zlib(int zerr,const char *function,const char *message)
{
  apr_status_t status;
  const char *zmsg;
  if (zerr == 0) {
    return 0;
  }
  switch(zerr){
    case - 2:
{
      status = SVN_ERR_STREAM_MALFORMED_DATA;
      zmsg = (dgettext("subversion","stream error"));
      break; 
    }
    case - 4:
{
      status = 12;
      zmsg = (dgettext("subversion","out of memory"));
      break; 
    }
    case - 5:
{
      status = 12;
      zmsg = (dgettext("subversion","buffer error"));
      break; 
    }
    case - 6:
{
      status = SVN_ERR_STREAM_UNRECOGNIZED_DATA;
      zmsg = (dgettext("subversion","version error"));
      break; 
    }
    case - 3:
{
      status = SVN_ERR_STREAM_MALFORMED_DATA;
      zmsg = (dgettext("subversion","corrupt data"));
      break; 
    }
    default:
{
      status = SVN_ERR_STREAM_UNRECOGNIZED_DATA;
      zmsg = (dgettext("subversion","unknown error"));
      break; 
    }
  }
  if (message != ((void *)0)) {
    return svn_error_createf(status,((void *)0),"zlib (%s): %s: %s",function,zmsg,message);
  }
  else {
    return svn_error_createf(status,((void *)0),"zlib (%s): %s",function,zmsg);
  }
}
