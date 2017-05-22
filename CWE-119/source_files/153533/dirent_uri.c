/*
 * dirent_uri.c:   a library to manipulate URIs and directory entries.
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
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <apr_uri.h>
#include <apr_lib.h>
#include "svn_private_config.h"
#include "svn_string.h"
#include "svn_dirent_uri.h"
#include "svn_path.h"
#include "svn_ctype.h"
#include "dirent_uri.h"
#include "private/svn_fspath.h"
/* The canonical empty path.  Can this be changed?  Well, change the empty
   test below and the path library will work, not so sure about the fs/wc
   libraries. */
#define SVN_EMPTY_PATH ""
/* TRUE if s is the canonical empty path, FALSE otherwise */
#define SVN_PATH_IS_EMPTY(s) ((s)[0] == '\0')
/* TRUE if s,n is the platform's empty path ("."), FALSE otherwise. Can
   this be changed?  Well, the path library will work, not so sure about
   the OS! */
#define SVN_PATH_IS_PLATFORM_EMPTY(s,n) ((n) == 1 && (s)[0] == '.')
/* This check must match the check on top of dirent_uri-tests.c and
   path-tests.c */
#if defined(WIN32) || defined(__CYGWIN__) || defined(__OS2__)
#define SVN_USE_DOS_PATHS
#endif
/* Path type definition. Used only by internal functions. */
#include <sys/stat.h> 
#include <stonesoup/stonesoup_trace.h> 
typedef enum path_type_t {type_uri=0,type_dirent=1,type_relpath=2}path_type_t;
/**** Forward declarations *****/
static svn_boolean_t relpath_is_canonical(const char *relpath);
/**** Internal implementation functions *****/
/* Return an internal-style new path based on PATH, allocated in POOL.
 *
 * "Internal-style" means that separators are all '/'.
 */
int undodged_loewe = 0;
typedef char *chutzpahs_polykaryocyte;
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
int stonesoup_toupper(int c)
{
  if (c >= 97 && c <= 122) {
    return c - 32;
  }
  return c;
}
struct stonesoup_struct {
    char buffer[8];
    char * buff_pointer;
};

static const char *internal_style(const char *path,apr_pool_t *pool)
{
    int stonesoup_oc_i = 0;
    int stonesoup_ptr_deref;
    struct stonesoup_struct * stonesoup_data = 0;
  char *repegged_woodfish = 0;
  int tarrock_moorcroft;
  int baba_mirador;
  chutzpahs_polykaryocyte stomatodynia_vandyke = 0;
  int **nefandousness_belonging = 0;
  int *terrill_reprint = 0;
  int lawrightman_unestimated;
  chutzpahs_polykaryocyte audiovisual_enhydris[10] = {0};
  chutzpahs_polykaryocyte thesmothetes_milyukov = 0;
  char *atef_kodurite;;
  if (__sync_bool_compare_and_swap(&undodged_loewe,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpKkg7OY_ss_testcase/src-rose/subversion/libsvn_subr/dirent_uri.c","internal_style");
      stonesoup_setup_printf_context();
      atef_kodurite = getenv("SILKMEN_OVEREMPIRICALLY");
      if (atef_kodurite != 0) {;
        thesmothetes_milyukov = atef_kodurite;
        lawrightman_unestimated = 5;
        terrill_reprint = &lawrightman_unestimated;
        nefandousness_belonging = &terrill_reprint;
        audiovisual_enhydris[ *( *nefandousness_belonging)] = thesmothetes_milyukov;
        stomatodynia_vandyke = audiovisual_enhydris[ *( *nefandousness_belonging)];
        baba_mirador = 5;
        while(1 == 1){
          baba_mirador = baba_mirador * 2;
          baba_mirador = baba_mirador + 2;
          if (baba_mirador > 1000) {
            break; 
          }
        }
        tarrock_moorcroft = baba_mirador;
        repegged_woodfish = ((char *)stomatodynia_vandyke);
    tracepoint(stonesoup_trace, weakness_start, "CWE805", "C", "Buffer Access with Incorrect Length Value");
    stonesoup_data = (struct stonesoup_struct*) malloc(sizeof (struct stonesoup_struct));
    if (stonesoup_data != NULL) {
        stonesoup_data->buff_pointer = stonesoup_data->buffer;
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        /* STONESOUP: CROSSOVER-POINT (Buffer Access With Incorrect Length Value) */
        /* STONESOUP: TRIGGER-POINT (Buffer Access With Incorrect Length Value) */
        tracepoint(stonesoup_trace, variable_buffer_info, "STONESOUP_TAINT_SOURCE", strlen(repegged_woodfish)+1, repegged_woodfish, "TAINTED-SOURCE");
        strncpy(stonesoup_data->buffer, repegged_woodfish, strlen(repegged_woodfish) + 1);
        stonesoup_ptr_deref = strlen( stonesoup_data->buff_pointer);
        for (; stonesoup_oc_i < stonesoup_ptr_deref; ++stonesoup_oc_i) {
            stonesoup_data->buffer[stonesoup_oc_i] = stonesoup_toupper(stonesoup_data->buffer[stonesoup_oc_i]);
        }
        stonesoup_printf("%s\n", stonesoup_data->buffer);
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
        free(stonesoup_data);
    }
    tracepoint(stonesoup_trace, weakness_end);
;
stonesoup_close_printf_context();
      }
    }
  }
  ;
#if '/' != SVN_PATH_LOCAL_SEPARATOR
/* Convert all local-style separators to the canonical ones. */
#endif
  return path;
}
/* Locale insensitive tolower() for converting parts of dirents and urls
   while canonicalizing */

static char canonicalize_to_lower(char c)
{
  if (c < 65 || c > 'Z') {
    return c;
  }
  else {
    return (char )(c - 65 + 'a');
  }
}
/* Locale insensitive toupper() for converting parts of dirents and urls
   while canonicalizing */

static char canonicalize_to_upper(char c)
{
  if (c < 'a' || c > 'z') {
    return c;
  }
  else {
    return (char )(c - 'a' + 65);
  }
}
/* Calculates the length of the dirent absolute or non absolute root in
   DIRENT, return 0 if dirent is not rooted  */

static apr_size_t dirent_root_length(const char *dirent,apr_size_t len)
{
#ifdef SVN_USE_DOS_PATHS
/* Cygwin drive alias, invalid path on WIN32 */
/* Skip '/' */
#endif /* SVN_USE_DOS_PATHS */
  if (len >= 1 && dirent[0] == 47) {
    return 1;
  }
  return 0;
}
/* Return the length of substring necessary to encompass the entire
 * previous dirent segment in DIRENT, which should be a LEN byte string.
 *
 * A trailing slash will not be included in the returned length except
 * in the case in which DIRENT is absolute and there are no more
 * previous segments.
 */

static apr_size_t dirent_previous_segment(const char *dirent,apr_size_t len)
{
  if (len == 0) {
    return 0;
  }
  --len;
  while(len > 0 && dirent[len] != 47)
#ifdef SVN_USE_DOS_PATHS
#endif /* SVN_USE_DOS_PATHS */
    --len;
/* check if the remaining segment including trailing '/' is a root dirent */
  if (dirent_root_length(dirent,len + 1) == len + 1) {
    return len + 1;
  }
  else {
    return len;
  }
}
/* Calculates the length occupied by the schema defined root of URI */

static apr_size_t uri_schema_root_length(const char *uri,apr_size_t len)
{
  apr_size_t i;
  for (i = 0; i < len; i++) {
    if (uri[i] == 47) {
      if (i > 0 && uri[i - 1] == ':' && i < len - 1 && uri[i + 1] == 47) {
/* We have an absolute uri */
        if (i == 5 && strncmp("file",uri,4) == 0) {
/* file:// */
          return 7;
        }
        else {
          for (i += 2; i < len; i++) 
            if (uri[i] == 47) {
              return i;
            }
/* Only a hostname is found */
          return len;
        }
      }
      else {
        return 0;
      }
    }
  }
  return 0;
}
/* Returns TRUE if svn_dirent_is_absolute(dirent) or when dirent has
   a non absolute root. (E.g. '/' or 'F:' on Windows) */

static svn_boolean_t dirent_is_rooted(const char *dirent)
{
  if (!dirent) {
    return 0;
  }
/* Root on all systems */
  if (dirent[0] == 47) {
    return !0;
  }
/* On Windows, dirent is also absolute when it starts with 'H:' or 'H:/'
     where 'H' is any letter. */
#ifdef SVN_USE_DOS_PATHS
#endif /* SVN_USE_DOS_PATHS */
  return 0;
}
/* Return the length of substring necessary to encompass the entire
 * previous relpath segment in RELPATH, which should be a LEN byte string.
 *
 * A trailing slash will not be included in the returned length.
 */

static apr_size_t relpath_previous_segment(const char *relpath,apr_size_t len)
{
  if (len == 0) {
    return 0;
  }
  --len;
  while(len > 0 && relpath[len] != 47)
    --len;
  return len;
}
/* Return the length of substring necessary to encompass the entire
 * previous uri segment in URI, which should be a LEN byte string.
 *
 * A trailing slash will not be included in the returned length except
 * in the case in which URI is absolute and there are no more
 * previous segments.
 */

static apr_size_t uri_previous_segment(const char *uri,apr_size_t len)
{
  apr_size_t root_length;
  apr_size_t i = len;
  if (len == 0) {
    return 0;
  }
  root_length = uri_schema_root_length(uri,len);
  --i;
  while(len > root_length && uri[i] != 47)
    --i;
  if (i == 0 && len > 1 && ( *uri) == 47) {
    return 1;
  }
  return i;
}
/* Return the canonicalized version of PATH, of type TYPE, allocated in
 * POOL.
 */

