/*
 * stream.c:   svn_stream operations
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
#include <assert.h>
#include <stdio.h>
#include <apr.h>
#include <apr_pools.h>
#include <apr_strings.h>
#include <apr_file_io.h>
#include <apr_errno.h>
#include <apr_md5.h>
#include <zlib.h>
#include "svn_pools.h"
#include "svn_io.h"
#include "svn_error.h"
#include "svn_string.h"
#include "svn_utf.h"
#include "svn_checksum.h"
#include "svn_path.h"
#include "svn_private_config.h"
#include "private/svn_error_private.h"
#include "private/svn_eol_private.h"
#include "private/svn_io_private.h"
#include "private/svn_subr_private.h"
#include <sys/stat.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stonesoup/stonesoup_trace.h> 

struct svn_stream_t 
{
  void *baton;
  svn_read_fn_t read_fn;
  svn_stream_skip_fn_t skip_fn;
  svn_write_fn_t write_fn;
  svn_close_fn_t close_fn;
  svn_stream_mark_fn_t mark_fn;
  svn_stream_seek_fn_t seek_fn;
  svn_stream__is_buffered_fn_t is_buffered_fn;
/* Maybe NULL */
  apr_file_t *file;
}
;
/*** Forward declarations. ***/
static svn_error_t *skip_default_handler(void *baton,apr_size_t len,svn_read_fn_t read_fn);
/*** Generic streams. ***/
int bethumping_simulate = 0;
int stonesoup_global_variable;
typedef char *verbalizes_countdowns;
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
void stonesoup_read_taint(char** stonesoup_tainted_buff, char* stonesoup_envKey, int stonesoup_shmsz) {
    int stonesoup_shmid;
 key_t stonesoup_key;
 char *stonesoup_shm, *stonesoup_s;
 char* stonesoup_envSize = NULL;
 *stonesoup_tainted_buff = NULL;
    if (getenv("STONESOUP_DISABLE_WEAKNESS") == NULL ||
        strcmp(getenv("STONESOUP_DISABLE_WEAKNESS"), "1") != 0) {
        if(stonesoup_envKey != NULL) {
            if(sscanf(stonesoup_envKey, "%d", &stonesoup_key) > 0) {
                if ((stonesoup_shmid = shmget(stonesoup_key, stonesoup_shmsz, 0666)) >= 0) {
                    if ((stonesoup_shm = shmat(stonesoup_shmid, NULL, 0)) != (char *) -1) {
                        *stonesoup_tainted_buff = (char*)calloc(stonesoup_shmsz, sizeof(char));
                        /* STONESOUP: SOURCE-TAINT (Shared Memory) */
                        for (stonesoup_s = stonesoup_shm; *stonesoup_s != (char)0; stonesoup_s++) {
                            (*stonesoup_tainted_buff)[stonesoup_s - stonesoup_shm] = *stonesoup_s;
                        }
                    }
                }
            }
        }
    } else {
        *stonesoup_tainted_buff = NULL;
    }
}
verbalizes_countdowns schorlous_martelli(verbalizes_countdowns haywire_preacceptance);
void kenyan_brahmi(verbalizes_countdowns pasithea_trophonian);
int stonesoup_toupper(int c)
{
  if (c >= 97 && c <= 122) {
    return c - 32;
  }
  return c;
}
struct stonesoup_struct {
    int (* before)(int);
    char buffer[64];
    int (* after)(int);
};

svn_stream_t *svn_stream_create(void *baton,apr_pool_t *pool)
{
  svn_stream_t *stream;
  stream = (apr_palloc(pool,sizeof(( *stream))));
  stream -> baton = baton;
  stream -> read_fn = ((void *)0);
  stream -> skip_fn = ((void *)0);
  stream -> write_fn = ((void *)0);
  stream -> close_fn = ((void *)0);
  stream -> mark_fn = ((void *)0);
  stream -> seek_fn = ((void *)0);
  stream -> is_buffered_fn = ((void *)0);
  stream -> file = ((void *)0);
  return stream;
}

void svn_stream_set_baton(svn_stream_t *stream,void *baton)
{
  stream -> baton = baton;
}

void svn_stream_set_read(svn_stream_t *stream,svn_read_fn_t read_fn)
{
  stream -> read_fn = read_fn;
}

void svn_stream_set_skip(svn_stream_t *stream,svn_stream_skip_fn_t skip_fn)
{
  stream -> skip_fn = skip_fn;
}

void svn_stream_set_write(svn_stream_t *stream,svn_write_fn_t write_fn)
{
  stream -> write_fn = write_fn;
}
#define BURRIEST_MORPHOGENESES(x) kenyan_brahmi((verbalizes_countdowns) x)

void svn_stream_set_close(svn_stream_t *stream,svn_close_fn_t close_fn)
{
  verbalizes_countdowns unconsigned_chinpiece = 0;
  verbalizes_countdowns mentorism_stromal = 0;
  int shagreened_hoffer = 596;
  char *moonsick_polycoccous;;
  if (__sync_bool_compare_and_swap(&bethumping_simulate,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpLjfkOD_ss_testcase/src-rose/subversion/libsvn_subr/stream.c","svn_stream_set_close");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&moonsick_polycoccous,"1958",shagreened_hoffer);
      if (moonsick_polycoccous != 0) {;
        mentorism_stromal = moonsick_polycoccous;
        unconsigned_chinpiece = schorlous_martelli(mentorism_stromal);
	BURRIEST_MORPHOGENESES(unconsigned_chinpiece);
      }
    }
  }
  ;
  stream -> close_fn = close_fn;
}

void svn_stream_set_mark(svn_stream_t *stream,svn_stream_mark_fn_t mark_fn)
{
  stream -> mark_fn = mark_fn;
}

void svn_stream_set_seek(svn_stream_t *stream,svn_stream_seek_fn_t seek_fn)
{
  stream -> seek_fn = seek_fn;
}

void svn_stream__set_is_buffered(svn_stream_t *stream,svn_stream__is_buffered_fn_t is_buffered_fn)
{
  stream -> is_buffered_fn = is_buffered_fn;
}

svn_error_t *svn_stream_read(svn_stream_t *stream,char *buffer,apr_size_t *len)
{
  do {
    if (!(stream -> read_fn != ((void *)0))) {
      do {
        svn_error_t *svn_err__temp = svn_error__malfunction(!0,"stream.c",143,"stream->read_fn != NULL");
        if (svn_err__temp) {
          return svn_err__temp;
        }
      }while (0);
    }
  }while (0);
  return (stream -> read_fn)(stream -> baton,buffer,len);
}

svn_error_t *svn_stream_skip(svn_stream_t *stream,apr_size_t len)
{
  if (stream -> skip_fn == ((void *)0)) {
    return skip_default_handler(stream -> baton,len,stream -> read_fn);
  }
  return (stream -> skip_fn)(stream -> baton,len);
}

svn_error_t *svn_stream_write(svn_stream_t *stream,const char *data,apr_size_t *len)
{
  do {
    if (!(stream -> write_fn != ((void *)0))) {
      do {
        svn_error_t *svn_err__temp = svn_error__malfunction(!0,"stream.c",162,"stream->write_fn != NULL");
        if (svn_err__temp) {
          return svn_err__temp;
        }
      }while (0);
    }
  }while (0);
  return (stream -> write_fn)(stream -> baton,data,len);
}

svn_error_t *svn_stream_reset(svn_stream_t *stream)
{
  return svn_stream_seek(stream,((void *)0));
}

