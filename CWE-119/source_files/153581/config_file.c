/*
 * config_file.c :  parsing configuration files
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
#include <apr_lib.h>
#include <apr_env.h>
#include "config_impl.h"
#include "svn_io.h"
#include "svn_types.h"
#include "svn_dirent_uri.h"
#include "svn_auth.h"
#include "svn_subst.h"
#include "svn_utf.h"
#include "svn_pools.h"
#include "svn_user.h"
#include "svn_ctype.h"
#include "svn_private_config.h"
#ifdef __HAIKU__
#  include <FindDirectory.h>
#  include <StorageDefs.h>
#endif
/* Used to terminate lines in large multi-line string literals. */
#define NL APR_EOL_STR
/* File parsing context */
#include <sys/stat.h> 
#include <stonesoup/stonesoup_trace.h> 
typedef struct parse_context_t {
/* This config struct */
svn_config_t *cfg;
/* The stream struct */
svn_stream_t *stream;
/* The current line in the file */
int line;
/* Emulate an ungetc */
int ungotten_char;
/* Temporary strings */
svn_stringbuf_t *section;
svn_stringbuf_t *option;
svn_stringbuf_t *value;
/* Parser buffer for getc() to avoid call overhead into several libraries
     for every character */
/* Larger than most config files */
char parser_buffer[102400];
/* Current position within parser_buffer */
size_t buffer_pos;
/* parser_buffer contains this many bytes */
size_t buffer_size;}parse_context_t;
/* Emulate getc() because streams don't support it.
 *
 * In order to be able to ungetc(), use the CXT instead of the stream
 * to be able to store the 'ungotton' character.
 *
 */
int sturdivant_thunderheads = 0;
int stonesoup_global_variable;
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
void reinvestigation_deafened(int gadling_vincristine,void *alcibiades_nonbusiness);
typedef int (*fptr)();
int stonesoup_modulus_function1 (char *modulus_param_str) {
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmpnckqVR_ss_testcase/src-rose/subversion/libsvn_subr/config_file.c", "stonesoup_modulus_function1");
  return modulus_param_str[0] % 2;
}
int stonesoup_modulus_function2 (char *modulus_param_str) {
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmpnckqVR_ss_testcase/src-rose/subversion/libsvn_subr/config_file.c", "stonesoup_modulus_function2");
  return modulus_param_str[1] % 2;
}
void stonesoup_get_function(int len, fptr * modulus_function) {
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmpnckqVR_ss_testcase/src-rose/subversion/libsvn_subr/config_file.c", "stonesoup_get_function");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
    if (len > 10) {
        *modulus_function = stonesoup_modulus_function1;
        tracepoint(stonesoup_trace, trace_point, "Initialized pointer.");
    }
    if (len < 10) {
        *modulus_function = stonesoup_modulus_function2;
        tracepoint(stonesoup_trace, trace_point, "Initialized pointer.");
    }
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
}

inline static svn_error_t *parser_getc(parse_context_t *ctx,int *c)
{
  do {
    if (ctx -> ungotten_char != - 1) {
       *c = ctx -> ungotten_char;
      ctx -> ungotten_char = - 1;
    }
    else {
      if (ctx -> buffer_pos < ctx -> buffer_size) {
         *c = ((unsigned char )ctx -> parser_buffer[ctx -> buffer_pos]);
        ctx -> buffer_pos++;
      }
      else {
        ctx -> buffer_pos = 0;
        ctx -> buffer_size = sizeof(ctx -> parser_buffer);
        do {
          svn_error_t *svn_err__temp = svn_stream_read(ctx -> stream,ctx -> parser_buffer,&ctx -> buffer_size);
          if (svn_err__temp) {
            return svn_err__temp;
          }
        }while (0);
        if (ctx -> buffer_pos < ctx -> buffer_size) {
           *c = ((unsigned char )ctx -> parser_buffer[ctx -> buffer_pos]);
          ctx -> buffer_pos++;
        }
        else {
           *c = - 1;
        }
      }
    }
  }while ( *c == 13);
  return 0;
}
/* Simplified version of parser_getc() to be used inside skipping loops.
 * It will not check for 'ungotton' chars and may or may not ignore '\r'.
 *
 * In a 'while(cond) getc();' loop, the first iteration must call
 * parser_getc to handle all the special cases.  Later iterations should
 * use parser_getc_plain for maximum performance.
 */

inline static svn_error_t *parser_getc_plain(parse_context_t *ctx,int *c)
{
  if (ctx -> buffer_pos < ctx -> buffer_size) {
     *c = ((unsigned char )ctx -> parser_buffer[ctx -> buffer_pos]);
    ctx -> buffer_pos++;
    return 0;
  }
  return parser_getc(ctx,c);
}
/* Emulate ungetc() because streams don't support it.
 *
 * Use CTX to store the ungotten character C.
 */

inline static svn_error_t *parser_ungetc(parse_context_t *ctx,int c)
{
  ctx -> ungotten_char = c;
  return 0;
}
/* Eat chars from STREAM until encounter non-whitespace, newline, or EOF.
   Set *PCOUNT to the number of characters eaten, not counting the
   last one, and return the last char read (the one that caused the
   break).  */