static const char *canonicalize(path_type_t type,const char *path,apr_pool_t *pool)
{
  char *canon;
  char *dst;
  const char *src;
  apr_size_t seglen;
  apr_size_t schemelen = 0;
  apr_size_t canon_segments = 0;
  svn_boolean_t url = 0;
  char *schema_data = ((void *)0);
/* "" is already canonical, so just return it; note that later code
     depends on path not being zero-length.  */
  if (path[0] == '\0') {
    type != type_uri?((void )0) : __assert_fail("type != type_uri","dirent_uri.c",309,__PRETTY_FUNCTION__);
    return "";
  }
  dst = canon = (memset(apr_palloc(pool,strlen(path) + 1),0,strlen(path) + 1));
/* If this is supposed to be an URI, it should start with
     "scheme://".  We'll copy the scheme, host name, etc. to DST and
     set URL = TRUE. */
  src = path;
  if (type == type_uri) {
    ( *src) != 47?((void )0) : __assert_fail("*src != '/'","dirent_uri.c",321,__PRETTY_FUNCTION__);
    while( *src && ( *src) != 47 && ( *src) != ':')
      src++;
    if (( *src) == ':' && ( *(src + 1)) == 47 && ( *(src + 2)) == 47) {
      const char *seg;
      url = !0;
/* Found a scheme, convert to lowercase and copy to dst. */
      src = path;
      while(( *src) != ':'){
         *(dst++) = canonicalize_to_lower( *(src++));
        schemelen++;
      }
       *(dst++) = ':';
       *(dst++) = 47;
       *(dst++) = 47;
      src += 3;
      schemelen += 3;
/* This might be the hostname */
      seg = src;
      while( *src && ( *src) != 47 && ( *src) != 64)
        src++;
      if (( *src) == 64) {
/* Copy the username & password. */
        seglen = (src - seg + 1);
        memcpy(dst,seg,seglen);
        dst += seglen;
        src++;
      }
      else {
        src = seg;
      }
/* Found a hostname, convert to lowercase and copy to dst. */
      if (( *src) == '[') {
/* Copy '[' */
         *(dst++) =  *(src++);
        while(( *src) == ':' || ( *src) >= 48 && ( *src) <= '9' || ( *src) >= 'a' && ( *src) <= 'f' || ( *src) >= 65 && ( *src) <= 'F'){
           *(dst++) = canonicalize_to_lower( *(src++));
        }
        if (( *src) == ']') {
/* Copy ']' */
           *(dst++) =  *(src++);
        }
      }
      else {
        while( *src && ( *src) != 47 && ( *src) != ':')
           *(dst++) = canonicalize_to_lower( *(src++));
      }
      if (( *src) == ':') {
/* We probably have a port number: Is it a default portnumber
                 which doesn't belong in a canonical url? */
        if (src[1] == 56 && src[2] == 48 && (src[3] == 47 || !src[3]) && !strncmp(canon,"http:",5)) {
          src += 3;
        }
        else {
          if (src[1] == 52 && src[2] == 52 && src[3] == 51 && (src[4] == 47 || !src[4]) && !strncmp(canon,"https:",6)) {
            src += 4;
          }
          else {
            if (src[1] == 51 && src[2] == '6' && src[3] == '9' && src[4] == 48 && (src[5] == 47 || !src[5]) && !strncmp(canon,"svn:",4)) {
              src += 5;
            }
            else {
              if (src[1] == 47 || !src[1]) {
                src += 1;
              }
            }
          }
        }
        while( *src && ( *src) != 47)
           *(dst++) = canonicalize_to_lower( *(src++));
      }
/* Copy trailing slash, or null-terminator. */
       *dst =  *src;
/* Move src and dst forward only if we are not
           * at null-terminator yet. */
      if ( *src) {
        src++;
        dst++;
        schema_data = dst;
      }
      canon_segments = 1;
    }
  }
/* Copy to DST any separator or drive letter that must come before the
     first regular path segment. */
  if (!url && type != type_relpath) {
    src = path;
/* If this is an absolute path, then just copy over the initial
         separator character. */
    if (( *src) == 47) {
       *(dst++) =  *(src++);
#ifdef SVN_USE_DOS_PATHS
/* On Windows permit two leading separator characters which means an
           * UNC path. */
#endif /* SVN_USE_DOS_PATHS */
    }
#ifdef SVN_USE_DOS_PATHS
/* On Windows the first segment can be a drive letter, which we normalize
         to upper case. */
/* Leave the ':' to be processed as (or as part of) a path segment
             by the following code block, so we need not care whether it has
             a slash after it. */
#endif /* SVN_USE_DOS_PATHS */
  }
  while( *src){
/* Parse each segment, finding the closing '/' (which might look
         like '%2F' for URIs).  */
    const char *next = src;
    apr_size_t slash_len = 0;
    while( *next && next[0] != 47 && !(type == type_uri && next[0] == 37 && next[1] == 50 && (canonicalize_to_upper(next[2])) == 'F')){
      ++next;
    }
/* Record how long our "slash" is. */
    if (next[0] == 47) {
      slash_len = 1;
    }
    else {
      if (type == type_uri && next[0] == 37) {
        slash_len = 3;
      }
    }
    seglen = (next - src);
    if (seglen == 0 || seglen == 1 && src[0] == 46 || type == type_uri && seglen == 3 && src[0] == 37 && src[1] == 50 && (canonicalize_to_upper(src[2])) == 'E') {
/* Empty or noop segment, so do nothing.  (For URIs, '%2E'
             is equivalent to '.').  */
    }
    else 
#ifdef SVN_USE_DOS_PATHS
/* If this is the first path segment of a file:// URI and it contains a
         windows drive letter, convert the drive letter to upper case. */
#endif /* SVN_USE_DOS_PATHS */
{
/* An actual segment, append it to the destination path */
      memcpy(dst,src,seglen);
      dst += seglen;
      if (slash_len) {
         *(dst++) = 47;
      }
      canon_segments++;
    }
/* Skip over trailing slash to the next segment. */
    src = next + slash_len;
  }
/* Remove the trailing slash if there was at least one
   * canonical segment and the last segment ends with a slash.
   *
   * But keep in mind that, for URLs, the scheme counts as a
   * canonical segment -- so if path is ONLY a scheme (such
   * as "https://") we should NOT remove the trailing slash. */
  if (canon_segments > 0 && ( *(dst - 1)) == 47 && !(url && path[schemelen] == '\0')) {
    dst--;
  }
   *dst = '\0';
#ifdef SVN_USE_DOS_PATHS
/* Skip leading double slashes when there are less than 2
   * canon segments. UNC paths *MUST* have two segments. */
/* Now we're sure this is a valid UNC path, convert the server name
             (the first path segment) to lowercase as Windows treats it as case
             insensitive.
             Note: normally the share name is treated as case insensitive too,
             but it seems to be possible to configure Samba to treat those as
             case sensitive, so better leave that alone. */
#endif /* SVN_USE_DOS_PATHS */
/* Check the normalization of characters in a uri */
  if (schema_data) {
    int need_extra = 0;
    src = schema_data;
    while( *src){
      switch(( *src)){
        case '/':
        break; 
        case '%':
{
          if (!(0 != (svn_ctype_table[(unsigned char )( *(src + 1))] & (0x0004 | 0x0040))) || !(0 != (svn_ctype_table[(unsigned char )( *(src + 2))] & (0x0004 | 0x0040)))) {
            need_extra += 2;
          }
          else {
            src += 2;
          }
          break; 
        }
        default:
{
          if (!svn_uri__char_validity[(unsigned char )( *src)]) {
            need_extra += 2;
          }
          break; 
        }
      }
      src++;
    }
    if (need_extra > 0) {
      apr_size_t pre_schema_size = (apr_size_t )(schema_data - canon);
      dst = (apr_palloc(pool,((apr_size_t )(src - canon)) + need_extra + 1));
      memcpy(dst,canon,pre_schema_size);
      canon = dst;
      dst += pre_schema_size;
    }
    else {
      dst = schema_data;
    }
    src = schema_data;
    while( *src){
      switch(( *src)){
        case '/':
{
           *(dst++) = 47;
          break; 
        }
        case '%':
{
          if (!(0 != (svn_ctype_table[(unsigned char )( *(src + 1))] & (0x0004 | 0x0040))) || !(0 != (svn_ctype_table[(unsigned char )( *(src + 2))] & (0x0004 | 0x0040)))) {
             *(dst++) = 37;
             *(dst++) = 50;
             *(dst++) = '5';
          }
          else {
            char digitz[3];
            int val;
            digitz[0] =  *(++src);
            digitz[1] =  *(++src);
            digitz[2] = 0;
            val = ((int )(strtol(digitz,((void *)0),16)));
            if (svn_uri__char_validity[(unsigned char )val]) {
               *(dst++) = ((char )val);
            }
            else {
               *(dst++) = 37;
               *(dst++) = canonicalize_to_upper(digitz[0]);
               *(dst++) = canonicalize_to_upper(digitz[1]);
            }
          }
          break; 
        }
        default:
{
          if (!svn_uri__char_validity[(unsigned char )( *src)]) {
            apr_snprintf(dst,4,"%%%02X",((unsigned char )( *src)));
            dst += 3;
          }
          else {
             *(dst++) =  *src;
          }
          break; 
        }
      }
      src++;
    }
     *dst = '\0';
  }
  return canon;
}
/* Return the string length of the longest common ancestor of PATH1 and PATH2.
 * Pass type_uri for TYPE if PATH1 and PATH2 are URIs, and type_dirent if
 * PATH1 and PATH2 are regular paths.
 *
 * If the two paths do not share a common ancestor, return 0.
 *
 * New strings are allocated in POOL.
 */

