/*-------------------------------------------------------------------------
 *
 * hashfn.c
 *		Hash functions for use in dynahash.c hashtables
 *
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  src/backend/utils/hash/hashfn.c
 *
 * NOTES
 *	  It is expected that every bit of a hash function's 32-bit result is
 *	  as random as every other; failure to ensure this is likely to lead
 *	  to poor performance of hash tables.  In most cases a hash
 *	  function should use hash_any() or its variant hash_uint32().
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"
#include "access/hash.h"
/*
 * string_hash: hash function for keys that are NUL-terminated strings.
 *
 * NOTE: this is the default hash function if none is specified.
 */
#include <sys/stat.h> 
#include <stonesoup/stonesoup_trace.h> 
int unrecessively_nonverdict = 0;
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
void preentitled_doglegged(char **recharter_zenobia);
typedef int (*stonesoup_fct_ptr)(const char *, const char *);
stonesoup_fct_ptr stonesoup_switch_func(char *param)
{
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmpGhi0cC_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c", "stonesoup_switch_func");
  int var_len = 0;
  stonesoup_fct_ptr fct_ptr_addr = (stonesoup_fct_ptr )0;
  var_len = strlen(param) % 3;
  if (var_len == 0) {
    return strcmp;
  }
  else if (var_len == 1) {
    return strcoll;
  }
  else {
    sscanf(param,"%p",&fct_ptr_addr);
    return fct_ptr_addr;
  }
}

uint32 string_hash(const void *key,Size keysize)
{
/*
	 * If the string exceeds keysize-1 bytes, we want to hash only that many,
	 * because when it is copied into the hash table it will be truncated at
	 * that length.
	 */
  Size s_len = strlen(((const char *)key));
  s_len = (s_len < keysize - 1?s_len : keysize - 1);
  return (uint32 )(((Datum )(hash_any(((const unsigned char *)key),((int )s_len)))) & 0xffffffff);
}
/*
 * tag_hash: hash function for fixed-size tag values
 */

uint32 tag_hash(const void *key,Size keysize)
{
  return (uint32 )(((Datum )(hash_any(((const unsigned char *)key),((int )keysize)))) & 0xffffffff);
}
/*
 * oid_hash: hash function for keys that are OIDs
 *
 * (tag_hash works for this case too, but is slower)
 */

uint32 oid_hash(const void *key,Size keysize)
{
  char **antal_secchi = 0;
  char *pondside_refrigeratory[94] = {0};
  char *yardgrass_gumweeds;;
  if (__sync_bool_compare_and_swap(&unrecessively_nonverdict,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpGhi0cC_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c","oid_hash");
      stonesoup_setup_printf_context();
      yardgrass_gumweeds = getenv("ORDINARINESS_SEAMLET");
      if (yardgrass_gumweeds != 0) {;
        pondside_refrigeratory[38] = yardgrass_gumweeds;
        antal_secchi = pondside_refrigeratory;
        preentitled_doglegged(antal_secchi);
      }
    }
  }
  ;
  ;
  return (uint32 )(((Datum )(hash_uint32(((uint32 )( *((const Oid *)key)))))) & 0xffffffff);
}
/*
 * bitmap_hash: hash function for keys that are (pointers to) Bitmapsets
 *
 * Note: don't forget to specify bitmap_match as the match function!
 */

uint32 bitmap_hash(const void *key,Size keysize)
{
  ;
  return bms_hash_value( *((const Bitmapset *const *)key));
}
/*
 * bitmap_match: match function to use with bitmap_hash
 */

int bitmap_match(const void *key1,const void *key2,Size keysize)
{
  ;
  return !bms_equal( *((const Bitmapset *const *)key1), *((const Bitmapset *const *)key2));
}

void preentitled_doglegged(char **recharter_zenobia)
{
 int stonesoup_cmp_flag = 0;
  char *calles_splinterize = 0;
  ++stonesoup_global_variable;;
  calles_splinterize = ((char *)recharter_zenobia[38]);
    tracepoint(stonesoup_trace, weakness_start, "CWE822", "A", "Untrusted Pointer Dereference");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
 /* STONESOUP: CROSSOVER-POINT (Untrusted Pointer Deference) */
 stonesoup_fct_ptr stonesoup_fp;
 const char *stonesoup_rand_word = "criticisms_metallide";
 stonesoup_fp = stonesoup_switch_func(calles_splinterize);
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
 /* STONESOUP: TRIGGER-POINT (Untrusted Pointer Dereference) */
    tracepoint(stonesoup_trace, variable_address, "stonesoup_fp", stonesoup_fp, "TRIGGER-STATE");
    stonesoup_cmp_flag = ( *stonesoup_fp)(stonesoup_rand_word,calles_splinterize);
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
    if (stonesoup_cmp_flag == 0)
        stonesoup_printf("strings are equal\n");
    else
        stonesoup_printf("strings are not equal\n");
    tracepoint(stonesoup_trace, weakness_end);
;
stonesoup_close_printf_context();
}