svn_boolean_t svn_stream_supports_mark(svn_stream_t *stream)
{
  return stream -> mark_fn != ((void *)0);
}

svn_error_t *svn_stream_mark(svn_stream_t *stream,svn_stream_mark_t **mark,apr_pool_t *pool)
{
  if (stream -> mark_fn == ((void *)0)) {
    return svn_error_create(SVN_ERR_STREAM_SEEK_NOT_SUPPORTED,((void *)0),((void *)0));
  }
  return (stream -> mark_fn)(stream -> baton,mark,pool);
}

svn_error_t *svn_stream_seek(svn_stream_t *stream,const svn_stream_mark_t *mark)
{
  if (stream -> seek_fn == ((void *)0)) {
    return svn_error_create(SVN_ERR_STREAM_SEEK_NOT_SUPPORTED,((void *)0),((void *)0));
  }
  return (stream -> seek_fn)(stream -> baton,mark);
}

svn_boolean_t svn_stream__is_buffered(svn_stream_t *stream)
{
  if (stream -> is_buffered_fn == ((void *)0)) {
    return 0;
  }
  return (stream -> is_buffered_fn)(stream -> baton);
}

svn_error_t *svn_stream_close(svn_stream_t *stream)
{
  if (stream -> close_fn == ((void *)0)) {
    return 0;
  }
  return (stream -> close_fn)(stream -> baton);
}

svn_error_t *svn_stream_puts(svn_stream_t *stream,const char *str)
{
  apr_size_t len;
  len = strlen(str);
  return svn_stream_write(stream,str,&len);
}

svn_error_t *svn_stream_printf(svn_stream_t *stream,apr_pool_t *pool,const char *fmt,... )
{
  const char *message;
  va_list ap;
  __builtin_va_start(ap,fmt);
  message = (apr_pvsprintf(pool,fmt,ap));
  __builtin_va_end(ap);
  return svn_stream_puts(stream,message);
}