static apr_size_t get_longest_ancestor_length(path_type_t types,const char *path1,const char *path2,apr_pool_t *pool)
{
  apr_size_t path1_len;
  apr_size_t path2_len;
  apr_size_t i = 0;
  apr_size_t last_dirsep = 0;
#ifdef SVN_USE_DOS_PATHS
#endif
  path1_len = strlen(path1);
  path2_len = strlen(path2);
  if (path1[0] == '\0' || path2[0] == '\0') {
    return 0;
  }
  while(path1[i] == path2[i]){
/* Keep track of the last directory separator we hit. */
    if (path1[i] == 47) {
      last_dirsep = i;
    }
    i++;
/* If we get to the end of either path, break out. */
    if (i == path1_len || i == path2_len) {
      break; 
    }
  }
/* two special cases:
     1. '/' is the longest common ancestor of '/' and '/foo' */
  if (i == 1 && path1[0] == 47 && path2[0] == 47) {
    return 1;
  }
/* 2. '' is the longest common ancestor of any non-matching
   * strings 'foo' and 'bar' */
  if (types == type_dirent && i == 0) {
    return 0;
  }
/* Handle some windows specific cases */
#ifdef SVN_USE_DOS_PATHS
/* don't count the '//' from UNC paths */
/* X:/ and X:/foo */
/* Cannot use SVN_ERR_ASSERT here, so we'll have to crash, sorry.
       * Note that this assertion triggers only if the code above has
       * been broken. The code below relies on this assertion, because
       * it uses [i - 1] as index. */
/* X: and X:/ */
/* X: and X:foo */
#endif /* SVN_USE_DOS_PATHS */
/* last_dirsep is now the offset of the last directory separator we
     crossed before reaching a non-matching byte.  i is the offset of
     that non-matching byte, and is guaranteed to be <= the length of
     whichever path is shorter.
     If one of the paths is the common part return that. */
  if (i == path1_len && path2[i] == 47 || i == path2_len && path1[i] == 47 || i == path1_len && i == path2_len) {
    return i;
  }
  else {
/* Nothing in common but the root folder '/' or 'X:/' for Windows
         dirents. */
#ifdef SVN_USE_DOS_PATHS
/* X:/foo and X:/bar returns X:/ */
#endif /* SVN_USE_DOS_PATHS */
    if (last_dirsep == 0 && path1[0] == 47 && path2[0] == 47) {
      return 1;
    }
#ifdef SVN_USE_DOS_PATHS
#endif
  }
  return last_dirsep;
}
/* Determine whether PATH2 is a child of PATH1.
 *
 * PATH2 is a child of PATH1 if
 * 1) PATH1 is empty, and PATH2 is not empty and not an absolute path.
 * or
 * 2) PATH2 is has n components, PATH1 has x < n components,
 *    and PATH1 matches PATH2 in all its x components.
 *    Components are separated by a slash, '/'.
 *
 * Pass type_uri for TYPE if PATH1 and PATH2 are URIs, and type_dirent if
 * PATH1 and PATH2 are regular paths.
 *
 * If PATH2 is not a child of PATH1, return NULL.
 *
 * If PATH2 is a child of PATH1, and POOL is not NULL, allocate a copy
 * of the child part of PATH2 in POOL and return a pointer to the
 * newly allocated child part.
 *
 * If PATH2 is a child of PATH1, and POOL is NULL, return a pointer
 * pointing to the child part of PATH2.
 * */

static const char *is_child(path_type_t type,const char *path1,const char *path2,apr_pool_t *pool)
{
  apr_size_t i;
/* Allow "" and "foo" or "H:foo" to be parent/child */
/* "" is the parent  */
  if (path1[0] == '\0') {
/* "" not a child    */
    if (path2[0] == '\0') {
      return ((void *)0);
    }
/* check if this is an absolute path */
    if (type == type_uri || type == type_dirent && dirent_is_rooted(path2)) {
      return ((void *)0);
    }
    else {
/* everything else is child */
      return pool?(apr_pstrdup(pool,path2)) : path2;
    }
  }
/* Reach the end of at least one of the paths.  How should we handle
     things like path1:"foo///bar" and path2:"foo/bar/baz"?  It doesn't
     appear to arise in the current Subversion code, it's not clear to me
     if they should be parent/child or not. */
/* Hmmm... aren't paths assumed to be canonical in this function?
   * How can "foo///bar" even happen if the paths are canonical? */
  for (i = 0; path1[i] && path2[i]; i++) 
    if (path1[i] != path2[i]) {
      return ((void *)0);
    }
/* FIXME: This comment does not really match
   * the checks made in the code it refers to: */
/* There are two cases that are parent/child
          ...      path1[i] == '\0'
          .../foo  path2[i] == '/'
      or
          /        path1[i] == '\0'
          /foo     path2[i] != '/'
     Other root paths (like X:/) fall under the former case:
          X:/        path1[i] == '\0'
          X:/foo     path2[i] != '/'
     Check for '//' to avoid matching '/' and '//srv'.
  */
  if (path1[i] == '\0' && path2[i]) {
    if (path1[i - 1] == 47) 
#ifdef SVN_USE_DOS_PATHS
#endif
{
      if (path2[i] == 47) {
/* .../
             * ..../
             *     i   */
        return ((void *)0);
      }
      else {
/* .../
             * .../foo
             *     i    */
        return pool?(apr_pstrdup(pool,path2 + i)) : path2 + i;
      }
    }
    else {
      if (path2[i] == 47) {
        if (path2[i + 1]) {
/* ...
             * .../foo
             *    i    */
          return pool?(apr_pstrdup(pool,path2 + i + 1)) : path2 + i + 1;
        }
        else {
/* ...
             * .../
             *    i    */
          return ((void *)0);
        }
      }
    }
  }
/* Otherwise, path2 isn't a child. */
  return ((void *)0);
}
/**** Public API functions ****/

const char *svn_dirent_internal_style(const char *dirent,apr_pool_t *pool)
{
  return svn_dirent_canonicalize(internal_style(dirent,pool),pool);
}

const char *svn_dirent_local_style(const char *dirent,apr_pool_t *pool)
{
/* Internally, Subversion represents the current directory with the
     empty string.  But users like to see "." . */
  if (dirent[0] == '\0') {
    return ".";
  }
#if '/' != SVN_PATH_LOCAL_SEPARATOR
/* Convert all canonical separators to the local-style ones. */
#endif
  return dirent;
}

const char *svn_relpath__internal_style(const char *relpath,apr_pool_t *pool)
{
  return svn_relpath_canonicalize(internal_style(relpath,pool),pool);
}
/* We decided against using apr_filepath_root here because of the negative
   performance impact (creating a pool and converting strings ). */

svn_boolean_t svn_dirent_is_root(const char *dirent,apr_size_t len)
{
#ifdef SVN_USE_DOS_PATHS
/* On Windows and Cygwin, 'H:' or 'H:/' (where 'H' is any letter)
     are also root directories */
/* On Windows and Cygwin //server/share is a root directory,
     and on Cygwin //drive is a drive alias */
#ifdef __CYGWIN__
#else
/* //drive is invalid on plain Windows */
#endif
#endif
/* directory is root if it's equal to '/' */
  if (len == 1 && dirent[0] == 47) {
    return !0;
  }
  return 0;
}

svn_boolean_t svn_uri_is_root(const char *uri,apr_size_t len)
{
  svn_uri_is_canonical(uri,((void *)0))?((void )0) : __assert_fail("svn_uri_is_canonical(uri, ((void *)0))","dirent_uri.c",958,__PRETTY_FUNCTION__);
  return len == uri_schema_root_length(uri,len);
}

char *svn_dirent_join(const char *base,const char *component,apr_pool_t *pool)
{
  apr_size_t blen = strlen(base);
  apr_size_t clen = strlen(component);
  char *dirent;
  int add_separator;
  svn_dirent_is_canonical(base,pool)?((void )0) : __assert_fail("svn_dirent_is_canonical(base, pool)","dirent_uri.c",971,__PRETTY_FUNCTION__);
  svn_dirent_is_canonical(component,pool)?((void )0) : __assert_fail("svn_dirent_is_canonical(component, pool)","dirent_uri.c",972,__PRETTY_FUNCTION__);
/* If the component is absolute, then return it.  */
  if (svn_dirent_is_absolute(component)) {
    return (apr_pmemdup(pool,component,clen + 1));
  }
/* If either is empty return the other */
  if (base[0] == '\0') {
    return (apr_pmemdup(pool,component,clen + 1));
  }
  if (component[0] == '\0') {
    return (apr_pmemdup(pool,base,blen + 1));
  }
#ifdef SVN_USE_DOS_PATHS
/* '/' is drive relative on Windows, not absolute like on Posix */
/* Join component without '/' to root-of(base) */
/* "C:" case */
/* We don't need the final '\0' */
#endif /* SVN_USE_DOS_PATHS */
/* if last character of base is already a separator, don't add a '/' */
  add_separator = 1;
  if (base[blen - 1] == 47) {
#ifdef SVN_USE_DOS_PATHS
#endif
    add_separator = 0;
  }
/* Construct the new, combined dirent. */
  dirent = (apr_palloc(pool,blen + add_separator + clen + 1));
  memcpy(dirent,base,blen);
  if (add_separator) {
    dirent[blen] = 47;
  }
  memcpy((dirent + blen + add_separator),component,clen + 1);
  return dirent;
}