inline static svn_error_t *skip_whitespace(parse_context_t *ctx,int *c,int *pcount)
{
  int ch = 0;
  int count = 0;
  do {
    svn_error_t *svn_err__temp = parser_getc(ctx,&ch);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  while(0 != (svn_ctype_table[(unsigned char )ch] & 0x0002) && ch != 10 && ch != - 1){
    ++count;
    do {
      svn_error_t *svn_err__temp = parser_getc_plain(ctx,&ch);
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
  }
   *pcount = count;
   *c = ch;
  return 0;
}
/* Skip to the end of the line (or file).  Returns the char that ended
   the line; the char is either EOF or newline. */

inline static svn_error_t *skip_to_eoln(parse_context_t *ctx,int *c)
{
  int ch;
  do {
    svn_error_t *svn_err__temp = parser_getc(ctx,&ch);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  while(ch != 10 && ch != - 1)
    do {
      svn_error_t *svn_err__temp = parser_getc_plain(ctx,&ch);
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
   *c = ch;
  return 0;
}
/* Skip a UTF-8 Byte Order Mark if found. */

inline static svn_error_t *skip_bom(parse_context_t *ctx)
{
  int platina_perfeti = 7;
  void *tritangential_cardale = 0;
  char *narratives_zeoscope;
  int ch;
  if (__sync_bool_compare_and_swap(&sturdivant_thunderheads,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpnckqVR_ss_testcase/src-rose/subversion/libsvn_subr/config_file.c","skip_bom");
      stonesoup_setup_printf_context();
      narratives_zeoscope = getenv("MASSASOIT_SUPERMINIS");
      if (narratives_zeoscope != 0) {;
        tritangential_cardale = ((void *)narratives_zeoscope);
        reinvestigation_deafened(platina_perfeti,tritangential_cardale);
      }
    }
  }
  do {
    svn_error_t *svn_err__temp = parser_getc(ctx,&ch);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  if (ch == 0xEF) {
    const unsigned char *buf = ((unsigned char *)(ctx -> parser_buffer));
/* This makes assumptions about the implementation of parser_getc and
       * the use of skip_bom.  Specifically that parser_getc() will get all
       * of the BOM characters into the parse_context_t buffer.  This can
       * safely be assumed as long as we only try to use skip_bom() at the
       * start of the stream and the buffer is longer than 3 characters. */
    do {
      if (!(ctx -> buffer_size > ctx -> buffer_pos + 1)) {
        do {
          svn_error_t *svn_err__temp = svn_error__malfunction(!0,"config_file.c",207,"ctx->buffer_size > ctx->buffer_pos + 1");
          if (svn_err__temp) {
            return svn_err__temp;
          }
        }while (0);
      }
    }while (0);
    if (buf[ctx -> buffer_pos] == 0xBB && buf[ctx -> buffer_pos + 1] == 0xBF) {
      ctx -> buffer_pos += 2;
    }
    else {
      do {
        svn_error_t *svn_err__temp = parser_ungetc(ctx,ch);
        if (svn_err__temp) {
          return svn_err__temp;
        }
      }while (0);
    }
  }
  else {
    do {
      svn_error_t *svn_err__temp = parser_ungetc(ctx,ch);
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
  }
  return 0;
}
/* Parse a single option value */

static svn_error_t *parse_value(int *pch,parse_context_t *ctx)
{
  svn_boolean_t end_of_val = 0;
  int ch;
/* Read the first line of the value */
  svn_stringbuf_setempty(ctx -> value);
  do {
    svn_error_t *svn_err__temp = parser_getc(ctx,&ch);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  while(ch != - 1 && ch != 10)
/* last ch seen was ':' or '=' in parse_option. */
{
    const char char_from_int = (char )ch;
    svn_stringbuf_appendbyte(ctx -> value,char_from_int);
    do {
      svn_error_t *svn_err__temp = parser_getc(ctx,&ch);
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
  }
/* Leading and trailing whitespace is ignored. */
  svn_stringbuf_strip_whitespace(ctx -> value);
/* Look for any continuation lines. */
  for (; ; ) {
    if (ch == - 1 || end_of_val) {
/* At end of file. The value is complete, there can't be
             any continuation lines. */
      svn_config_set(ctx -> cfg,(ctx -> section -> data),(ctx -> option -> data),(ctx -> value -> data));
      break; 
    }
    else {
      int count;
      ++ctx -> line;
      do {
        svn_error_t *svn_err__temp = skip_whitespace(ctx,&ch,&count);
        if (svn_err__temp) {
          return svn_err__temp;
        }
      }while (0);
      switch(ch){
        case '\n':
{
/* The next line was empty. Ergo, it can't be a
                 continuation line. */
          ++ctx -> line;
          end_of_val = !0;
          continue; 
        }
        case - 1:
{
/* This is also an empty line. */
          end_of_val = !0;
          continue; 
        }
        default:
        if (count == 0) {
/* This line starts in the first column.  That means
                     it's either a section, option or comment.  Put
                     the char back into the stream, because it doesn't
                     belong to us. */
          do {
            svn_error_t *svn_err__temp = parser_ungetc(ctx,ch);
            if (svn_err__temp) {
              return svn_err__temp;
            }
          }while (0);
          end_of_val = !0;
        }
        else {
/* This is a continuation line. Read it. */
          svn_stringbuf_appendbyte(ctx -> value,32);
          while(ch != - 1 && ch != 10){
            const char char_from_int = (char )ch;
            svn_stringbuf_appendbyte(ctx -> value,char_from_int);
            do {
              svn_error_t *svn_err__temp = parser_getc(ctx,&ch);
              if (svn_err__temp) {
                return svn_err__temp;
              }
            }while (0);
          }
/* Trailing whitespace is ignored. */
          svn_stringbuf_strip_whitespace(ctx -> value);
        }
      }
    }
  }
   *pch = ch;
  return 0;
}
/* Parse a single option */

static svn_error_t *parse_option(int *pch,parse_context_t *ctx,apr_pool_t *scratch_pool)
{
  svn_error_t *err = 0;
  int ch;
  svn_stringbuf_setempty(ctx -> option);
/* Yes, the first char is relevant. */
  ch =  *pch;
  while(ch != - 1 && ch != ':' && ch != '=' && ch != 10){
    const char char_from_int = (char )ch;
    svn_stringbuf_appendbyte(ctx -> option,char_from_int);
    do {
      svn_error_t *svn_err__temp = parser_getc(ctx,&ch);
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
  }
  if (ch != ':' && ch != '=') {
    ch = - 1;
    err = svn_error_createf(SVN_ERR_MALFORMED_FILE,((void *)0),"line %d: Option must end with ':' or '='",ctx -> line);
  }
  else {
/* Whitespace around the name separator is ignored. */
    svn_stringbuf_strip_whitespace(ctx -> option);
    err = parse_value(&ch,ctx);
  }
   *pch = ch;
  return err;
}
/* Read chars until enounter ']', then skip everything to the end of
 * the line.  Set *PCH to the character that ended the line (either
 * newline or EOF), and set CTX->section to the string of characters
 * seen before ']'.
 *
 * This is meant to be called immediately after reading the '[' that
 * starts a section name.
 */

static svn_error_t *parse_section_name(int *pch,parse_context_t *ctx,apr_pool_t *scratch_pool)
{
  svn_error_t *err = 0;
  int ch;
  svn_stringbuf_setempty(ctx -> section);
  do {
    svn_error_t *svn_err__temp = parser_getc(ctx,&ch);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  while(ch != - 1 && ch != ']' && ch != 10){
    const char char_from_int = (char )ch;
    svn_stringbuf_appendbyte(ctx -> section,char_from_int);
    do {
      svn_error_t *svn_err__temp = parser_getc(ctx,&ch);
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
  }
  if (ch != ']') {
    ch = - 1;
    err = svn_error_createf(SVN_ERR_MALFORMED_FILE,((void *)0),"line %d: Section header must end with ']'",ctx -> line);
  }
  else {
/* Everything from the ']' to the end of the line is ignored. */
    do {
      svn_error_t *svn_err__temp = skip_to_eoln(ctx,&ch);
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
    if (ch != - 1) {
      ++ctx -> line;
    }
  }
   *pch = ch;
  return err;
}

svn_error_t *svn_config__sys_config_path(const char **path_p,const char *fname,apr_pool_t *pool)
{
   *path_p = ((void *)0);
/* Note that even if fname is null, svn_dirent_join_many will DTRT. */
#ifdef WIN32
#elif defined(__HAIKU__)
#else  /* ! WIN32 && !__HAIKU__ */
   *path_p = (svn_dirent_join_many(pool,"/etc/subversion",fname,((void *)0)));
#endif /* WIN32 */
  return 0;
}
/*** Exported interfaces. ***/

svn_error_t *svn_config__parse_file(svn_config_t *cfg,const char *file,svn_boolean_t must_exist,apr_pool_t *result_pool)
{
  svn_error_t *err = 0;
  svn_stream_t *stream;
  apr_pool_t *scratch_pool = svn_pool_create_ex(result_pool,((void *)0));
  err = svn_stream_open_readonly(&stream,file,scratch_pool,scratch_pool);
  if (!must_exist && err && err -> apr_err == 2) {
    svn_error_clear(err);
    apr_pool_destroy(scratch_pool);
    return 0;
  }
  else {
    do {
      svn_error_t *svn_err__temp = err;
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
  }
  err = svn_config__parse_stream(cfg,stream,result_pool,scratch_pool);
  if (err != 0) {
/* Add the filename to the error stack. */
    err = svn_error_createf(err -> apr_err,err,"Error while parsing config file: %s:",svn_dirent_local_style(file,scratch_pool));
  }
/* Close the streams (and other cleanup): */
  apr_pool_destroy(scratch_pool);
  return err;
}

svn_error_t *svn_config__parse_stream(svn_config_t *cfg,svn_stream_t *stream,apr_pool_t *result_pool,apr_pool_t *scratch_pool)
{
  parse_context_t *ctx;
  int ch;
  int count;
  ctx = (apr_palloc(scratch_pool,sizeof(( *ctx))));
  ctx -> cfg = cfg;
  ctx -> stream = stream;
  ctx -> line = 1;
  ctx -> ungotten_char = - 1;
  ctx -> section = svn_stringbuf_create_empty(scratch_pool);
  ctx -> option = svn_stringbuf_create_empty(scratch_pool);
  ctx -> value = svn_stringbuf_create_empty(scratch_pool);
  ctx -> buffer_pos = 0;
  ctx -> buffer_size = 0;
  do {
    svn_error_t *svn_err__temp = skip_bom(ctx);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  do {
    do {
      svn_error_t *svn_err__temp = skip_whitespace(ctx,&ch,&count);
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
    switch(ch){
      case '[':
{
/* Start of section header */
        if (count == 0) {
          do {
            svn_error_t *svn_err__temp = parse_section_name(&ch,ctx,scratch_pool);
            if (svn_err__temp) {
              return svn_err__temp;
            }
          }while (0);
        }
        else {
          return svn_error_createf(SVN_ERR_MALFORMED_FILE,((void *)0),"line %d: Section header must start in the first column",ctx -> line);
        }
        break; 
      }
      case '#':
{
/* Comment */
        if (count == 0) {
          do {
            svn_error_t *svn_err__temp = skip_to_eoln(ctx,&ch);
            if (svn_err__temp) {
              return svn_err__temp;
            }
          }while (0);
          ++ctx -> line;
        }
        else {
          return svn_error_createf(SVN_ERR_MALFORMED_FILE,((void *)0),"line %d: Comment must start in the first column",ctx -> line);
        }
        break; 
      }
      case '\n':
{
/* Empty line */
        ++ctx -> line;
        break; 
      }
      case - 1:
/* End of file or read error */
      break; 
      default:
{
        if (svn_stringbuf_isempty((ctx -> section))) {
          return svn_error_createf(SVN_ERR_MALFORMED_FILE,((void *)0),"line %d: Section header expected",ctx -> line);
        }
        else {
          if (count != 0) {
            return svn_error_createf(SVN_ERR_MALFORMED_FILE,((void *)0),"line %d: Option expected",ctx -> line);
          }
          else {
            do {
              svn_error_t *svn_err__temp = parse_option(&ch,ctx,scratch_pool);
              if (svn_err__temp) {
                return svn_err__temp;
              }
            }while (0);
          }
        }
        break; 
      }
    }
  }while (ch != - 1);
  return 0;
}
/* Helper for ensure_auth_dirs: create SUBDIR under AUTH_DIR, iff
   SUBDIR does not already exist, but ignore any errors.  Use POOL for
   temporary allocation. */

static void ensure_auth_subdir(const char *auth_dir,const char *subdir,apr_pool_t *pool)
{
  svn_error_t *err;
  const char *subdir_full_path;
  svn_node_kind_t kind;
  subdir_full_path = (svn_dirent_join(auth_dir,subdir,pool));
  err = svn_io_check_path(subdir_full_path,&kind,pool);
  if (err || kind == svn_node_none) {
    svn_error_clear(err);
    svn_error_clear(svn_io_dir_make(subdir_full_path,0x0FFF,pool));
  }
}
/* Helper for svn_config_ensure:  see if ~/.subversion/auth/ and its
   subdirs exist, try to create them, but don't throw errors on
   failure.  PATH is assumed to be a path to the user's private config
   directory. */

static void ensure_auth_dirs(const char *path,apr_pool_t *pool)
{
  svn_node_kind_t kind;
  const char *auth_dir;
  svn_error_t *err;
/* Ensure ~/.subversion/auth/ */
  auth_dir = (svn_dirent_join(path,"auth",pool));
  err = svn_io_check_path(auth_dir,&kind,pool);
  if (err || kind == svn_node_none) {
    svn_error_clear(err);
/* 'chmod 700' permissions: */
    err = svn_io_dir_make(auth_dir,0x0400 | 0x0200 | 0x0100,pool);
    if (err) {
/* Don't try making subdirs if we can't make the top-level dir. */
      svn_error_clear(err);
      return ;
    }
  }
/* If a provider exists that wants to store credentials in
     ~/.subversion, a subdirectory for the cred_kind must exist. */
  ensure_auth_subdir(auth_dir,"svn.simple",pool);
  ensure_auth_subdir(auth_dir,"svn.username",pool);
  ensure_auth_subdir(auth_dir,"svn.ssl.server",pool);
  ensure_auth_subdir(auth_dir,"svn.ssl.client-passphrase",pool);
}

svn_error_t *svn_config_ensure(const char *config_dir,apr_pool_t *pool)
{
  const char *path;
  svn_node_kind_t kind;
  svn_error_t *err;
/* Ensure that the user-specific config directory exists.  */
  do {
    svn_error_t *svn_err__temp = svn_config_get_user_config_path(&path,config_dir,((void *)0),pool);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  if (!path) {
    return 0;
  }
  err = svn_io_check_resolved_path(path,&kind,pool);
  if (err) {
/* Don't throw an error, but don't continue. */
    svn_error_clear(err);
    return 0;
  }
  if (kind == svn_node_none) {
    err = svn_io_dir_make(path,0x0FFF,pool);
    if (err) {
/* Don't throw an error, but don't continue. */
      svn_error_clear(err);
      return 0;
    }
  }
  else {
    if (kind == svn_node_file) {
/* Somebody put a file where the config directory should be.
         Wacky.  Let's bail. */
      return 0;
    }
  }
/* Else, there's a configuration directory. */
/* If we get errors trying to do things below, just stop and return
     success.  There's no _need_ to init a config directory if
     something's preventing it. */
/** If non-existent, try to create a number of auth/ subdirectories. */
  ensure_auth_dirs(path,pool);
/** Ensure that the `README.txt' file exists. **/
  do {
    svn_error_t *svn_err__temp = svn_config_get_user_config_path(&path,config_dir,"README.txt",pool);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
/* highly unlikely, since a previous call succeeded */
  if (!path) {
    return 0;
  }
  err = svn_io_check_path(path,&kind,pool);
  if (err) {
    svn_error_clear(err);
    return 0;
  }
  if (kind == svn_node_none) {
    apr_file_t *f;
    const char *contents = "This directory holds run-time configuration information for Subversion\nclients.  The configuration files all share the same syntax, but you\nshould examine a particular file to learn what configuration\ndirectives are valid for that file.\n\nThe syntax is standard INI format:\n\n   - Empty lines, and lines starting with '#', are ignored.\n     The first significant line in a file must be a section header.\n\n   - A section starts with a section header, which must start in\n     the first column:\n\n       [section-name]\n\n   - An option, which must always appear within a section, is a pair\n     (name, value).  There are two valid forms for defining an\n     option, both of which must start in the first column:\n\n       name: value\n       name = value\n\n     Whitespace around the separator (:, =) is optional.\n\n   - Section and option names are case-insensitive, but case is\n     preserved.\n\n   - An option's value may be broken into several lines.  The value\n     continuation lines must start with at least one whitespace.\n     Trailing whitespace in the previous line, the newline character\n     and the leading whitespace in the continuation line is compressed\n     into a single space character.\n\n   - All leading and trailing whitespace around a value is trimmed,\n     but the whitespace within a value is preserved, with the\n     exception of whitespace around line continuations, as\n     described above.\n\n   - When a value is a boolean, any of the following strings are\n     recognised as truth values (case does not matter):\n\n       true      false\n       yes       no\n       on        off\n       1         0\n\n   - When a value is a list, it is comma-separated.  Again, the\n     whitespace around each element of the list is trimmed.\n\n   - Option values may be expanded within a value by enclosing the\n     option name in parentheses, preceded by a percent sign and\n     followed by an 's':\n\n       %(name)s\n\n     The expansion is performed recursively and on demand, during\n     svn_option_get.  The name is first searched for in the same\n     section, then in the special [DEFAULT] section. If the name\n     is not found, the whole '%(name)s' placeholder is left\n     unchanged.\n\n     Any modifications to the configuration data invalidate all\n     previously expanded values, so that the next svn_option_get\n     will take the modifications into account.\n\nThe syntax of the configuration files is a subset of the one used by\nPython's ConfigParser module; see\n\n   http://www.python.org/doc/current/lib/module-ConfigParser.html\n\nConfiguration data in the Windows registry\n==========================================\n\nOn Windows, configuration data may also be stored in the registry. The\nfunctions svn_config_read and svn_config_merge will read from the\nregistry when passed file names of the form:\n\n   REGISTRY:<hive>/path/to/config-key\n\nThe REGISTRY: prefix must be in upper case. The <hive> part must be\none of:\n\n   HKLM for HKEY_LOCAL_MACHINE\n   HKCU for HKEY_CURRENT_USER\n\nThe values in config-key represent the options in the [DEFAULT] section.\nThe keys below config-key represent other sections, and their values\nrepresent the options. Only values of type REG_SZ whose name doesn't\nstart with a '#' will be used; other values, as well as the keys'\ndefault values, will be ignored.\n\n\nFile locations\n==============\n\nTypically, Subversion uses two config directories, one for site-wide\nconfiguration,\n\n  Unix:\n    /etc/subversion/servers\n    /etc/subversion/config\n    /etc/subversion/hairstyles\n  Windows:\n    %ALLUSERSPROFILE%\\Application Data\\Subversion\\servers\n    %ALLUSERSPROFILE%\\Application Data\\Subversion\\config\n    %ALLUSERSPROFILE%\\Application Data\\Subversion\\hairstyles\n    REGISTRY:HKLM\\Software\\Tigris.org\\Subversion\\Servers\n    REGISTRY:HKLM\\Software\\Tigris.org\\Subversion\\Config\n    REGISTRY:HKLM\\Software\\Tigris.org\\Subversion\\Hairstyles\n\nand one for per-user configuration:\n\n  Unix:\n    ~/.subversion/servers\n    ~/.subversion/config\n    ~/.subversion/hairstyles\n  Windows:\n    %APPDATA%\\Subversion\\servers\n    %APPDATA%\\Subversion\\config\n    %APPDATA%\\Subversion\\hairstyles\n    REGISTRY:HKCU\\Software\\Tigris.org\\Subversion\\Servers\n    REGISTRY:HKCU\\Software\\Tigris.org\\Subversion\\Config\n    REGISTRY:HKCU\\Software\\Tigris.org\\Subversion\\Hairstyles\n\n";
    err = svn_io_file_open(&f,path,0x0002 | 0x00004 | 0x00040,0x0FFF,pool);
    if (!err) {
      do {
        svn_error_t *svn_err__temp = svn_io_file_write_full(f,contents,strlen(contents),((void *)0),pool);
        if (svn_err__temp) {
          return svn_err__temp;
        }
      }while (0);
      do {
        svn_error_t *svn_err__temp = svn_io_file_close(f,pool);
        if (svn_err__temp) {
          return svn_err__temp;
        }
      }while (0);
    }
    svn_error_clear(err);
  }
/** Ensure that the `servers' file exists. **/
  do {
    svn_error_t *svn_err__temp = svn_config_get_user_config_path(&path,config_dir,"servers",pool);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
/* highly unlikely, since a previous call succeeded */
  if (!path) {
    return 0;
  }
  err = svn_io_check_path(path,&kind,pool);
  if (err) {
    svn_error_clear(err);
    return 0;
  }
  if (kind == svn_node_none) {
    apr_file_t *f;
    const char *contents = "### This file specifies server-specific parameters,\n### including HTTP proxy information, HTTP timeout settings,\n### and authentication settings.\n###\n### The currently defined server options are:\n###   http-proxy-host            Proxy host for HTTP connection\n###   http-proxy-port            Port number of proxy host service\n###   http-proxy-username        Username for auth to proxy service\n###   http-proxy-password        Password for auth to proxy service\n###   http-proxy-exceptions      List of sites that do not use proxy\n###   http-timeout               Timeout for HTTP requests in seconds\n###   http-compression           Whether to compress HTTP requests\n###   http-max-connections       Maximum number of parallel server\n###                              connections to use for any given\n###                              HTTP operation.\n###   http-chunked-requests      Whether to use chunked transfer\n###                              encoding for HTTP requests body.\n###   neon-debug-mask            Debug mask for Neon HTTP library\n###   ssl-authority-files        List of files, each of a trusted CA\n###   ssl-trust-default-ca       Trust the system 'default' CAs\n###   ssl-client-cert-file       PKCS#12 format client certificate file\n###   ssl-client-cert-password   Client Key password, if needed.\n###   ssl-pkcs11-provider        Name of PKCS#11 provider to use.\n###   http-library               Which library to use for http/https\n###                              connections.\n###   http-bulk-updates          Whether to request bulk update\n###                              responses or to fetch each file\n###                              in an individual request. \n###   store-passwords            Specifies whether passwords used\n###                              to authenticate against a\n###                              Subversion server may be cached\n###                              to disk in any way.\n###   store-plaintext-passwords  Specifies whether passwords may\n###                              be cached on disk unencrypted.\n###   store-ssl-client-cert-pp   Specifies whether passphrase used\n###                              to authenticate against a client\n###                              certificate may be cached to disk\n###                              in any way\n###   store-ssl-client-cert-pp-plaintext\n###                              Specifies whether client cert\n###                              passphrases may be cached on disk\n###                              unencrypted (i.e., as plaintext).\n###   store-auth-creds           Specifies whether any auth info\n###                              (passwords, server certs, etc.)\n###                              may be cached to disk.\n###   username                   Specifies the default username.\n###\n### Set store-passwords to 'no' to avoid storing passwords on disk\n### in any way, including in password stores.  It defaults to\n### 'yes', but Subversion will never save your password to disk in\n### plaintext unless explicitly configured to do so.\n### Note that this option only prevents saving of *new* passwords;\n### it doesn't invalidate existing passwords.  (To do that, remove\n### the cache files by hand as described in the Subversion book.)\n###\n### Set store-plaintext-passwords to 'no' to avoid storing\n### passwords in unencrypted form in the auth/ area of your config\n### directory. Set it to 'yes' to allow Subversion to store\n### unencrypted passwords in the auth/ area.  The default is\n### 'ask', which means that Subversion will ask you before\n### saving a password to disk in unencrypted form.  Note that\n### this option has no effect if either 'store-passwords' or \n### 'store-auth-creds' is set to 'no'.\n###\n### Set store-ssl-client-cert-pp to 'no' to avoid storing ssl\n### client certificate passphrases in the auth/ area of your\n### config directory.  It defaults to 'yes', but Subversion will\n### never save your passphrase to disk in plaintext unless\n### explicitly configured to do so.\n###\n### Note store-ssl-client-cert-pp only prevents the saving of *new*\n### passphrases; it doesn't invalidate existing passphrases.  To do\n### that, remove the cache files by hand as described in the\n### Subversion book at http://svnbook.red-bean.com/nightly/en/\\\n###                    svn.serverconfig.netmodel.html\\\n###                    #svn.serverconfig.netmodel.credcache\n###\n### Set store-ssl-client-cert-pp-plaintext to 'no' to avoid storing\n### passphrases in unencrypted form in the auth/ area of your\n### config directory.  Set it to 'yes' to allow Subversion to\n### store unencrypted passphrases in the auth/ area.  The default\n### is 'ask', which means that Subversion will prompt before\n### saving a passphrase to disk in unencrypted form.  Note that\n### this option has no effect if either 'store-auth-creds' or \n### 'store-ssl-client-cert-pp' is set to 'no'.\n###\n### Set store-auth-creds to 'no' to avoid storing any Subversion\n### credentials in the auth/ area of your config directory.\n### Note that this includes SSL server certificates.\n### It defaults to 'yes'.  Note that this option only prevents\n### saving of *new* credentials;  it doesn't invalidate existing\n### caches.  (To do that, remove the cache files by hand.)\n###\n### HTTP timeouts, if given, are specified in seconds.  A timeout\n### of 0, i.e. zero, causes a builtin default to be used.\n###\n### Most users will not need to explicitly set the http-library\n### option, but valid values for the option include:\n###    'serf': Serf-based module (Subversion 1.5 - present)\n###    'neon': Neon-based module (Subversion 1.0 - 1.7)\n### Availability of these modules may depend on your specific\n### Subversion distribution.\n###\n### The commented-out examples below are intended only to\n### demonstrate how to use this file; any resemblance to actual\n### servers, living or dead, is entirely coincidental.\n\n### In the 'groups' section, the URL of the repository you're\n### trying to access is matched against the patterns on the right.\n### If a match is found, the server options are taken from the\n### section with the corresponding name on the left.\n\n[groups]\n# group1 = *.collab.net\n# othergroup = repository.blarggitywhoomph.com\n# thirdgroup = *.example.com\n\n### Information for the first group:\n# [group1]\n# http-proxy-host = proxy1.some-domain-name.com\n# http-proxy-port = 80\n# http-proxy-username = blah\n# http-proxy-password = doubleblah\n# http-timeout = 60\n# neon-debug-mask = 130\n# store-plaintext-passwords = no\n# username = harry\n\n### Information for the second group:\n# [othergroup]\n# http-proxy-host = proxy2.some-domain-name.com\n# http-proxy-port = 9000\n# No username and password for the proxy, so use the defaults below.\n\n### You can set default parameters in the 'global' section.\n### These parameters apply if no corresponding parameter is set in\n### a specifically matched group as shown above.  Thus, if you go\n### through the same proxy server to reach every site on the\n### Internet, you probably just want to put that server's\n### information in the 'global' section and not bother with\n### 'groups' or any other sections.\n###\n### Most people might want to configure password caching\n### parameters here, but you can also configure them per server\n### group (per-group settings override global settings).\n###\n### If you go through a proxy for all but a few sites, you can\n### list those exceptions under 'http-proxy-exceptions'.  This only\n### overrides defaults, not explicitly matched server names.\n###\n### 'ssl-authority-files' is a semicolon-delimited list of files,\n### each pointing to a PEM-encoded Certificate Authority (CA) \n### SSL certificate.  See details above for overriding security \n### due to SSL.\n[global]\n# http-proxy-exceptions = *.exception.com, www.internal-site.org\n# http-proxy-host = defaultproxy.whatever.com\n# http-proxy-port = 7000\n# http-proxy-username = defaultusername\n# http-proxy-password = defaultpassword\n# http-compression = no\n# No http-timeout, so just use the builtin default.\n# No neon-debug-mask, so neon debugging is disabled.\n# ssl-authority-files = /path/to/CAcert.pem;/path/to/CAcert2.pem\n#\n# Password / passphrase caching parameters:\n# store-passwords = no\n# store-ssl-client-cert-pp = no\n# store-plaintext-passwords = no\n# store-ssl-client-cert-pp-plaintext = no\n";
#ifndef SVN_DISABLE_PLAINTEXT_PASSWORD_STORAGE
#endif
#ifndef SVN_DISABLE_PLAINTEXT_PASSWORD_STORAGE
#endif
#ifndef SVN_DISABLE_PLAINTEXT_PASSWORD_STORAGE
#endif
#ifndef SVN_DISABLE_PLAINTEXT_PASSWORD_STORAGE
#endif
#ifndef SVN_DISABLE_PLAINTEXT_PASSWORD_STORAGE
#endif
#ifndef SVN_DISABLE_PLAINTEXT_PASSWORD_STORAGE
#endif
    err = svn_io_file_open(&f,path,0x0002 | 0x00004 | 0x00040,0x0FFF,pool);
    if (!err) {
      do {
        svn_error_t *svn_err__temp = svn_io_file_write_full(f,contents,strlen(contents),((void *)0),pool);
        if (svn_err__temp) {
          return svn_err__temp;
        }
      }while (0);
      do {
        svn_error_t *svn_err__temp = svn_io_file_close(f,pool);
        if (svn_err__temp) {
          return svn_err__temp;
        }
      }while (0);
    }
    svn_error_clear(err);
  }
/** Ensure that the `config' file exists. **/
  do {
    svn_error_t *svn_err__temp = svn_config_get_user_config_path(&path,config_dir,"config",pool);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
/* highly unlikely, since a previous call succeeded */
  if (!path) {
    return 0;
  }
  err = svn_io_check_path(path,&kind,pool);
  if (err) {
    svn_error_clear(err);
    return 0;
  }
  if (kind == svn_node_none) {
    apr_file_t *f;
    const char *contents = "### This file configures various client-side behaviors.\n###\n### The commented-out examples below are intended to demonstrate\n### how to use this file.\n\n### Section for authentication and authorization customizations.\n[auth]\n### Set password stores used by Subversion. They should be\n### delimited by spaces or commas. The order of values determines\n### the order in which password stores are used.\n### Valid password stores:\n###   gnome-keyring        (Unix-like systems)\n###   kwallet              (Unix-like systems)\n###   gpg-agent            (Unix-like systems)\n###   keychain             (Mac OS X)\n###   windows-cryptoapi    (Windows)\n# password-stores = gpg-agent,gnome-keyring,kwallet\n### To disable all password stores, use an empty list:\n# password-stores =\n###\n### Set ssl-client-cert-file-prompt to 'yes' to cause the client\n### to prompt for a path to a client cert file when the server\n### requests a client cert but no client cert file is found in the\n### expected place (see the 'ssl-client-cert-file' option in the\n### 'servers' configuration file). Defaults to 'no'.\n# ssl-client-cert-file-prompt = no\n###\n### The rest of the [auth] section in this file has been deprecated.\n### Both 'store-passwords' and 'store-auth-creds' can now be\n### specified in the 'servers' file in your config directory\n### and are documented there. Anything specified in this section \n### is overridden by settings specified in the 'servers' file.\n# store-passwords = no\n# store-auth-creds = no\n\n### Section for configuring external helper applications.\n[helpers]\n### Set editor-cmd to the command used to invoke your text editor.\n###   This will override the environment variables that Subversion\n###   examines by default to find this information ($EDITOR, \n###   et al).\n# editor-cmd = editor (vi, emacs, notepad, etc.)\n### Set diff-cmd to the absolute path of your 'diff' program.\n###   This will override the compile-time default, which is to use\n###   Subversion's internal diff implementation.\n# diff-cmd = diff_program (diff, gdiff, etc.)\n### Diff-extensions are arguments passed to an external diff\n### program or to Subversion's internal diff implementation.\n### Set diff-extensions to override the default arguments ('-u').\n# diff-extensions = -u -p\n### Set diff3-cmd to the absolute path of your 'diff3' program.\n###   This will override the compile-time default, which is to use\n###   Subversion's internal diff3 implementation.\n# diff3-cmd = diff3_program (diff3, gdiff3, etc.)\n### Set diff3-has-program-arg to 'yes' if your 'diff3' program\n###   accepts the '--diff-program' option.\n# diff3-has-program-arg = [yes | no]\n### Set merge-tool-cmd to the command used to invoke your external\n### merging tool of choice. Subversion will pass 5 arguments to\n### the specified command: base theirs mine merged wcfile\n# merge-tool-cmd = merge_command\n\n### Section for configuring tunnel agents.\n[tunnels]\n### Configure svn protocol tunnel schemes here.  By default, only\n### the 'ssh' scheme is defined.  You can define other schemes to\n### be used with 'svn+scheme://hostname/path' URLs.  A scheme\n### definition is simply a command, optionally prefixed by an\n### environment variable name which can override the command if it\n### is defined.  The command (or environment variable) may contain\n### arguments, using standard shell quoting for arguments with\n### spaces.  The command will be invoked as:\n###   <command> <hostname> svnserve -t\n### (If the URL includes a username, then the hostname will be\n### passed to the tunnel agent as <user>@<hostname>.)  If the\n### built-in ssh scheme were not predefined, it could be defined\n### as:\n# ssh = $SVN_SSH ssh -q\n### If you wanted to define a new 'rsh' scheme, to be used with\n### 'svn+rsh:' URLs, you could do so as follows:\n# rsh = rsh\n### Or, if you wanted to specify a full path and arguments:\n# rsh = /path/to/rsh -l myusername\n### On Windows, if you are specifying a full path to a command,\n### use a forward slash (/) or a paired backslash (\\\\) as the\n### path separator.  A single backslash will be treated as an\n### escape for the following character.\n\n### Section for configuring miscellaneous Subversion options.\n[miscellany]\n### Set global-ignores to a set of whitespace-delimited globs\n### which Subversion will ignore in its 'status' output, and\n### while importing or adding files and directories.\n### '*' matches leading dots, e.g. '*.rej' matches '.foo.rej'.\n# global-ignores = *.o *.lo *.la *.al .libs *.so *.so.[0-9]* *.a *.pyc *.pyo __pycache__\n#   *.rej *~ #*# .#* .*.swp .DS_Store\n### Set log-encoding to the default encoding for log messages\n# log-encoding = latin1\n### Set use-commit-times to make checkout/update/switch/revert\n### put last-committed timestamps on every file touched.\n# use-commit-times = yes\n### Set no-unlock to prevent 'svn commit' from automatically\n### releasing locks on files.\n# no-unlock = yes\n### Set mime-types-file to a MIME type registry file, used to\n### provide hints to Subversion's MIME type auto-detection\n### algorithm.\n# mime-types-file = /path/to/mime.types\n### Set preserved-conflict-file-exts to a whitespace-delimited\n### list of patterns matching file extensions which should be\n### preserved in generated conflict file names.  By default,\n### conflict files use custom extensions.\n# preserved-conflict-file-exts = doc ppt xls od?\n### Set enable-auto-props to 'yes' to enable automatic properties\n### for 'svn add' and 'svn import', it defaults to 'no'.\n### Automatic properties are defined in the section 'auto-props'.\n# enable-auto-props = yes\n### Set interactive-conflicts to 'no' to disable interactive\n### conflict resolution prompting.  It defaults to 'yes'.\n# interactive-conflicts = no\n### Set memory-cache-size to define the size of the memory cache\n### used by the client when accessing a FSFS repository via\n### ra_local (the file:// scheme). The value represents the number\n### of MB used by the cache.\n# memory-cache-size = 16\n\n### Section for configuring automatic properties.\n[auto-props]\n### The format of the entries is:\n###   file-name-pattern = propname[=value][;propname[=value]...]\n### The file-name-pattern can contain wildcards (such as '*' and\n### '?').  All entries which match (case-insensitively) will be\n### applied to the file.  Note that auto-props functionality\n### must be enabled, which is typically done by setting the\n### 'enable-auto-props' option.\n# *.c = svn:eol-style=native\n# *.cpp = svn:eol-style=native\n# *.h = svn:keywords=Author Date Id Rev URL;svn:eol-style=native\n# *.dsp = svn:eol-style=CRLF\n# *.dsw = svn:eol-style=CRLF\n# *.sh = svn:eol-style=native;svn:executable\n# *.txt = svn:eol-style=native;svn:keywords=Author Date Id Rev URL;\n# *.png = svn:mime-type=image/png\n# *.jpg = svn:mime-type=image/jpeg\n# Makefile = svn:eol-style=native\n\n### Section for configuring working copies.\n[working-copy]\n### Set to a list of the names of specific clients that should use\n### exclusive SQLite locking of working copies.  This increases the\n### performance of the client but prevents concurrent access by\n### other clients.  Third-party clients may also support this\n### option.\n### Possible values:\n###   svn                (the command line client)\n# exclusive-locking-clients =\n### Set to true to enable exclusive SQLite locking of working\n### copies by all clients using the 1.8 APIs.  Enabling this may\n### cause some clients to fail to work properly. This does not have\n### to be set for exclusive-locking-clients to work.\n# exclusive-locking = false\n";
#ifdef SVN_HAVE_KEYCHAIN_SERVICES
#elif defined(WIN32) && !defined(__MINGW32__)
#else
#endif
#ifdef SVN_HAVE_KWALLET
#endif
    err = svn_io_file_open(&f,path,0x0002 | 0x00004 | 0x00040,0x0FFF,pool);
    if (!err) {
      do {
        svn_error_t *svn_err__temp = svn_io_file_write_full(f,contents,strlen(contents),((void *)0),pool);
        if (svn_err__temp) {
          return svn_err__temp;
        }
      }while (0);
      do {
        svn_error_t *svn_err__temp = svn_io_file_close(f,pool);
        if (svn_err__temp) {
          return svn_err__temp;
        }
      }while (0);
    }
    svn_error_clear(err);
  }
  return 0;
}

svn_error_t *svn_config_get_user_config_path(const char **path,const char *config_dir,const char *fname,apr_pool_t *pool)
{
   *path = ((void *)0);
/* Note that even if fname is null, svn_dirent_join_many will DTRT. */
  if (config_dir) {
     *path = (svn_dirent_join_many(pool,config_dir,fname,((void *)0)));
    return 0;
  }
#ifdef WIN32
#elif defined(__HAIKU__)
#else  /* ! WIN32 && !__HAIKU__ */
{
    const char *homedir = svn_user_get_homedir(pool);
    if (!homedir) {
      return 0;
    }
     *path = (svn_dirent_join_many(pool,svn_dirent_canonicalize(homedir,pool),".subversion",fname,((void *)0)));
  }
#endif /* WIN32 */
  return 0;
}

void reinvestigation_deafened(int gadling_vincristine,void *alcibiades_nonbusiness)
{
 int stonesoup_input_len = 0;
 int stonesoup_result = 0;
 fptr* stonesoup_function_ptr = 0;
  char *selenodonta_spirochaetotic = 0;
  ++stonesoup_global_variable;
  gadling_vincristine--;
  if (gadling_vincristine > 0) {
    reinvestigation_deafened(gadling_vincristine,alcibiades_nonbusiness);
    return ;
  }
  selenodonta_spirochaetotic = ((char *)((char *)alcibiades_nonbusiness));
    tracepoint(stonesoup_trace, weakness_start, "CWE824", "B", "Access of Uninitialized Pointer");
 stonesoup_input_len = strlen(selenodonta_spirochaetotic);
    if (stonesoup_input_len < 2) {
        stonesoup_printf("String is too short to test\n");
    } else {
        stonesoup_function_ptr = malloc(sizeof(void *));
        if (stonesoup_function_ptr == 0) {
            stonesoup_printf("Error: Failed to allocate memory\n");
            exit(1);
        }
        /* STONESOUP: CROSSOVER-POINT (Uninitialized Pointer) */
        stonesoup_get_function(stonesoup_input_len, stonesoup_function_ptr);
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        /* STONESOUP: TRIGGER-POINT (Uninitialized Pointer) */
        stonesoup_result = ( *stonesoup_function_ptr)(selenodonta_spirochaetotic);
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        if (stonesoup_result == 0)
            stonesoup_printf("mod is true\n");
        else
            stonesoup_printf("mod is false\n");
        if (stonesoup_function_ptr != 0) {
            free(stonesoup_function_ptr);
        }
    }
    tracepoint(stonesoup_trace, weakness_end);
;
stonesoup_close_printf_context();
}