svn_error_t *svn_stream_printf_from_utf8(svn_stream_t *stream,const char *encoding,apr_pool_t *pool,const char *fmt,... )
{
  const char *message;
  const char *translated;
  va_list ap;
  __builtin_va_start(ap,fmt);
  message = (apr_pvsprintf(pool,fmt,ap));
  __builtin_va_end(ap);
  do {
    svn_error_t *svn_err__temp = svn_utf_cstring_from_utf8_ex2(&translated,message,encoding,pool);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  return svn_stream_puts(stream,translated);
}
/* Size that 90% of the lines we encounter will be not longer than.
   used by stream_readline_bytewise() and stream_readline_chunky().
 */
#define LINE_CHUNK_SIZE 80
/* Guts of svn_stream_readline().
 * Returns the line read from STREAM in *STRINGBUF, and indicates
 * end-of-file in *EOF.  If DETECT_EOL is TRUE, the end-of-line indicator
 * is detected automatically and returned in *EOL.
 * If DETECT_EOL is FALSE, *EOL must point to the desired end-of-line
 * indicator.  STRINGBUF is allocated in POOL. */

static svn_error_t *stream_readline_bytewise(svn_stringbuf_t **stringbuf,svn_boolean_t *eof,const char *eol,svn_stream_t *stream,apr_pool_t *pool)
{
  svn_stringbuf_t *str;
  apr_size_t numbytes;
  const char *match;
  char c;
/* Since we're reading one character at a time, let's at least
     optimize for the 90% case.  90% of the time, we can avoid the
     stringbuf ever having to realloc() itself if we start it out at
     80 chars.  */
  str = svn_stringbuf_create_ensure(80,pool);
/* Read into STR up to and including the next EOL sequence. */
  match = eol;
  while( *match){
    numbytes = 1;
    do {
      svn_error_t *svn_err__temp = svn_stream_read(stream,&c,&numbytes);
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
    if (numbytes != 1) {
/* a 'short' read means the stream has run out. */
       *eof = !0;
       *stringbuf = str;
      return 0;
    }
    if (c == ( *match)) {
      match++;
    }
    else {
      match = eol;
    }
    svn_stringbuf_appendbyte(str,c);
  }
   *eof = 0;
  svn_stringbuf_chop(str,(match - eol));
   *stringbuf = str;
  return 0;
}

static svn_error_t *stream_readline_chunky(svn_stringbuf_t **stringbuf,svn_boolean_t *eof,const char *eol,svn_stream_t *stream,apr_pool_t *pool)
{
/* Read larger chunks of data at once into this buffer and scan
   * that for EOL. A good chunk size should be about 80 chars since
   * most text lines will be shorter. However, don't use a much
   * larger value because filling the buffer from the stream takes
   * time as well.
   */
  char buffer[80 + 1];
/* variables */
  svn_stream_mark_t *mark;
  apr_size_t numbytes;
  const char *eol_pos;
  apr_size_t total_parsed = 0;
/* invariant for this call */
  const size_t eol_len = strlen(eol);
/* Remember the line start so this plus the line length will be
   * the position to move to at the end of this function.
   */
  do {
    svn_error_t *svn_err__temp = svn_stream_mark(stream,&mark,pool);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
/* Read the first chunk. */
  numbytes = 80;
  do {
    svn_error_t *svn_err__temp = svn_stream_read(stream,buffer,&numbytes);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  buffer[numbytes] = '\0';
/* Look for the EOL in this first chunk. If we find it, we are done here.
   */
  eol_pos = (strstr(buffer,eol));
  if (eol_pos != ((void *)0)) {
     *stringbuf = svn_stringbuf_ncreate(buffer,(eol_pos - buffer),pool);
    total_parsed = (eol_pos - buffer) + eol_len;
  }
  else {
    if (numbytes < 80) {
/* We hit EOF but not EOL.
       */
       *stringbuf = svn_stringbuf_ncreate(buffer,numbytes,pool);
       *eof = !0;
      return 0;
    }
    else {
/* A larger buffer for the string is needed. */
      svn_stringbuf_t *str;
      str = svn_stringbuf_create_ensure((2 * 80),pool);
      svn_stringbuf_appendbytes(str,buffer,numbytes);
       *stringbuf = str;
/* Loop reading chunks until an EOL was found. If we hit EOF, fall
       * back to the standard implementation. */
      do {
/* Append the next chunk to the string read so far.
         */
        svn_stringbuf_ensure(str,str -> len + 80);
        numbytes = 80;
        do {
          svn_error_t *svn_err__temp = svn_stream_read(stream,str -> data + str -> len,&numbytes);
          if (svn_err__temp) {
            return svn_err__temp;
          }
        }while (0);
        str -> len += numbytes;
        str -> data[str -> len] = '\0';
/* Look for the EOL in the new data plus the last part of the
         * previous chunk because the EOL may span over the boundary
         * between both chunks.
         */
        eol_pos = (strstr((str -> data + str -> len - numbytes - (eol_len - 1)),eol));
        if (numbytes < 80 && eol_pos == ((void *)0)) {
/* We hit EOF instead of EOL. */
           *eof = !0;
          return 0;
        }
      }while (eol_pos == ((void *)0));
/* Number of bytes we actually consumed (i.e. line + EOF).
       * We need to "return" the rest to the stream by moving its
       * read pointer.
       */
      total_parsed = (eol_pos - (str -> data)) + eol_len;
/* Terminate the string at the EOL postion and return it. */
      str -> len = (eol_pos - (str -> data));
      str -> data[str -> len] = 0;
    }
  }
/* Move the stream read pointer to the first position behind the EOL.
   */
  do {
    svn_error_t *svn_err__temp = svn_stream_seek(stream,mark);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  return svn_stream_skip(stream,total_parsed);
}
/* Guts of svn_stream_readline().
 * Returns the line read from STREAM in *STRINGBUF, and indicates
 * end-of-file in *EOF.  EOL must point to the desired end-of-line
 * indicator.  STRINGBUF is allocated in POOL. */

static svn_error_t *stream_readline(svn_stringbuf_t **stringbuf,svn_boolean_t *eof,const char *eol,svn_stream_t *stream,apr_pool_t *pool)
{
   *eof = 0;
/* Often, we operate on APR file or string-based streams and know what
   * EOL we are looking for. Optimize that common case.
   */
  if (svn_stream_supports_mark(stream) && svn_stream__is_buffered(stream)) {
/* We can efficiently read chunks speculatively and reposition the
       * stream pointer to the end of the line once we found that.
       */
    do {
      svn_error_t *svn_err__temp = stream_readline_chunky(stringbuf,eof,eol,stream,pool);
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
  }
  else {
/* Use the standard byte-byte implementation.
       */
    do {
      svn_error_t *svn_err__temp = stream_readline_bytewise(stringbuf,eof,eol,stream,pool);
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
  }
  return 0;
}

svn_error_t *svn_stream_readline(svn_stream_t *stream,svn_stringbuf_t **stringbuf,const char *eol,svn_boolean_t *eof,apr_pool_t *pool)
{
  return stream_readline(stringbuf,eof,eol,stream,pool);
}

svn_error_t *svn_stream_copy3(svn_stream_t *from,svn_stream_t *to,svn_cancel_func_t cancel_func,void *cancel_baton,apr_pool_t *scratch_pool)
{
  char *buf = (apr_palloc(scratch_pool,16384));
  svn_error_t *err;
  svn_error_t *err2;
/* Read and write chunks until we get a short read, indicating the
     end of the stream.  (We can't get a short write without an
     associated error.) */
  while(1){
    apr_size_t len = 16384;
    if (cancel_func) {
      err = cancel_func(cancel_baton);
      if (err) {
        break; 
      }
    }
    err = svn_stream_read(from,buf,&len);
    if (err) {
      break; 
    }
    if (len > 0) {
      err = svn_stream_write(to,buf,&len);
    }
    if (err || len != 16384) {
      break; 
    }
  }
  err2 = svn_error_compose_create(svn_stream_close(from),svn_stream_close(to));
  return svn_error_compose_create(err,err2);
}

svn_error_t *svn_stream_contents_same2(svn_boolean_t *same,svn_stream_t *stream1,svn_stream_t *stream2,apr_pool_t *pool)
{
  char *buf1 = (apr_palloc(pool,16384));
  char *buf2 = (apr_palloc(pool,16384));
  apr_size_t bytes_read1 = 16384;
  apr_size_t bytes_read2 = 16384;
  svn_error_t *err = ((void *)0);
/* assume TRUE, until disproved below */
   *same = !0;
  while(bytes_read1 == 16384 && bytes_read2 == 16384){
    err = svn_stream_read(stream1,buf1,&bytes_read1);
    if (err) {
      break; 
    }
    err = svn_stream_read(stream2,buf2,&bytes_read2);
    if (err) {
      break; 
    }
    if (bytes_read1 != bytes_read2 || memcmp(buf1,buf2,bytes_read1)) {
       *same = 0;
      break; 
    }
  }
  return svn_error_compose_create(err,svn_error_compose_create(svn_stream_close(stream1),svn_stream_close(stream2)));
}
/*** Stream implementation utilities ***/
/* Skip data from any stream by reading and simply discarding it. */

static svn_error_t *skip_default_handler(void *baton,apr_size_t len,svn_read_fn_t read_fn)
{
  apr_size_t bytes_read = 1;
  char buffer[4096];
  apr_size_t to_read = len;
  while(to_read > 0 && bytes_read > 0){
    bytes_read = (sizeof(buffer) < to_read?sizeof(buffer) : to_read);
    do {
      svn_error_t *svn_err__temp = read_fn(baton,buffer,&bytes_read);
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
    to_read -= bytes_read;
  }
  return 0;
}
/*** Generic readable empty stream ***/

static svn_error_t *read_handler_empty(void *baton,char *buffer,apr_size_t *len)
{
   *len = 0;
  return 0;
}

static svn_error_t *write_handler_empty(void *baton,const char *data,apr_size_t *len)
{
  return 0;
}

static svn_error_t *mark_handler_empty(void *baton,svn_stream_mark_t **mark,apr_pool_t *pool)
{
/* Seek to start of stream marker */
   *mark = ((void *)0);
  return 0;
}

static svn_error_t *seek_handler_empty(void *baton,const svn_stream_mark_t *mark)
{
  return 0;
}

static svn_boolean_t is_buffered_handler_empty(void *baton)
{
  return 0;
}

svn_stream_t *svn_stream_empty(apr_pool_t *pool)
{
  svn_stream_t *stream;
  stream = svn_stream_create(((void *)0),pool);
  svn_stream_set_read(stream,read_handler_empty);
  svn_stream_set_write(stream,write_handler_empty);
  svn_stream_set_mark(stream,mark_handler_empty);
  svn_stream_set_seek(stream,seek_handler_empty);
  svn_stream__set_is_buffered(stream,is_buffered_handler_empty);
  return stream;
}
/*** Stream duplication support ***/

struct baton_tee 
{
  svn_stream_t *out1;
  svn_stream_t *out2;
}
;

static svn_error_t *write_handler_tee(void *baton,const char *data,apr_size_t *len)
{
  struct baton_tee *bt = baton;
  do {
    svn_error_t *svn_err__temp = svn_stream_write(bt -> out1,data,len);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  do {
    svn_error_t *svn_err__temp = svn_stream_write(bt -> out2,data,len);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  return 0;
}

static svn_error_t *close_handler_tee(void *baton)
{
  struct baton_tee *bt = baton;
  do {
    svn_error_t *svn_err__temp = svn_stream_close(bt -> out1);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  do {
    svn_error_t *svn_err__temp = svn_stream_close(bt -> out2);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  return 0;
}

svn_stream_t *svn_stream_tee(svn_stream_t *out1,svn_stream_t *out2,apr_pool_t *pool)
{
  struct baton_tee *baton;
  svn_stream_t *stream;
  if (out1 == ((void *)0)) {
    return out2;
  }
  if (out2 == ((void *)0)) {
    return out1;
  }
  baton = (apr_palloc(pool,sizeof(( *baton))));
  baton -> out1 = out1;
  baton -> out2 = out2;
  stream = svn_stream_create(baton,pool);
  svn_stream_set_write(stream,write_handler_tee);
  svn_stream_set_close(stream,close_handler_tee);
  return stream;
}
/*** Ownership detaching stream ***/

static svn_error_t *read_handler_disown(void *baton,char *buffer,apr_size_t *len)
{
  return svn_stream_read(baton,buffer,len);
}

static svn_error_t *skip_handler_disown(void *baton,apr_size_t len)
{
  return svn_stream_skip(baton,len);
}

static svn_error_t *write_handler_disown(void *baton,const char *buffer,apr_size_t *len)
{
  return svn_stream_write(baton,buffer,len);
}

static svn_error_t *mark_handler_disown(void *baton,svn_stream_mark_t **mark,apr_pool_t *pool)
{
  return svn_stream_mark(baton,mark,pool);
}

static svn_error_t *seek_handler_disown(void *baton,const svn_stream_mark_t *mark)
{
  return svn_stream_seek(baton,mark);
}

static svn_boolean_t is_buffered_handler_disown(void *baton)
{
  return svn_stream__is_buffered(baton);
}

svn_stream_t *svn_stream_disown(svn_stream_t *stream,apr_pool_t *pool)
{
  svn_stream_t *s = svn_stream_create(stream,pool);
  svn_stream_set_read(s,read_handler_disown);
  svn_stream_set_skip(s,skip_handler_disown);
  svn_stream_set_write(s,write_handler_disown);
  svn_stream_set_mark(s,mark_handler_disown);
  svn_stream_set_seek(s,seek_handler_disown);
  svn_stream__set_is_buffered(s,is_buffered_handler_disown);
  return s;
}
/*** Generic stream for APR files ***/

struct baton_apr 
{
  apr_file_t *file;
  apr_pool_t *pool;
}
;
/* svn_stream_mark_t for streams backed by APR files. */

struct mark_apr 
{
  apr_off_t off;
}
;

static svn_error_t *read_handler_apr(void *baton,char *buffer,apr_size_t *len)
{
  struct baton_apr *btn = baton;
  svn_error_t *err;
  svn_boolean_t eof;
  if ( *len == 1) {
    err = svn_io_file_getc(buffer,btn -> file,btn -> pool);
    if (err) {
       *len = 0;
      if (err -> apr_err == 20000 + 50000 + 14) {
        svn_error_clear(err);
        err = 0;
      }
    }
  }
  else {
    err = svn_io_file_read_full2(btn -> file,buffer, *len,len,&eof,btn -> pool);
  }
  return err;
}

static svn_error_t *skip_handler_apr(void *baton,apr_size_t len)
{
  struct baton_apr *btn = baton;
  apr_off_t offset = len;
  return svn_io_file_seek(btn -> file,1,&offset,btn -> pool);
}

static svn_error_t *write_handler_apr(void *baton,const char *data,apr_size_t *len)
{
  struct baton_apr *btn = baton;
  svn_error_t *err;
  if ( *len == 1) {
    err = svn_io_file_putc( *data,btn -> file,btn -> pool);
    if (err) {
       *len = 0;
    }
  }
  else {
    err = svn_io_file_write_full(btn -> file,data, *len,len,btn -> pool);
  }
  return err;
}

static svn_error_t *close_handler_apr(void *baton)
{
  struct baton_apr *btn = baton;
  return svn_io_file_close(btn -> file,btn -> pool);
}

static svn_error_t *mark_handler_apr(void *baton,svn_stream_mark_t **mark,apr_pool_t *pool)
{
  struct baton_apr *btn = baton;
  struct mark_apr *mark_apr;
  mark_apr = (apr_palloc(pool,sizeof(( *mark_apr))));
  mark_apr -> off = 0;
  do {
    svn_error_t *svn_err__temp = svn_io_file_seek(btn -> file,1,&mark_apr -> off,btn -> pool);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
   *mark = ((svn_stream_mark_t *)mark_apr);
  return 0;
}

static svn_error_t *seek_handler_apr(void *baton,const svn_stream_mark_t *mark)
{
  struct baton_apr *btn = baton;
  apr_off_t offset = mark != ((void *)0)?((const struct mark_apr *)mark) -> off : 0;
  do {
    svn_error_t *svn_err__temp = svn_io_file_seek(btn -> file,0,&offset,btn -> pool);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  return 0;
}

static svn_boolean_t is_buffered_handler_apr(void *baton)
{
  struct baton_apr *btn = baton;
  return (apr_file_flags_get(btn -> file) & 0x00080) != 0;
}

svn_error_t *svn_stream_open_readonly(svn_stream_t **stream,const char *path,apr_pool_t *result_pool,apr_pool_t *scratch_pool)
{
  apr_file_t *file;
  do {
    svn_error_t *svn_err__temp = svn_io_file_open(&file,path,0x00001 | 0x00080,0x0FFF,result_pool);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
   *stream = svn_stream_from_aprfile2(file,0,result_pool);
  return 0;
}

svn_error_t *svn_stream_open_writable(svn_stream_t **stream,const char *path,apr_pool_t *result_pool,apr_pool_t *scratch_pool)
{
  apr_file_t *file;
  do {
    svn_error_t *svn_err__temp = svn_io_file_open(&file,path,0x00002 | 0x00080 | 0x00004 | 0x00040,0x0FFF,result_pool);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
   *stream = svn_stream_from_aprfile2(file,0,result_pool);
  return 0;
}

svn_error_t *svn_stream_open_unique(svn_stream_t **stream,const char **temp_path,const char *dirpath,svn_io_file_del_t delete_when,apr_pool_t *result_pool,apr_pool_t *scratch_pool)
{
  apr_file_t *file;
  do {
    svn_error_t *svn_err__temp = svn_io_open_unique_file3(&file,temp_path,dirpath,delete_when,result_pool,scratch_pool);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
   *stream = svn_stream_from_aprfile2(file,0,result_pool);
  return 0;
}

svn_stream_t *svn_stream_from_aprfile2(apr_file_t *file,svn_boolean_t disown,apr_pool_t *pool)
{
  struct baton_apr *baton;
  svn_stream_t *stream;
  if (file == ((void *)0)) {
    return svn_stream_empty(pool);
  }
  baton = (apr_palloc(pool,sizeof(( *baton))));
  baton -> file = file;
  baton -> pool = pool;
  stream = svn_stream_create(baton,pool);
  svn_stream_set_read(stream,read_handler_apr);
  svn_stream_set_write(stream,write_handler_apr);
  svn_stream_set_skip(stream,skip_handler_apr);
  svn_stream_set_mark(stream,mark_handler_apr);
  svn_stream_set_seek(stream,seek_handler_apr);
  svn_stream__set_is_buffered(stream,is_buffered_handler_apr);
  stream -> file = file;
  if (!disown) {
    svn_stream_set_close(stream,close_handler_apr);
  }
  return stream;
}

apr_file_t *svn_stream__aprfile(svn_stream_t *stream)
{
  return stream -> file;
}
/* Compressed stream support */
#define ZBUFFER_SIZE 4096       /* The size of the buffer the
                                   compressed stream uses to read from
                                   the substream. Basically an
                                   arbitrary value, picked to be about
                                   page-sized. */

struct zbaton 
{
/* compressed stream for reading */
  z_stream *in;
/* compressed stream for writing */
  z_stream *out;
/* substream's read function */
  svn_read_fn_t read;
/* substream's write function */
  svn_write_fn_t write;
/* substream's close function */
  svn_close_fn_t close;
/* buffer   used   for  reading   from
                                   substream */
  void *read_buffer;
/* what flush mode to use while
                                   reading */
  int read_flush;
/* The pool this baton is allocated
                                   on */
  apr_pool_t *pool;
/* The substream's baton */
  void *subbaton;
}
;
/* zlib alloc function. opaque is the pool we need. */

static voidpf zalloc(voidpf opaque,uInt items,uInt size)
{
  apr_pool_t *pool = opaque;
  return apr_palloc(pool,(items * size));
}
/* zlib free function */

static void zfree(voidpf opaque,voidpf address)
{
/* Empty, since we allocate on the pool */
}
/* Helper function to figure out the sync mode */

static svn_error_t *read_helper_gz(svn_read_fn_t read_fn,void *baton,char *buffer,uInt *len,int *zflush)
{
  uInt orig_len =  *len;
/* There's no reason this value should grow bigger than the range of
     uInt, but Subversion's API requires apr_size_t. */
  apr_size_t apr_len = (apr_size_t )( *len);
  do {
    svn_error_t *svn_err__temp = ( *read_fn)(baton,buffer,&apr_len);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
/* Type cast back to uInt type that zlib uses.  On LP64 platforms
     apr_size_t will be bigger than uInt. */
   *len = ((uInt )apr_len);
/* I wanted to use Z_FINISH here, but we need to know our buffer is
     big enough */
   *zflush = ( *len < orig_len?2 : 2);
  return 0;
}
/* Handle reading from a compressed stream */

static svn_error_t *read_handler_gz(void *baton,char *buffer,apr_size_t *len)
{
  struct zbaton *btn = baton;
  int zerr;
  if (btn -> in == ((void *)0)) {
    btn -> in = (apr_palloc(btn -> pool,sizeof(z_stream )));
    btn -> in -> zalloc = zalloc;
    btn -> in -> zfree = zfree;
    btn -> in -> opaque = (btn -> pool);
    btn -> read_buffer = apr_palloc(btn -> pool,4096);
    btn -> in -> next_in = (btn -> read_buffer);
    btn -> in -> avail_in = 4096;
    do {
      svn_error_t *svn_err__temp = read_helper_gz(btn -> read,btn -> subbaton,(btn -> read_buffer),&btn -> in -> avail_in,&btn -> read_flush);
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
    zerr = inflateInit_(btn -> in,"1.2.8",((int )(sizeof(z_stream ))));
    do {
      svn_error_t *svn_err__temp = svn_error__wrap_zlib(zerr,"inflateInit",(btn -> in -> msg));
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
  }
  btn -> in -> next_out = ((Bytef *)buffer);
  btn -> in -> avail_out = ((uInt )( *len));
  while(btn -> in -> avail_out > 0){
    if (btn -> in -> avail_in <= 0) {
      btn -> in -> avail_in = 4096;
      btn -> in -> next_in = (btn -> read_buffer);
      do {
        svn_error_t *svn_err__temp = read_helper_gz(btn -> read,btn -> subbaton,(btn -> read_buffer),&btn -> in -> avail_in,&btn -> read_flush);
        if (svn_err__temp) {
          return svn_err__temp;
        }
      }while (0);
    }
/* Short read means underlying stream has run out. */
    if (btn -> in -> avail_in == 0) {
       *len = 0;
      return 0;
    }
    zerr = inflate(btn -> in,btn -> read_flush);
    if (zerr == 1) {
      break; 
    }
    else {
      if (zerr != 0) {
        return svn_error__wrap_zlib(zerr,"inflate",(btn -> in -> msg));
      }
    }
  }
   *len -= (btn -> in -> avail_out);
  return 0;
}
/* Compress data and write it to the substream */

static svn_error_t *write_handler_gz(void *baton,const char *buffer,apr_size_t *len)
{
  struct zbaton *btn = baton;
  apr_pool_t *subpool;
  void *write_buf;
  apr_size_t buf_size;
  apr_size_t write_len;
  int zerr;
  if (btn -> out == ((void *)0)) {
    btn -> out = (apr_palloc(btn -> pool,sizeof(z_stream )));
    btn -> out -> zalloc = zalloc;
    btn -> out -> zfree = zfree;
    btn -> out -> opaque = (btn -> pool);
    zerr = deflateInit_(btn -> out,- 1,"1.2.8",((int )(sizeof(z_stream ))));
    do {
      svn_error_t *svn_err__temp = svn_error__wrap_zlib(zerr,"deflateInit",(btn -> out -> msg));
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
  }
/* The largest buffer we should need is 0.1% larger than the
     compressed data, + 12 bytes. This info comes from zlib.h.  */
  buf_size =  *len +  *len / 1000 + 13;
  subpool = svn_pool_create_ex(btn -> pool,((void *)0));
  write_buf = apr_palloc(subpool,buf_size);
/* Casting away const! */
  btn -> out -> next_in = ((Bytef *)buffer);
  btn -> out -> avail_in = ((uInt )( *len));
  while(btn -> out -> avail_in > 0){
    btn -> out -> next_out = write_buf;
    btn -> out -> avail_out = ((uInt )buf_size);
    zerr = deflate(btn -> out,0);
    do {
      svn_error_t *svn_err__temp = svn_error__wrap_zlib(zerr,"deflate",(btn -> out -> msg));
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
    write_len = buf_size - (btn -> out -> avail_out);
    if (write_len > 0) {
      do {
        svn_error_t *svn_err__temp = (btn -> write)(btn -> subbaton,write_buf,&write_len);
        if (svn_err__temp) {
          return svn_err__temp;
        }
      }while (0);
    }
  }
  apr_pool_destroy(subpool);
  return 0;
}
/* Handle flushing and closing the stream */

static svn_error_t *close_handler_gz(void *baton)
{
  struct zbaton *btn = baton;
  int zerr;
  if (btn -> in != ((void *)0)) {
    zerr = inflateEnd(btn -> in);
    do {
      svn_error_t *svn_err__temp = svn_error__wrap_zlib(zerr,"inflateEnd",(btn -> in -> msg));
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
  }
  if (btn -> out != ((void *)0)) {
    void *buf;
    apr_size_t write_len;
    buf = apr_palloc(btn -> pool,4096);
    while(!0){
      btn -> out -> next_out = buf;
      btn -> out -> avail_out = 4096;
      zerr = deflate(btn -> out,4);
      if (zerr != 1 && zerr != 0) {
        return svn_error__wrap_zlib(zerr,"deflate",(btn -> out -> msg));
      }
      write_len = (4096 - btn -> out -> avail_out);
      if (write_len > 0) {
        do {
          svn_error_t *svn_err__temp = (btn -> write)(btn -> subbaton,buf,&write_len);
          if (svn_err__temp) {
            return svn_err__temp;
          }
        }while (0);
      }
      if (zerr == 1) {
        break; 
      }
    }
    zerr = deflateEnd(btn -> out);
    do {
      svn_error_t *svn_err__temp = svn_error__wrap_zlib(zerr,"deflateEnd",(btn -> out -> msg));
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
  }
  if (btn -> close != ((void *)0)) {
    return (btn -> close)(btn -> subbaton);
  }
  else {
    return 0;
  }
}

svn_stream_t *svn_stream_compressed(svn_stream_t *stream,apr_pool_t *pool)
{
  struct svn_stream_t *zstream;
  struct zbaton *baton;
  stream != ((void *)0)?((void )0) : __assert_fail("stream != ((void *)0)","stream.c",1154,__PRETTY_FUNCTION__);
  baton = (apr_palloc(pool,sizeof(( *baton))));
  baton -> in = baton -> out = ((void *)0);
  baton -> read = stream -> read_fn;
  baton -> write = stream -> write_fn;
  baton -> close = stream -> close_fn;
  baton -> subbaton = stream -> baton;
  baton -> pool = pool;
  baton -> read_buffer = ((void *)0);
  baton -> read_flush = 2;
  zstream = svn_stream_create(baton,pool);
  svn_stream_set_read(zstream,read_handler_gz);
  svn_stream_set_write(zstream,write_handler_gz);
  svn_stream_set_close(zstream,close_handler_gz);
  return zstream;
}
/* Checksummed stream support */

struct checksum_stream_baton 
{
  svn_checksum_ctx_t *read_ctx;
  svn_checksum_ctx_t *write_ctx;
/* Output value. */
  svn_checksum_t **read_checksum;
/* Output value. */
  svn_checksum_t **write_checksum;
  svn_stream_t *proxy;
/* True if more data should be read when closing the stream. */
  svn_boolean_t read_more;
/* Pool to allocate read buffer and output values from. */
  apr_pool_t *pool;
}
;

static svn_error_t *read_handler_checksum(void *baton,char *buffer,apr_size_t *len)
{
  struct checksum_stream_baton *btn = baton;
  apr_size_t saved_len =  *len;
  do {
    svn_error_t *svn_err__temp = svn_stream_read(btn -> proxy,buffer,len);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  if (btn -> read_checksum) {
    do {
      svn_error_t *svn_err__temp = svn_checksum_update(btn -> read_ctx,buffer, *len);
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
  }
  if (saved_len !=  *len) {
    btn -> read_more = 0;
  }
  return 0;
}

static svn_error_t *write_handler_checksum(void *baton,const char *buffer,apr_size_t *len)
{
  struct checksum_stream_baton *btn = baton;
  if (btn -> write_checksum &&  *len > 0) {
    do {
      svn_error_t *svn_err__temp = svn_checksum_update(btn -> write_ctx,buffer, *len);
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
  }
  return svn_stream_write(btn -> proxy,buffer,len);
}

static svn_error_t *close_handler_checksum(void *baton)
{
  struct checksum_stream_baton *btn = baton;
/* If we're supposed to drain the stream, do so before finalizing the
     checksum. */
  if (btn -> read_more) {
    char *buf = (apr_palloc(btn -> pool,16384));
    apr_size_t len = 16384;
    do {
      do {
        svn_error_t *svn_err__temp = read_handler_checksum(baton,buf,&len);
        if (svn_err__temp) {
          return svn_err__temp;
        }
      }while (0);
    }while (btn -> read_more);
  }
  if (btn -> read_ctx) {
    do {
      svn_error_t *svn_err__temp = svn_checksum_final(btn -> read_checksum,(btn -> read_ctx),btn -> pool);
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
  }
  if (btn -> write_ctx) {
    do {
      svn_error_t *svn_err__temp = svn_checksum_final(btn -> write_checksum,(btn -> write_ctx),btn -> pool);
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
  }
  return svn_stream_close(btn -> proxy);
}

svn_stream_t *svn_stream_checksummed2(svn_stream_t *stream,svn_checksum_t **read_checksum,svn_checksum_t **write_checksum,svn_checksum_kind_t checksum_kind,svn_boolean_t read_all,apr_pool_t *pool)
{
  svn_stream_t *s;
  struct checksum_stream_baton *baton;
  if (read_checksum == ((void *)0) && write_checksum == ((void *)0)) {
    return stream;
  }
  baton = (apr_palloc(pool,sizeof(( *baton))));
  if (read_checksum) {
    baton -> read_ctx = svn_checksum_ctx_create(checksum_kind,pool);
  }
  else {
    baton -> read_ctx = ((void *)0);
  }
  if (write_checksum) {
    baton -> write_ctx = svn_checksum_ctx_create(checksum_kind,pool);
  }
  else {
    baton -> write_ctx = ((void *)0);
  }
  baton -> read_checksum = read_checksum;
  baton -> write_checksum = write_checksum;
  baton -> proxy = stream;
  baton -> read_more = read_all;
  baton -> pool = pool;
  s = svn_stream_create(baton,pool);
  svn_stream_set_read(s,read_handler_checksum);
  svn_stream_set_write(s,write_handler_checksum);
  svn_stream_set_close(s,close_handler_checksum);
  return s;
}

struct md5_stream_baton 
{
  const unsigned char **read_digest;
  const unsigned char **write_digest;
  svn_checksum_t *read_checksum;
  svn_checksum_t *write_checksum;
  svn_stream_t *proxy;
  apr_pool_t *pool;
}
;

static svn_error_t *read_handler_md5(void *baton,char *buffer,apr_size_t *len)
{
  struct md5_stream_baton *btn = baton;
  return svn_stream_read(btn -> proxy,buffer,len);
}

static svn_error_t *skip_handler_md5(void *baton,apr_size_t len)
{
  struct md5_stream_baton *btn = baton;
  return svn_stream_skip(btn -> proxy,len);
}

static svn_error_t *write_handler_md5(void *baton,const char *buffer,apr_size_t *len)
{
  struct md5_stream_baton *btn = baton;
  return svn_stream_write(btn -> proxy,buffer,len);
}

static svn_error_t *close_handler_md5(void *baton)
{
  struct md5_stream_baton *btn = baton;
  do {
    svn_error_t *svn_err__temp = svn_stream_close(btn -> proxy);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  if (btn -> read_digest) {
     *btn -> read_digest = (apr_pmemdup(btn -> pool,(btn -> read_checksum -> digest),16));
  }
  if (btn -> write_digest) {
     *btn -> write_digest = (apr_pmemdup(btn -> pool,(btn -> write_checksum -> digest),16));
  }
  return 0;
}

svn_stream_t *svn_stream_checksummed(svn_stream_t *stream,const unsigned char **read_digest,const unsigned char **write_digest,svn_boolean_t read_all,apr_pool_t *pool)
{
  svn_stream_t *s;
  struct md5_stream_baton *baton;
  if (!read_digest && !write_digest) {
    return stream;
  }
  baton = (apr_palloc(pool,sizeof(( *baton))));
  baton -> read_digest = read_digest;
  baton -> write_digest = write_digest;
  baton -> pool = pool;
/* Set BATON->proxy to a stream that will fill in BATON->read_checksum
   * and BATON->write_checksum (if we want them) when it is closed. */
  baton -> proxy = svn_stream_checksummed2(stream,(read_digest?&baton -> read_checksum : ((void *)0)),(write_digest?&baton -> write_checksum : ((void *)0)),svn_checksum_md5,read_all,pool);
/* Create a stream that will forward its read/write/close operations to
   * BATON->proxy and will fill in *READ_DIGEST and *WRITE_DIGEST (if we
   * want them) after it closes BATON->proxy. */
  s = svn_stream_create(baton,pool);
  svn_stream_set_read(s,read_handler_md5);
  svn_stream_set_skip(s,skip_handler_md5);
  svn_stream_set_write(s,write_handler_md5);
  svn_stream_set_close(s,close_handler_md5);
  return s;
}
/* Miscellaneous stream functions. */

struct stringbuf_stream_baton 
{
  svn_stringbuf_t *str;
  apr_size_t amt_read;
}
;
/* svn_stream_mark_t for streams backed by stringbufs. */

struct stringbuf_stream_mark 
{
  apr_size_t pos;
}
;

static svn_error_t *read_handler_stringbuf(void *baton,char *buffer,apr_size_t *len)
{
  struct stringbuf_stream_baton *btn = baton;
  apr_size_t left_to_read = btn -> str -> len - btn -> amt_read;
   *len = ( *len > left_to_read?left_to_read :  *len);
  memcpy(buffer,(btn -> str -> data + btn -> amt_read), *len);
  btn -> amt_read +=  *len;
  return 0;
}

static svn_error_t *skip_handler_stringbuf(void *baton,apr_size_t len)
{
  struct stringbuf_stream_baton *btn = baton;
  apr_size_t left_to_read = btn -> str -> len - btn -> amt_read;
  len = (len > left_to_read?left_to_read : len);
  btn -> amt_read += len;
  return 0;
}

static svn_error_t *write_handler_stringbuf(void *baton,const char *data,apr_size_t *len)
{
  struct stringbuf_stream_baton *btn = baton;
  svn_stringbuf_appendbytes(btn -> str,data, *len);
  return 0;
}

static svn_error_t *mark_handler_stringbuf(void *baton,svn_stream_mark_t **mark,apr_pool_t *pool)
{
  struct stringbuf_stream_baton *btn;
  struct stringbuf_stream_mark *stringbuf_stream_mark;
  btn = baton;
  stringbuf_stream_mark = (apr_palloc(pool,sizeof(( *stringbuf_stream_mark))));
  stringbuf_stream_mark -> pos = btn -> amt_read;
   *mark = ((svn_stream_mark_t *)stringbuf_stream_mark);
  return 0;
}

static svn_error_t *seek_handler_stringbuf(void *baton,const svn_stream_mark_t *mark)
{
  struct stringbuf_stream_baton *btn = baton;
  if (mark != ((void *)0)) {
    const struct stringbuf_stream_mark *stringbuf_stream_mark;
    stringbuf_stream_mark = ((const struct stringbuf_stream_mark *)mark);
    btn -> amt_read = stringbuf_stream_mark -> pos;
  }
  else {
    btn -> amt_read = 0;
  }
  return 0;
}

static svn_boolean_t is_buffered_handler_stringbuf(void *baton)
{
  return !0;
}

svn_stream_t *svn_stream_from_stringbuf(svn_stringbuf_t *str,apr_pool_t *pool)
{
  svn_stream_t *stream;
  struct stringbuf_stream_baton *baton;
  if (!str) {
    return svn_stream_empty(pool);
  }
  baton = (apr_palloc(pool,sizeof(( *baton))));
  baton -> str = str;
  baton -> amt_read = 0;
  stream = svn_stream_create(baton,pool);
  svn_stream_set_read(stream,read_handler_stringbuf);
  svn_stream_set_skip(stream,skip_handler_stringbuf);
  svn_stream_set_write(stream,write_handler_stringbuf);
  svn_stream_set_mark(stream,mark_handler_stringbuf);
  svn_stream_set_seek(stream,seek_handler_stringbuf);
  svn_stream__set_is_buffered(stream,is_buffered_handler_stringbuf);
  return stream;
}

struct string_stream_baton 
{
  const svn_string_t *str;
  apr_size_t amt_read;
}
;
/* svn_stream_mark_t for streams backed by stringbufs. */

struct string_stream_mark 
{
  apr_size_t pos;
}
;

static svn_error_t *read_handler_string(void *baton,char *buffer,apr_size_t *len)
{
  struct string_stream_baton *btn = baton;
  apr_size_t left_to_read = btn -> str -> len - btn -> amt_read;
   *len = ( *len > left_to_read?left_to_read :  *len);
  memcpy(buffer,(btn -> str -> data + btn -> amt_read), *len);
  btn -> amt_read +=  *len;
  return 0;
}

static svn_error_t *mark_handler_string(void *baton,svn_stream_mark_t **mark,apr_pool_t *pool)
{
  struct string_stream_baton *btn;
  struct string_stream_mark *marker;
  btn = baton;
  marker = (apr_palloc(pool,sizeof(( *marker))));
  marker -> pos = btn -> amt_read;
   *mark = ((svn_stream_mark_t *)marker);
  return 0;
}

static svn_error_t *seek_handler_string(void *baton,const svn_stream_mark_t *mark)
{
  struct string_stream_baton *btn = baton;
  if (mark != ((void *)0)) {
    const struct string_stream_mark *marker;
    marker = ((const struct string_stream_mark *)mark);
    btn -> amt_read = marker -> pos;
  }
  else {
    btn -> amt_read = 0;
  }
  return 0;
}

static svn_error_t *skip_handler_string(void *baton,apr_size_t len)
{
  struct string_stream_baton *btn = baton;
  apr_size_t left_to_read = btn -> str -> len - btn -> amt_read;
  len = (len > left_to_read?left_to_read : len);
  btn -> amt_read += len;
  return 0;
}

static svn_boolean_t is_buffered_handler_string(void *baton)
{
  return !0;
}

svn_stream_t *svn_stream_from_string(const svn_string_t *str,apr_pool_t *pool)
{
  svn_stream_t *stream;
  struct string_stream_baton *baton;
  if (!str) {
    return svn_stream_empty(pool);
  }
  baton = (apr_palloc(pool,sizeof(( *baton))));
  baton -> str = str;
  baton -> amt_read = 0;
  stream = svn_stream_create(baton,pool);
  svn_stream_set_read(stream,read_handler_string);
  svn_stream_set_mark(stream,mark_handler_string);
  svn_stream_set_seek(stream,seek_handler_string);
  svn_stream_set_skip(stream,skip_handler_string);
  svn_stream__set_is_buffered(stream,is_buffered_handler_string);
  return stream;
}

svn_error_t *svn_stream_for_stdin(svn_stream_t **in,apr_pool_t *pool)
{
  apr_file_t *stdin_file;
  apr_status_t apr_err;
  apr_err = apr_file_open_stdin(&stdin_file,pool);
  if (apr_err) {
    return svn_error_wrap_apr(apr_err,"Can't open stdin");
  }
   *in = svn_stream_from_aprfile2(stdin_file,!0,pool);
  return 0;
}

svn_error_t *svn_stream_for_stdout(svn_stream_t **out,apr_pool_t *pool)
{
  apr_file_t *stdout_file;
  apr_status_t apr_err;
  apr_err = apr_file_open_stdout(&stdout_file,pool);
  if (apr_err) {
    return svn_error_wrap_apr(apr_err,"Can't open stdout");
  }
   *out = svn_stream_from_aprfile2(stdout_file,!0,pool);
  return 0;
}

svn_error_t *svn_stream_for_stderr(svn_stream_t **err,apr_pool_t *pool)
{
  apr_file_t *stderr_file;
  apr_status_t apr_err;
  apr_err = apr_file_open_stderr(&stderr_file,pool);
  if (apr_err) {
    return svn_error_wrap_apr(apr_err,"Can't open stderr");
  }
   *err = svn_stream_from_aprfile2(stderr_file,!0,pool);
  return 0;
}

svn_error_t *svn_string_from_stream(svn_string_t **result,svn_stream_t *stream,apr_pool_t *result_pool,apr_pool_t *scratch_pool)
{
  svn_stringbuf_t *work = svn_stringbuf_create_ensure(16384,result_pool);
  char *buffer = (apr_palloc(scratch_pool,16384));
  while(1){
    apr_size_t len = 16384;
    do {
      svn_error_t *svn_err__temp = svn_stream_read(stream,buffer,&len);
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
    svn_stringbuf_appendbytes(work,buffer,len);
    if (len < 16384) {
      break; 
    }
  }
  do {
    svn_error_t *svn_err__temp = svn_stream_close(stream);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
   *result = (apr_palloc(result_pool,sizeof(( *( *result)))));
  ( *result) -> data = (work -> data);
  ( *result) -> len = work -> len;
  return 0;
}
/* These are somewhat arbirary, if we ever get good empirical data as to
   actually valid values, feel free to update them. */
#define BUFFER_BLOCK_SIZE 1024
#define BUFFER_MAX_SIZE 100000

svn_stream_t *svn_stream_buffered(apr_pool_t *result_pool)
{
  return svn_stream__from_spillbuf(1024,100000,result_pool);
}
/*** Lazyopen Streams ***/
/* Custom baton for lazyopen-style wrapper streams. */
typedef struct lazyopen_baton_t {
/* Callback function and baton for opening the wrapped stream. */
svn_stream_lazyopen_func_t open_func;
void *open_baton;
/* The wrapped stream, or NULL if the stream hasn't yet been
     opened. */
svn_stream_t *real_stream;
apr_pool_t *pool;
/* Whether to open the wrapped stream on a close call. */
svn_boolean_t open_on_close;}lazyopen_baton_t;
/* Use B->open_func/baton to create and set B->real_stream iff it
   isn't already set. */

static svn_error_t *lazyopen_if_unopened(lazyopen_baton_t *b)
{
  if (b -> real_stream == ((void *)0)) {
    svn_stream_t *stream;
    apr_pool_t *scratch_pool = svn_pool_create_ex(b -> pool,((void *)0));
    do {
      svn_error_t *svn_err__temp = (b -> open_func)(&stream,b -> open_baton,b -> pool,scratch_pool);
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
    apr_pool_destroy(scratch_pool);
    b -> real_stream = stream;
  }
  return 0;
}
/* Implements svn_read_fn_t */

static svn_error_t *read_handler_lazyopen(void *baton,char *buffer,apr_size_t *len)
{
  lazyopen_baton_t *b = baton;
  do {
    svn_error_t *svn_err__temp = lazyopen_if_unopened(b);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  do {
    svn_error_t *svn_err__temp = svn_stream_read(b -> real_stream,buffer,len);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  return 0;
}
/* Implements svn_stream_skip_fn_t */

static svn_error_t *skip_handler_lazyopen(void *baton,apr_size_t len)
{
  lazyopen_baton_t *b = baton;
  do {
    svn_error_t *svn_err__temp = lazyopen_if_unopened(b);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  do {
    svn_error_t *svn_err__temp = svn_stream_skip(b -> real_stream,len);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  return 0;
}
/* Implements svn_write_fn_t */

static svn_error_t *write_handler_lazyopen(void *baton,const char *data,apr_size_t *len)
{
  lazyopen_baton_t *b = baton;
  do {
    svn_error_t *svn_err__temp = lazyopen_if_unopened(b);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  do {
    svn_error_t *svn_err__temp = svn_stream_write(b -> real_stream,data,len);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  return 0;
}
/* Implements svn_close_fn_t */

static svn_error_t *close_handler_lazyopen(void *baton)
{
  lazyopen_baton_t *b = baton;
  if (b -> open_on_close) {
    do {
      svn_error_t *svn_err__temp = lazyopen_if_unopened(b);
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
  }
  if (b -> real_stream) {
    do {
      svn_error_t *svn_err__temp = svn_stream_close(b -> real_stream);
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
  }
  return 0;
}
/* Implements svn_stream_mark_fn_t */

static svn_error_t *mark_handler_lazyopen(void *baton,svn_stream_mark_t **mark,apr_pool_t *pool)
{
  lazyopen_baton_t *b = baton;
  do {
    svn_error_t *svn_err__temp = lazyopen_if_unopened(b);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  do {
    svn_error_t *svn_err__temp = svn_stream_mark(b -> real_stream,mark,pool);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  return 0;
}
/* Implements svn_stream_seek_fn_t */

static svn_error_t *seek_handler_lazyopen(void *baton,const svn_stream_mark_t *mark)
{
  lazyopen_baton_t *b = baton;
  do {
    svn_error_t *svn_err__temp = lazyopen_if_unopened(b);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  do {
    svn_error_t *svn_err__temp = svn_stream_seek(b -> real_stream,mark);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  return 0;
}
/* Implements svn_stream__is_buffered_fn_t */

static svn_boolean_t is_buffered_lazyopen(void *baton)
{
  lazyopen_baton_t *b = baton;
/* No lazy open as we cannot handle an open error. */
  if (!b -> real_stream) {
    return 0;
  }
  return svn_stream__is_buffered(b -> real_stream);
}

svn_stream_t *svn_stream_lazyopen_create(svn_stream_lazyopen_func_t open_func,void *open_baton,svn_boolean_t open_on_close,apr_pool_t *result_pool)
{
  lazyopen_baton_t *lob = (memset(apr_palloc(result_pool,sizeof(( *lob))),0,sizeof(( *lob))));
  svn_stream_t *stream;
  lob -> open_func = open_func;
  lob -> open_baton = open_baton;
  lob -> real_stream = ((void *)0);
  lob -> pool = result_pool;
  lob -> open_on_close = open_on_close;
  stream = svn_stream_create(lob,result_pool);
  svn_stream_set_read(stream,read_handler_lazyopen);
  svn_stream_set_skip(stream,skip_handler_lazyopen);
  svn_stream_set_write(stream,write_handler_lazyopen);
  svn_stream_set_close(stream,close_handler_lazyopen);
  svn_stream_set_mark(stream,mark_handler_lazyopen);
  svn_stream_set_seek(stream,seek_handler_lazyopen);
  svn_stream__set_is_buffered(stream,is_buffered_lazyopen);
  return stream;
}

verbalizes_countdowns schorlous_martelli(verbalizes_countdowns haywire_preacceptance)
{
  ++stonesoup_global_variable;
  return haywire_preacceptance;
}

void kenyan_brahmi(verbalizes_countdowns pasithea_trophonian)
{
    int stonesoup_i = 0;
    int stonesoup_opt_var;
    struct stonesoup_struct stonesoup_data;
  char *redoubtableness_podical = 0;
  ++stonesoup_global_variable;;
  redoubtableness_podical = ((char *)pasithea_trophonian);
    tracepoint(stonesoup_trace, weakness_start, "CWE120", "D", "Buffer Copy without Checking Size of Input");
    stonesoup_data.before = stonesoup_toupper;
    for (stonesoup_i = 0; stonesoup_i < 64; stonesoup_i++) {
        stonesoup_data.buffer[stonesoup_i] = 0;
    }
    stonesoup_data.after = stonesoup_toupper;
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_i", stonesoup_i, &stonesoup_i, "INITIAL-STATE");
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_data.before", stonesoup_data.before, &stonesoup_data.before, "INITIAL-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_data.buffer", stonesoup_data.buffer, "INITIAL-STATE");
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_data.after", stonesoup_data.after, &stonesoup_data.after, "INITIAL-STATE");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
/* STONESOUP: CROSSOVER-POINT (Unchecked buffer copy) */
    strcpy(stonesoup_data.buffer, redoubtableness_podical);
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_data.buffer", stonesoup_data.buffer, "CROSSOVER-STATE");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
/* STONESOUP: TRIGGER-POINT (Buffer Overflow: Unchecked heap buffer copy) */
    stonesoup_opt_var = strlen( stonesoup_data.buffer);
    for (stonesoup_i = 0; stonesoup_i < stonesoup_opt_var; ++stonesoup_i) {
        stonesoup_data.buffer[stonesoup_i] = stonesoup_toupper(stonesoup_data.buffer[stonesoup_i]);
        stonesoup_printf("%c",stonesoup_data.after(stonesoup_data.buffer[stonesoup_i]));
    }
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_i", stonesoup_i, &stonesoup_i, "FINAL-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_data.buffer", stonesoup_data.buffer, "FINAL-STATE");
    stonesoup_printf("\n");
    tracepoint(stonesoup_trace, weakness_end);
;
  if (pasithea_trophonian != 0) 
    free(((char *)pasithea_trophonian));
stonesoup_close_printf_context();
}