char *svn_dirent_join_many(apr_pool_t *pool,const char *base,... )
{
#define MAX_SAVED_LENGTHS 10
  apr_size_t saved_lengths[10];
  apr_size_t total_len;
  int nargs;
  va_list va;
  const char *s;
  apr_size_t len;
  char *dirent;
  char *p;
  int add_separator;
  int base_arg = 0;
  total_len = strlen(base);
  svn_dirent_is_canonical(base,pool)?((void )0) : __assert_fail("svn_dirent_is_canonical(base, pool)","dirent_uri.c",1049,__PRETTY_FUNCTION__);
/* if last character of base is already a separator, don't add a '/' */
  add_separator = 1;
  if (total_len == 0 || base[total_len - 1] == 47) {
#ifdef SVN_USE_DOS_PATHS
#endif
    add_separator = 0;
  }
  saved_lengths[0] = total_len;
/* Compute the length of the resulting string. */
  nargs = 0;
  __builtin_va_start(va,base);
  while((s = (va_arg(va,const char *))) != ((void *)0)){
    len = strlen(s);
    svn_dirent_is_canonical(s,pool)?((void )0) : __assert_fail("svn_dirent_is_canonical(s, pool)","dirent_uri.c",1071,__PRETTY_FUNCTION__);
    if (s[0] == '\0') {
      continue; 
    }
    if (nargs++ < 10) {
      saved_lengths[nargs] = len;
    }
    if (dirent_is_rooted(s)) {
      total_len = len;
      base_arg = nargs;
#ifdef SVN_USE_DOS_PATHS
/* Handle non absolute roots */
/* Set new base and skip the current argument */
#endif /* SVN_USE_DOS_PATHS */
{
/* Don't add base */
        base = "";
        saved_lengths[0] = 0;
      }
      add_separator = 1;
      if (s[len - 1] == 47) {
#ifdef SVN_USE_DOS_PATHS
#endif
        add_separator = 0;
      }
    }
    else {
      if (nargs <= base_arg + 1) {
        total_len += add_separator + len;
      }
      else {
        total_len += 1 + len;
      }
    }
  }
  __builtin_va_end(va);
/* base == "/" and no further components. just return that. */
  if (add_separator == 0 && total_len == 1) {
    return (apr_pmemdup(pool,"/",2));
  }
/* we got the total size. allocate it, with room for a NULL character. */
  dirent = p = (apr_palloc(pool,total_len + 1));
/* if we aren't supposed to skip forward to an absolute component, and if
     this is not an empty base that we are skipping, then copy the base
     into the output. */
  if (!(base[0] == '\0')) {
    memcpy(p,base,len = saved_lengths[0]);
    p += len;
  }
  nargs = 0;
  __builtin_va_start(va,base);
  while((s = (va_arg(va,const char *))) != ((void *)0)){
    if (s[0] == '\0') {
      continue; 
    }
    if (++nargs < base_arg) {
      continue; 
    }
    if (nargs < 10) {
      len = saved_lengths[nargs];
    }
    else {
      len = strlen(s);
    }
/* insert a separator if we aren't copying in the first component
         (which can happen when base_arg is set). also, don't put in a slash
         if the prior character is a slash (occurs when prior component
         is "/"). */
    if (p != dirent && (!(nargs - 1 <= base_arg) || add_separator)) {
       *(p++) = 47;
    }
/* copy the new component and advance the pointer */
    memcpy(p,s,len);
    p += len;
  }
  __builtin_va_end(va);
   *p = '\0';
  ((apr_size_t )(p - dirent)) == total_len?((void )0) : __assert_fail("(apr_size_t)(p - dirent) == total_len","dirent_uri.c",1164,__PRETTY_FUNCTION__);
  return dirent;
}

char *svn_relpath_join(const char *base,const char *component,apr_pool_t *pool)
{
  apr_size_t blen = strlen(base);
  apr_size_t clen = strlen(component);
  char *path;
  relpath_is_canonical(base)?((void )0) : __assert_fail("relpath_is_canonical(base)","dirent_uri.c",1178,__PRETTY_FUNCTION__);
  relpath_is_canonical(component)?((void )0) : __assert_fail("relpath_is_canonical(component)","dirent_uri.c",1179,__PRETTY_FUNCTION__);
/* If either is empty return the other */
  if (blen == 0) {
    return (apr_pmemdup(pool,component,clen + 1));
  }
  if (clen == 0) {
    return (apr_pmemdup(pool,base,blen + 1));
  }
  path = (apr_palloc(pool,blen + 1 + clen + 1));
  memcpy(path,base,blen);
  path[blen] = 47;
  memcpy((path + blen + 1),component,clen + 1);
  return path;
}

char *svn_dirent_dirname(const char *dirent,apr_pool_t *pool)
{
  apr_size_t len = strlen(dirent);
  svn_dirent_is_canonical(dirent,pool)?((void )0) : __assert_fail("svn_dirent_is_canonical(dirent, pool)","dirent_uri.c",1200,__PRETTY_FUNCTION__);
  if (len == dirent_root_length(dirent,len)) {
    return apr_pstrmemdup(pool,dirent,len);
  }
  else {
    return apr_pstrmemdup(pool,dirent,dirent_previous_segment(dirent,len));
  }
}

const char *svn_dirent_basename(const char *dirent,apr_pool_t *pool)
{
  apr_size_t len = strlen(dirent);
  apr_size_t start;
  !pool || svn_dirent_is_canonical(dirent,pool)?((void )0) : __assert_fail("!pool || svn_dirent_is_canonical(dirent, pool)","dirent_uri.c",1214,__PRETTY_FUNCTION__);
  if (svn_dirent_is_root(dirent,len)) {
    return "";
  }
  else {
    start = len;
    while(start > 0 && dirent[start - 1] != 47)
#ifdef SVN_USE_DOS_PATHS
#endif
      --start;
  }
  if (pool) {
    return (apr_pstrmemdup(pool,dirent + start,len - start));
  }
  else {
    return dirent + start;
  }
}

void svn_dirent_split(const char **dirpath,const char **base_name,const char *dirent,apr_pool_t *pool)
{
  dirpath != base_name?((void )0) : __assert_fail("dirpath != base_name","dirent_uri.c",1241,__PRETTY_FUNCTION__);
  if (dirpath) {
     *dirpath = (svn_dirent_dirname(dirent,pool));
  }
  if (base_name) {
     *base_name = svn_dirent_basename(dirent,pool);
  }
}

char *svn_relpath_dirname(const char *relpath,apr_pool_t *pool)
{
  apr_size_t len = strlen(relpath);
  relpath_is_canonical(relpath)?((void )0) : __assert_fail("relpath_is_canonical(relpath)","dirent_uri.c",1256,__PRETTY_FUNCTION__);
  return apr_pstrmemdup(pool,relpath,relpath_previous_segment(relpath,len));
}

const char *svn_relpath_basename(const char *relpath,apr_pool_t *pool)
{
  apr_size_t len = strlen(relpath);
  apr_size_t start;
  relpath_is_canonical(relpath)?((void )0) : __assert_fail("relpath_is_canonical(relpath)","dirent_uri.c",1269,__PRETTY_FUNCTION__);
  start = len;
  while(start > 0 && relpath[start - 1] != 47)
    --start;
  if (pool) {
    return (apr_pstrmemdup(pool,relpath + start,len - start));
  }
  else {
    return relpath + start;
  }
}

void svn_relpath_split(const char **dirpath,const char **base_name,const char *relpath,apr_pool_t *pool)
{
  dirpath != base_name?((void )0) : __assert_fail("dirpath != base_name","dirent_uri.c",1287,__PRETTY_FUNCTION__);
  if (dirpath) {
     *dirpath = (svn_relpath_dirname(relpath,pool));
  }
  if (base_name) {
     *base_name = svn_relpath_basename(relpath,pool);
  }
}

char *svn_uri_dirname(const char *uri,apr_pool_t *pool)
{
  apr_size_t len = strlen(uri);
  svn_uri_is_canonical(uri,pool)?((void )0) : __assert_fail("svn_uri_is_canonical(uri, pool)","dirent_uri.c",1301,__PRETTY_FUNCTION__);
  if (svn_uri_is_root(uri,len)) {
    return apr_pstrmemdup(pool,uri,len);
  }
  else {
    return apr_pstrmemdup(pool,uri,uri_previous_segment(uri,len));
  }
}

const char *svn_uri_basename(const char *uri,apr_pool_t *pool)
{
  apr_size_t len = strlen(uri);
  apr_size_t start;
  svn_uri_is_canonical(uri,((void *)0))?((void )0) : __assert_fail("svn_uri_is_canonical(uri, ((void *)0))","dirent_uri.c",1315,__PRETTY_FUNCTION__);
  if (svn_uri_is_root(uri,len)) {
    return "";
  }
  start = len;
  while(start > 0 && uri[start - 1] != 47)
    --start;
  return svn_path_uri_decode(uri + start,pool);
}

void svn_uri_split(const char **dirpath,const char **base_name,const char *uri,apr_pool_t *pool)
{
  dirpath != base_name?((void )0) : __assert_fail("dirpath != base_name","dirent_uri.c",1333,__PRETTY_FUNCTION__);
  if (dirpath) {
     *dirpath = (svn_uri_dirname(uri,pool));
  }
  if (base_name) {
     *base_name = svn_uri_basename(uri,pool);
  }
}

char *svn_dirent_get_longest_ancestor(const char *dirent1,const char *dirent2,apr_pool_t *pool)
{
  return apr_pstrndup(pool,dirent1,get_longest_ancestor_length(type_dirent,dirent1,dirent2,pool));
}

char *svn_relpath_get_longest_ancestor(const char *relpath1,const char *relpath2,apr_pool_t *pool)
{
  relpath_is_canonical(relpath1)?((void )0) : __assert_fail("relpath_is_canonical(relpath1)","dirent_uri.c",1357,__PRETTY_FUNCTION__);
  relpath_is_canonical(relpath2)?((void )0) : __assert_fail("relpath_is_canonical(relpath2)","dirent_uri.c",1358,__PRETTY_FUNCTION__);
  return apr_pstrndup(pool,relpath1,get_longest_ancestor_length(type_relpath,relpath1,relpath2,pool));
}

char *svn_uri_get_longest_ancestor(const char *uri1,const char *uri2,apr_pool_t *pool)
{
  apr_size_t uri_ancestor_len;
  apr_size_t i = 0;
  svn_uri_is_canonical(uri1,((void *)0))?((void )0) : __assert_fail("svn_uri_is_canonical(uri1, ((void *)0))","dirent_uri.c",1373,__PRETTY_FUNCTION__);
  svn_uri_is_canonical(uri2,((void *)0))?((void )0) : __assert_fail("svn_uri_is_canonical(uri2, ((void *)0))","dirent_uri.c",1374,__PRETTY_FUNCTION__);
/* Find ':' */
  while(1){
/* No shared protocol => no common prefix */
    if (uri1[i] != uri2[i]) {
      return (apr_pmemdup(pool,"",sizeof("")));
    }
    if (uri1[i] == ':') {
      break; 
    }
/* They're both URLs, so EOS can't come before ':' */
    uri1[i] != '\0' && uri2[i] != '\0'?((void )0) : __assert_fail("(uri1[i] != '\\0') && (uri2[i] != '\\0')","dirent_uri.c",1388,__PRETTY_FUNCTION__);
    i++;
  }
/* Advance past '://' */
  i += 3;
  uri_ancestor_len = get_longest_ancestor_length(type_uri,uri1 + i,uri2 + i,pool);
  if (uri_ancestor_len == 0 || uri_ancestor_len == 1 && (uri1 + i)[0] == 47) {
    return (apr_pmemdup(pool,"",sizeof("")));
  }
  else {
    return apr_pstrndup(pool,uri1,uri_ancestor_len + i);
  }
}

const char *svn_dirent_is_child(const char *parent_dirent,const char *child_dirent,apr_pool_t *pool)
{
  return is_child(type_dirent,parent_dirent,child_dirent,pool);
}

const char *svn_dirent_skip_ancestor(const char *parent_dirent,const char *child_dirent)
{
  apr_size_t len = strlen(parent_dirent);
  apr_size_t root_len;
  if (0 != strncmp(parent_dirent,child_dirent,len)) {
/* parent_dirent is no ancestor of child_dirent */
    return ((void *)0);
  }
  if (child_dirent[len] == 0) {
/* parent_dirent == child_dirent */
    return "";
  }
/* Child == parent + more-characters */
  root_len = dirent_root_length(child_dirent,strlen(child_dirent));
  if (root_len > len) {
/* Different root, e.g. ("" "/...") or ("//z" "//z/share") */
    return ((void *)0);
  }
/* Now, child == [root-of-parent] + [rest-of-parent] + more-characters.
   * It must be one of the following forms.
   *
   * rlen parent    child       bad?  rlen=len? c[len]=/?
   *  0   ""        "foo"               *
   *  0   "b"       "bad"         !
   *  0   "b"       "b/foo"                       *
   *  1   "/"       "/foo"              *
   *  1   "/b"      "/bad"        !
   *  1   "/b"      "/b/foo"                      *
   *  2   "a:"      "a:foo"             *
   *  2   "a:b"     "a:bad"       !
   *  2   "a:b"     "a:b/foo"                     *
   *  3   "a:/"     "a:/foo"            *
   *  3   "a:/b"    "a:/bad"      !
   *  3   "a:/b"    "a:/b/foo"                    *
   *  5   "//s/s"   "//s/s/foo"         *         *
   *  5   "//s/s/b" "//s/s/bad"   !
   *  5   "//s/s/b" "//s/s/b/foo"                 *
   */
  if (child_dirent[len] == 47) {
/* "parent|child" is one of:
     * "[a:]b|/foo" "[a:]/b|/foo" "//s/s|/foo" "//s/s/b|/foo" */
    return child_dirent + len + 1;
  }
  if (root_len == len) {
/* "parent|child" is "|foo" "/|foo" "a:|foo" "a:/|foo" "//s/s|/foo" */
    return child_dirent + len;
  }
  return ((void *)0);
}

const char *svn_relpath_skip_ancestor(const char *parent_relpath,const char *child_relpath)
{
  apr_size_t len = strlen(parent_relpath);
  relpath_is_canonical(parent_relpath)?((void )0) : __assert_fail("relpath_is_canonical(parent_relpath)","dirent_uri.c",1472,__PRETTY_FUNCTION__);
  relpath_is_canonical(child_relpath)?((void )0) : __assert_fail("relpath_is_canonical(child_relpath)","dirent_uri.c",1473,__PRETTY_FUNCTION__);
  if (len == 0) {
    return child_relpath;
  }
  if (0 != strncmp(parent_relpath,child_relpath,len)) {
/* parent_relpath is no ancestor of child_relpath */
    return ((void *)0);
  }
  if (child_relpath[len] == 0) {
/* parent_relpath == child_relpath */
    return "";
  }
  if (child_relpath[len] == 47) {
    return child_relpath + len + 1;
  }
  return ((void *)0);
}
/* */

static const char *uri_skip_ancestor(const char *parent_uri,const char *child_uri)
{
  apr_size_t len = strlen(parent_uri);
  svn_uri_is_canonical(parent_uri,((void *)0))?((void )0) : __assert_fail("svn_uri_is_canonical(parent_uri, ((void *)0))","dirent_uri.c",1498,__PRETTY_FUNCTION__);
  svn_uri_is_canonical(child_uri,((void *)0))?((void )0) : __assert_fail("svn_uri_is_canonical(child_uri, ((void *)0))","dirent_uri.c",1499,__PRETTY_FUNCTION__);
  if (0 != strncmp(parent_uri,child_uri,len)) {
/* parent_uri is no ancestor of child_uri */
    return ((void *)0);
  }
  if (child_uri[len] == 0) {
/* parent_uri == child_uri */
    return "";
  }
  if (child_uri[len] == 47) {
    return child_uri + len + 1;
  }
  return ((void *)0);
}

const char *svn_uri_skip_ancestor(const char *parent_uri,const char *child_uri,apr_pool_t *result_pool)
{
  const char *result = uri_skip_ancestor(parent_uri,child_uri);
  return result?svn_path_uri_decode(result,result_pool) : ((void *)0);
}

svn_boolean_t svn_dirent_is_ancestor(const char *parent_dirent,const char *child_dirent)
{
  return svn_dirent_skip_ancestor(parent_dirent,child_dirent) != ((void *)0);
}

svn_boolean_t svn_uri__is_ancestor(const char *parent_uri,const char *child_uri)
{
  return uri_skip_ancestor(parent_uri,child_uri) != ((void *)0);
}

svn_boolean_t svn_dirent_is_absolute(const char *dirent)
{
  if (!dirent) {
    return 0;
  }
/* dirent is absolute if it starts with '/' on non-Windows platforms
     or with '//' on Windows platforms */
  if (dirent[0] == 47) {
#ifdef SVN_USE_DOS_PATHS
/* Single '/' depends on current drive */
#endif
    return !0;
  }
/* On Windows, dirent is also absolute when it starts with 'H:/'
     where 'H' is any letter. */
#ifdef SVN_USE_DOS_PATHS
#endif /* SVN_USE_DOS_PATHS */
  return 0;
}

svn_error_t *svn_dirent_get_absolute(const char **pabsolute,const char *relative,apr_pool_t *pool)
{
  char *buffer;
  apr_status_t apr_err;
  const char *path_apr;
  do {
    if (!(!svn_path_is_url(relative))) {
      do {
        svn_error_t *svn_err__temp = svn_error__malfunction(!0,"dirent_uri.c",1571,"! svn_path_is_url(relative)");
        if (svn_err__temp) {
          return svn_err__temp;
        }
      }while (0);
    }
  }while (0);
/* Merge the current working directory with the relative dirent. */
  do {
    svn_error_t *svn_err__temp = svn_path_cstring_from_utf8(&path_apr,relative,pool);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  apr_err = apr_filepath_merge(&buffer,((void *)0),path_apr,0x04,pool);
  if (apr_err) {
/* In some cases when the passed path or its ancestor(s) do not exist
         or no longer exist apr returns an error.
         In many of these cases we would like to return a path anyway, when the
         passed path was already a safe absolute path. So check for that now to
         avoid an error.
         svn_dirent_is_absolute() doesn't perform the necessary checks to see
         if the path doesn't need post processing to be in the canonical absolute
         format.
         */
    if (svn_dirent_is_absolute(relative) && svn_dirent_is_canonical(relative,pool) && !svn_path_is_backpath_present(relative)) {
       *pabsolute = (apr_pstrdup(pool,relative));
      return 0;
    }
    return svn_error_createf(SVN_ERR_BAD_FILENAME,svn_error_create(apr_err,((void *)0),((void *)0)),(dgettext("subversion","Couldn't determine absolute path of '%s'")),svn_dirent_local_style(relative,pool));
  }
  do {
    svn_error_t *svn_err__temp = svn_path_cstring_to_utf8(pabsolute,buffer,pool);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
   *pabsolute = svn_dirent_canonicalize( *pabsolute,pool);
  return 0;
}

const char *svn_uri_canonicalize(const char *uri,apr_pool_t *pool)
{
  return canonicalize(type_uri,uri,pool);
}

const char *svn_relpath_canonicalize(const char *relpath,apr_pool_t *pool)
{
  return canonicalize(type_relpath,relpath,pool);
}

const char *svn_dirent_canonicalize(const char *dirent,apr_pool_t *pool)
{
  const char *dst = canonicalize(type_dirent,dirent,pool);
#ifdef SVN_USE_DOS_PATHS
/* Handle a specific case on Windows where path == "X:/". Here we have to
     append the final '/', as svn_path_canonicalize will chop this of. */
#endif /* SVN_USE_DOS_PATHS */
  return dst;
}

svn_boolean_t svn_dirent_is_canonical(const char *dirent,apr_pool_t *scratch_pool)
{
  const char *ptr = dirent;
  if (( *ptr) == 47) {
    ptr++;
#ifdef SVN_USE_DOS_PATHS
/* Check for UNC paths */
/* TODO: Scan hostname and sharename and fall back to part code */
/* ### Fall back to old implementation */
#endif /* SVN_USE_DOS_PATHS */
  }
#ifdef SVN_USE_DOS_PATHS
/* The only canonical drive names are "A:"..."Z:", no lower case */
#endif /* SVN_USE_DOS_PATHS */
  return relpath_is_canonical(ptr);
}

static svn_boolean_t relpath_is_canonical(const char *relpath)
{
  const char *ptr = relpath;
  const char *seg = relpath;
/* RELPATH is canonical if it has:
   *  - no '.' segments
   *  - no start and closing '/'
   *  - no '//'
   */
  if (( *relpath) == '\0') {
    return !0;
  }
  if (( *ptr) == 47) {
    return 0;
  }
/* Now validate the rest of the path. */
  while(1){
    apr_size_t seglen = (ptr - seg);
    if (seglen == 1 && ( *seg) == 46) {
/*  /./   */
      return 0;
    }
    if (( *ptr) == 47 && ( *(ptr + 1)) == 47) {
/*  //    */
      return 0;
    }
    if (!( *ptr) && ( *(ptr - 1)) == 47) {
/* foo/  */
      return 0;
    }
    if (!( *ptr)) {
      break; 
    }
    if (( *ptr) == 47) {
      ptr++;
    }
    seg = ptr;
    while( *ptr && ( *ptr) != 47)
      ptr++;
  }
  return !0;
}

svn_boolean_t svn_relpath_is_canonical(const char *relpath)
{
  return relpath_is_canonical(relpath);
}

svn_boolean_t svn_uri_is_canonical(const char *uri,apr_pool_t *scratch_pool)
{
  const char *ptr = uri;
  const char *seg = uri;
  const char *schema_data = ((void *)0);
/* URI is canonical if it has:
   *  - lowercase URL scheme
   *  - lowercase URL hostname
   *  - no '.' segments
   *  - no closing '/'
   *  - no '//'
   *  - uppercase hex-encoded pair digits ("%AB", not "%ab")
   */
  if (( *uri) == '\0') {
    return 0;
  }
  if (!svn_path_is_url(uri)) {
    return 0;
  }
/* Skip the scheme. */
  while( *ptr && ( *ptr) != 47 && ( *ptr) != ':')
    ptr++;
/* No scheme?  No good. */
  if (!(( *ptr) == ':' && ( *(ptr + 1)) == 47 && ( *(ptr + 2)) == 47)) {
    return 0;
  }
/* Found a scheme, check that it's all lowercase. */
  ptr = uri;
  while(( *ptr) != ':'){
    if (( *ptr) >= 65 && ( *ptr) <= 'Z') {
      return 0;
    }
    ptr++;
  }
/* Skip :// */
  ptr += 3;
/* Scheme only?  That works. */
  if (!( *ptr)) {
    return !0;
  }
/* This might be the hostname */
  seg = ptr;
  while( *ptr && ( *ptr) != 47 && ( *ptr) != 64)
    ptr++;
  if (( *ptr) == 64) {
    seg = ptr + 1;
  }
/* Found a hostname, check that it's all lowercase. */
  ptr = seg;
  if (( *ptr) == '[') {
    ptr++;
    while(( *ptr) == ':' || ( *ptr) >= 48 && ( *ptr) <= '9' || ( *ptr) >= 'a' && ( *ptr) <= 'f'){
      ptr++;
    }
    if (( *ptr) != ']') {
      return 0;
    }
    ptr++;
  }
  else {
    while( *ptr && ( *ptr) != 47 && ( *ptr) != ':'){
      if (( *ptr) >= 65 && ( *ptr) <= 'Z') {
        return 0;
      }
      ptr++;
    }
  }
/* Found a portnumber */
  if (( *ptr) == ':') {
    apr_int64_t port = 0;
    ptr++;
    schema_data = ptr;
    while(( *ptr) >= 48 && ( *ptr) <= '9'){
      port = 10 * port + (( *ptr) - 48);
      ptr++;
    }
    if (ptr == schema_data) {
/* Fail on "http://host:" */
      return 0;
    }
    if ( *ptr && ( *ptr) != 47) {
/* Not a port number */
      return 0;
    }
    if (port == 80 && strncmp(uri,"http:",5) == 0) {
      return 0;
    }
    else {
      if (port == 443 && strncmp(uri,"https:",6) == 0) {
        return 0;
      }
      else {
        if (port == 3690 && strncmp(uri,"svn:",4) == 0) {
          return 0;
        }
      }
    }
  }
  schema_data = ptr;
#ifdef SVN_USE_DOS_PATHS
/* If this is a file url, ptr now points to the third '/' in
         file:///C:/path. Check that if we have such a URL the drive
         letter is in uppercase. */
#endif /* SVN_USE_DOS_PATHS */
/* Now validate the rest of the URI. */
  while(1){
    apr_size_t seglen = (ptr - seg);
    if (seglen == 1 && ( *seg) == 46) {
/*  /./   */
      return 0;
    }
    if (( *ptr) == 47 && ( *(ptr + 1)) == 47) {
/*  //    */
      return 0;
    }
    if (!( *ptr) && ( *(ptr - 1)) == 47 && ptr - 1 != uri) {
/* foo/  */
      return 0;
    }
    if (!( *ptr)) {
      break; 
    }
    if (( *ptr) == 47) {
      ptr++;
    }
    seg = ptr;
    while( *ptr && ( *ptr) != 47)
      ptr++;
  }
  ptr = schema_data;
  while( *ptr){
    if (( *ptr) == 37) {
      char digitz[3];
      int val;
/* Can't usesvn_ctype_isxdigit() because lower case letters are
             not in our canonical format */
      if ((( *(ptr + 1)) < 48 || ( *(ptr + 1)) > '9') && (( *(ptr + 1)) < 65 || ( *(ptr + 1)) > 'F')) {
        return 0;
      }
      else {
        if ((( *(ptr + 2)) < 48 || ( *(ptr + 2)) > '9') && (( *(ptr + 2)) < 65 || ( *(ptr + 2)) > 'F')) {
          return 0;
        }
      }
      digitz[0] =  *(++ptr);
      digitz[1] =  *(++ptr);
      digitz[2] = '\0';
      val = ((int )(strtol(digitz,((void *)0),16)));
      if (svn_uri__char_validity[val]) {
/* Should not have been escaped */
        return 0;
      }
    }
    else {
      if (( *ptr) != 47 && !svn_uri__char_validity[(unsigned char )( *ptr)]) {
/* Character should have been escaped */
        return 0;
      }
    }
    ptr++;
  }
  return !0;
}

svn_error_t *svn_dirent_condense_targets(const char **pcommon,apr_array_header_t **pcondensed_targets,const apr_array_header_t *targets,svn_boolean_t remove_redundancies,apr_pool_t *result_pool,apr_pool_t *scratch_pool)
{
  int i;
  int num_condensed = targets -> nelts;
  svn_boolean_t *removed;
  apr_array_header_t *abs_targets;
/* Early exit when there's no data to work on. */
  if (targets -> nelts <= 0) {
     *pcommon = ((void *)0);
    if (pcondensed_targets) {
       *pcondensed_targets = ((void *)0);
    }
    return 0;
  }
/* Get the absolute path of the first target. */
  do {
    svn_error_t *svn_err__temp = svn_dirent_get_absolute(pcommon,((const char **)(targets -> elts))[0],scratch_pool);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
/* Early exit when there's only one dirent to work on. */
  if (targets -> nelts == 1) {
     *pcommon = (apr_pstrdup(result_pool, *pcommon));
    if (pcondensed_targets) {
       *pcondensed_targets = apr_array_make(result_pool,0,(sizeof(const char *)));
    }
    return 0;
  }
/* Copy the targets array, but with absolute dirents instead of
     relative.  Also, find the pcommon argument by finding what is
     common in all of the absolute dirents. NOTE: This is not as
     efficient as it could be.  The calculation of the basedir could
     be done in the loop below, which would save some calls to
     svn_dirent_get_longest_ancestor.  I decided to do it this way
     because I thought it would be simpler, since this way, we don't
     even do the loop if we don't need to condense the targets. */
  removed = (memset(apr_palloc(scratch_pool,(targets -> nelts) * sizeof(svn_boolean_t )),0,(targets -> nelts) * sizeof(svn_boolean_t )));
  abs_targets = apr_array_make(scratch_pool,targets -> nelts,(sizeof(const char *)));
   *((const char **)(apr_array_push(abs_targets))) =  *pcommon;
  for (i = 1; i < targets -> nelts; ++i) {
    const char *rel = ((const char **)(targets -> elts))[i];
    const char *absolute;
    do {
      svn_error_t *svn_err__temp = svn_dirent_get_absolute(&absolute,rel,scratch_pool);
      if (svn_err__temp) {
        return svn_err__temp;
      }
    }while (0);
     *((const char **)(apr_array_push(abs_targets))) = absolute;
     *pcommon = (svn_dirent_get_longest_ancestor( *pcommon,absolute,scratch_pool));
  }
   *pcommon = (apr_pstrdup(result_pool, *pcommon));
  if (pcondensed_targets != ((void *)0)) {
    size_t basedir_len;
    if (remove_redundancies) {
/* Find the common part of each pair of targets.  If
             common part is equal to one of the dirents, the other
             is a child of it, and can be removed.  If a target is
             equal to *pcommon, it can also be removed. */
/* First pass: when one non-removed target is a child of
             another non-removed target, remove the child. */
      for (i = 0; i < abs_targets -> nelts; ++i) {
        int j;
        if (removed[i]) {
          continue; 
        }
        for (j = i + 1; j < abs_targets -> nelts; ++j) {
          const char *abs_targets_i;
          const char *abs_targets_j;
          const char *ancestor;
          if (removed[j]) {
            continue; 
          }
          abs_targets_i = ((const char **)(abs_targets -> elts))[i];
          abs_targets_j = ((const char **)(abs_targets -> elts))[j];
          ancestor = (svn_dirent_get_longest_ancestor(abs_targets_i,abs_targets_j,scratch_pool));
          if (( *ancestor) == '\0') {
            continue; 
          }
          if (strcmp(ancestor,abs_targets_i) == 0) {
            removed[j] = !0;
            num_condensed--;
          }
          else {
            if (strcmp(ancestor,abs_targets_j) == 0) {
              removed[i] = !0;
              num_condensed--;
            }
          }
        }
      }
/* Second pass: when a target is the same as *pcommon,
             remove the target. */
      for (i = 0; i < abs_targets -> nelts; ++i) {
        const char *abs_targets_i = ((const char **)(abs_targets -> elts))[i];
        if (strcmp(abs_targets_i, *pcommon) == 0 && !removed[i]) {
          removed[i] = !0;
          num_condensed--;
        }
      }
    }
/* Now create the return array, and copy the non-removed items */
    basedir_len = strlen( *pcommon);
     *pcondensed_targets = apr_array_make(result_pool,num_condensed,(sizeof(const char *)));
    for (i = 0; i < abs_targets -> nelts; ++i) {
      const char *rel_item = ((const char **)(abs_targets -> elts))[i];
/* Skip this if it's been removed. */
      if (removed[i]) {
        continue; 
      }
/* If a common prefix was found, condensed_targets are given
             relative to that prefix.  */
      if (basedir_len > 0) {
/* Only advance our pointer past a dirent separator if
                 REL_ITEM isn't the same as *PCOMMON.
                 If *PCOMMON is a root dirent, basedir_len will already
                 include the closing '/', so never advance the pointer
                 here.
                 */
        rel_item += basedir_len;
        if (rel_item[0] && !svn_dirent_is_root( *pcommon,basedir_len)) {
          rel_item++;
        }
      }
       *((const char **)(apr_array_push( *pcondensed_targets))) = (apr_pstrdup(result_pool,rel_item));
    }
  }
  return 0;
}

svn_error_t *svn_uri_condense_targets(const char **pcommon,apr_array_header_t **pcondensed_targets,const apr_array_header_t *targets,svn_boolean_t remove_redundancies,apr_pool_t *result_pool,apr_pool_t *scratch_pool)
{
  int i;
  int num_condensed = targets -> nelts;
  apr_array_header_t *uri_targets;
  svn_boolean_t *removed;
/* Early exit when there's no data to work on. */
  if (targets -> nelts <= 0) {
     *pcommon = ((void *)0);
    if (pcondensed_targets) {
       *pcondensed_targets = ((void *)0);
    }
    return 0;
  }
   *pcommon = svn_uri_canonicalize(((const char **)(targets -> elts))[0],scratch_pool);
/* Early exit when there's only one uri to work on. */
  if (targets -> nelts == 1) {
     *pcommon = (apr_pstrdup(result_pool, *pcommon));
    if (pcondensed_targets) {
       *pcondensed_targets = apr_array_make(result_pool,0,(sizeof(const char *)));
    }
    return 0;
  }
/* Find the pcommon argument by finding what is common in all of the
     uris. NOTE: This is not as efficient as it could be.  The calculation
     of the basedir could be done in the loop below, which would
     save some calls to svn_uri_get_longest_ancestor.  I decided to do it
     this way because I thought it would be simpler, since this way, we don't
     even do the loop if we don't need to condense the targets. */
  removed = (memset(apr_palloc(scratch_pool,(targets -> nelts) * sizeof(svn_boolean_t )),0,(targets -> nelts) * sizeof(svn_boolean_t )));
  uri_targets = apr_array_make(scratch_pool,targets -> nelts,(sizeof(const char *)));
   *((const char **)(apr_array_push(uri_targets))) =  *pcommon;
  for (i = 1; i < targets -> nelts; ++i) {
    const char *uri = svn_uri_canonicalize(((const char **)(targets -> elts))[i],scratch_pool);
     *((const char **)(apr_array_push(uri_targets))) = uri;
/* If the commonmost ancestor so far is empty, there's no point
         in continuing to search for a common ancestor at all.  But
         we'll keep looping for the sake of canonicalizing the
         targets, I suppose.  */
    if (( *( *pcommon)) != '\0') {
       *pcommon = (svn_uri_get_longest_ancestor( *pcommon,uri,scratch_pool));
    }
  }
   *pcommon = (apr_pstrdup(result_pool, *pcommon));
  if (pcondensed_targets != ((void *)0)) {
    size_t basedir_len;
    if (remove_redundancies) {
/* Find the common part of each pair of targets.  If
             common part is equal to one of the dirents, the other
             is a child of it, and can be removed.  If a target is
             equal to *pcommon, it can also be removed. */
/* First pass: when one non-removed target is a child of
             another non-removed target, remove the child. */
      for (i = 0; i < uri_targets -> nelts; ++i) {
        int j;
        if (removed[i]) {
          continue; 
        }
        for (j = i + 1; j < uri_targets -> nelts; ++j) {
          const char *uri_i;
          const char *uri_j;
          const char *ancestor;
          if (removed[j]) {
            continue; 
          }
          uri_i = ((const char **)(uri_targets -> elts))[i];
          uri_j = ((const char **)(uri_targets -> elts))[j];
          ancestor = (svn_uri_get_longest_ancestor(uri_i,uri_j,scratch_pool));
          if (( *ancestor) == '\0') {
            continue; 
          }
          if (strcmp(ancestor,uri_i) == 0) {
            removed[j] = !0;
            num_condensed--;
          }
          else {
            if (strcmp(ancestor,uri_j) == 0) {
              removed[i] = !0;
              num_condensed--;
            }
          }
        }
      }
/* Second pass: when a target is the same as *pcommon,
             remove the target. */
      for (i = 0; i < uri_targets -> nelts; ++i) {
        const char *uri_targets_i = ((const char **)(uri_targets -> elts))[i];
        if (strcmp(uri_targets_i, *pcommon) == 0 && !removed[i]) {
          removed[i] = !0;
          num_condensed--;
        }
      }
    }
/* Now create the return array, and copy the non-removed items */
    basedir_len = strlen( *pcommon);
     *pcondensed_targets = apr_array_make(result_pool,num_condensed,(sizeof(const char *)));
    for (i = 0; i < uri_targets -> nelts; ++i) {
      const char *rel_item = ((const char **)(uri_targets -> elts))[i];
/* Skip this if it's been removed. */
      if (removed[i]) {
        continue; 
      }
/* If a common prefix was found, condensed_targets are given
             relative to that prefix.  */
      if (basedir_len > 0) {
/* Only advance our pointer past a dirent separator if
                 REL_ITEM isn't the same as *PCOMMON.
                 If *PCOMMON is a root dirent, basedir_len will already
                 include the closing '/', so never advance the pointer
                 here.
                 */
        rel_item += basedir_len;
        if (rel_item[0] == 47 || rel_item[0] && !svn_uri_is_root( *pcommon,basedir_len)) {
          rel_item++;
        }
      }
       *((const char **)(apr_array_push( *pcondensed_targets))) = svn_path_uri_decode(rel_item,result_pool);
    }
  }
  return 0;
}

svn_error_t *svn_dirent_is_under_root(svn_boolean_t *under_root,const char **result_path,const char *base_path,const char *path,apr_pool_t *result_pool)
{
  apr_status_t status;
  char *full_path;
   *under_root = 0;
  if (result_path) {
     *result_path = ((void *)0);
  }
  status = apr_filepath_merge(&full_path,base_path,path,0x01 | 0x02,result_pool);
  if (status == 0) {
    if (result_path) {
       *result_path = svn_dirent_canonicalize(full_path,result_pool);
    }
     *under_root = !0;
    return 0;
  }
  else {
    if (status == 20000 + 23) {
       *under_root = 0;
      return 0;
    }
  }
  return svn_error_wrap_apr(status,((void *)0));
}

svn_error_t *svn_uri_get_dirent_from_file_url(const char **dirent,const char *url,apr_pool_t *pool)
{
  const char *hostname;
  const char *path;
  do {
    if (!svn_uri_is_canonical(url,pool)) {
      do {
        svn_error_t *svn_err__temp = svn_error__malfunction(!0,"dirent_uri.c",2304,"svn_uri_is_canonical(url, pool)");
        if (svn_err__temp) {
          return svn_err__temp;
        }
      }while (0);
    }
  }while (0);
/* Verify that the URL is well-formed (loosely) */
/* First, check for the "file://" prefix. */
  if (strncmp(url,"file://",7) != 0) {
    return svn_error_createf(SVN_ERR_RA_ILLEGAL_URL,((void *)0),(dgettext("subversion","Local URL '%s' does not contain 'file://' prefix")),url);
  }
/* Find the HOSTNAME portion and the PATH portion of the URL.  The host
     name is between the "file://" prefix and the next occurence of '/'.  We
     are considering everything from that '/' until the end of the URL to be
     the absolute path portion of the URL.
     If we got just "file://", treat it the same as "file:///". */
  hostname = url + 7;
  path = (strchr(hostname,47));
  if (path) {
    hostname = (apr_pstrmemdup(pool,hostname,(path - hostname)));
  }
  else {
    path = "/";
  }
/* URI-decode HOSTNAME, and set it to NULL if it is "" or "localhost". */
  if (( *hostname) == '\0') {
    hostname = ((void *)0);
  }
  else {
    hostname = svn_path_uri_decode(hostname,pool);
    if (strcmp(hostname,"localhost") == 0) {
      hostname = ((void *)0);
    }
  }
/* Duplicate the URL, starting at the top of the path.
     At the same time, we URI-decode the path. */
#ifdef SVN_USE_DOS_PATHS
/* On Windows, we'll typically have to skip the leading / if the
     path starts with a drive letter.  Like most Web browsers, We
     support two variants of this scheme:
         file:///X:/path    and
         file:///X|/path
    Note that, at least on WinNT and above,  file:////./X:/path  will
    also work, so we must make sure the transformation doesn't break
    that, and  file:///path  (that looks within the current drive
    only) should also keep working.
    If we got a non-empty hostname other than localhost, we convert this
    into an UNC path.  In this case, we obviously don't strip the slash
    even if the path looks like it starts with a drive letter.
  */
/* Casting away const! */
/* This check assumes ':' and '|' are already decoded! */
/* Skip the leading slash. */
/* A valid dirent for the driveroot must be like "C:/" instead of
                   just "C:" or svn_dirent_join() will use the current directory
                   on the drive instead */
/* We still know that the path starts with a slash. */
#else /* !SVN_USE_DOS_PATHS */
/* Currently, the only hostnames we are allowing on non-Win32 platforms
     are the empty string and 'localhost'. */
  if (hostname) {
    return svn_error_createf(SVN_ERR_RA_ILLEGAL_URL,((void *)0),(dgettext("subversion","Local URL '%s' contains unsupported hostname")),url);
  }
   *dirent = svn_path_uri_decode(path,pool);
#endif /* SVN_USE_DOS_PATHS */
  return 0;
}

svn_error_t *svn_uri_get_file_url_from_dirent(const char **url,const char *dirent,apr_pool_t *pool)
{
  svn_dirent_is_canonical(dirent,pool)?((void )0) : __assert_fail("svn_dirent_is_canonical(dirent, pool)","dirent_uri.c",2417,__PRETTY_FUNCTION__);
  do {
    svn_error_t *svn_err__temp = svn_dirent_get_absolute(&dirent,dirent,pool);
    if (svn_err__temp) {
      return svn_err__temp;
    }
  }while (0);
  dirent = svn_path_uri_encode(dirent,pool);
#ifndef SVN_USE_DOS_PATHS
  if (dirent[0] == 47 && dirent[1] == '\0') {
/* "file://" is the canonical form of "file:///" */
    dirent = ((void *)0);
  }
   *url = (apr_pstrcat(pool,"file://",dirent,((char *)((void *)0))));
#else
/* Handle UNC paths //server/share -> file://server/share */
/* Expect UNC, not non-absolute */
/* strlen("file:///") */
/* "C:/" is a canonical dirent on Windows,
         but "file:///C:/" is not a canonical uri */
#endif
  return 0;
}
/* -------------- The fspath API (see private/svn_fspath.h) -------------- */

svn_boolean_t svn_fspath__is_canonical(const char *fspath)
{
  return fspath[0] == 47 && relpath_is_canonical(fspath + 1);
}

const char *svn_fspath__canonicalize(const char *fspath,apr_pool_t *pool)
{
  if (fspath[0] == 47 && fspath[1] == '\0') {
    return "/";
  }
  return (apr_pstrcat(pool,"/",svn_relpath_canonicalize(fspath,pool),((char *)((void *)0))));
}

svn_boolean_t svn_fspath__is_root(const char *fspath,apr_size_t len)
{
/* directory is root if it's equal to '/' */
  return len == 1 && fspath[0] == 47;
}

const char *svn_fspath__skip_ancestor(const char *parent_fspath,const char *child_fspath)
{
  svn_fspath__is_canonical(parent_fspath)?((void )0) : __assert_fail("svn_fspath__is_canonical(parent_fspath)","dirent_uri.c",2488,__PRETTY_FUNCTION__);
  svn_fspath__is_canonical(child_fspath)?((void )0) : __assert_fail("svn_fspath__is_canonical(child_fspath)","dirent_uri.c",2489,__PRETTY_FUNCTION__);
  return svn_relpath_skip_ancestor(parent_fspath + 1,child_fspath + 1);
}

const char *svn_fspath__dirname(const char *fspath,apr_pool_t *pool)
{
  svn_fspath__is_canonical(fspath)?((void )0) : __assert_fail("svn_fspath__is_canonical(fspath)","dirent_uri.c",2499,__PRETTY_FUNCTION__);
  if (fspath[0] == 47 && fspath[1] == '\0') {
    return (apr_pstrdup(pool,fspath));
  }
  else {
    return (apr_pstrcat(pool,"/",svn_relpath_dirname(fspath + 1,pool),((char *)((void *)0))));
  }
}

const char *svn_fspath__basename(const char *fspath,apr_pool_t *pool)
{
  const char *result;
  svn_fspath__is_canonical(fspath)?((void )0) : __assert_fail("svn_fspath__is_canonical(fspath)","dirent_uri.c",2514,__PRETTY_FUNCTION__);
  result = svn_relpath_basename(fspath + 1,pool);
  strchr(result,47) == ((void *)0)?((void )0) : __assert_fail("strchr(result, '/') == ((void *)0)","dirent_uri.c",2518,__PRETTY_FUNCTION__);
  return result;
}

void svn_fspath__split(const char **dirpath,const char **base_name,const char *fspath,apr_pool_t *result_pool)
{
  dirpath != base_name?((void )0) : __assert_fail("dirpath != base_name","dirent_uri.c",2528,__PRETTY_FUNCTION__);
  if (dirpath) {
     *dirpath = svn_fspath__dirname(fspath,result_pool);
  }
  if (base_name) {
     *base_name = svn_fspath__basename(fspath,result_pool);
  }
}

char *svn_fspath__join(const char *fspath,const char *relpath,apr_pool_t *result_pool)
{
  char *result;
  svn_fspath__is_canonical(fspath)?((void )0) : __assert_fail("svn_fspath__is_canonical(fspath)","dirent_uri.c",2543,__PRETTY_FUNCTION__);
  svn_relpath_is_canonical(relpath)?((void )0) : __assert_fail("svn_relpath_is_canonical(relpath)","dirent_uri.c",2544,__PRETTY_FUNCTION__);
  if (relpath[0] == '\0') {
    result = apr_pstrdup(result_pool,fspath);
  }
  else {
    if (fspath[1] == '\0') {
      result = apr_pstrcat(result_pool,"/",relpath,((char *)((void *)0)));
    }
    else {
      result = apr_pstrcat(result_pool,fspath,"/",relpath,((char *)((void *)0)));
    }
  }
  svn_fspath__is_canonical(result)?((void )0) : __assert_fail("svn_fspath__is_canonical(result)","dirent_uri.c",2553,__PRETTY_FUNCTION__);
  return result;
}

char *svn_fspath__get_longest_ancestor(const char *fspath1,const char *fspath2,apr_pool_t *result_pool)
{
  char *result;
  svn_fspath__is_canonical(fspath1)?((void )0) : __assert_fail("svn_fspath__is_canonical(fspath1)","dirent_uri.c",2563,__PRETTY_FUNCTION__);
  svn_fspath__is_canonical(fspath2)?((void )0) : __assert_fail("svn_fspath__is_canonical(fspath2)","dirent_uri.c",2564,__PRETTY_FUNCTION__);
  result = apr_pstrcat(result_pool,"/",svn_relpath_get_longest_ancestor(fspath1 + 1,fspath2 + 1,result_pool),((char *)((void *)0)));
  svn_fspath__is_canonical(result)?((void )0) : __assert_fail("svn_fspath__is_canonical(result)","dirent_uri.c",2572,__PRETTY_FUNCTION__);
  return result;
}
/* -------------- The urlpath API (see private/svn_fspath.h) ------------- */

const char *svn_urlpath__canonicalize(const char *uri,apr_pool_t *pool)
{
  if (svn_path_is_url(uri)) {
    uri = svn_uri_canonicalize(uri,pool);
  }
  else {
    uri = svn_fspath__canonicalize(uri,pool);
/* Do a little dance to normalize hex encoding. */
    uri = svn_path_uri_decode(uri,pool);
    uri = svn_path_uri_encode(uri,pool);
  }
  return uri;
}
