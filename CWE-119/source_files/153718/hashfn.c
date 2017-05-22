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
int titivating_writ = 0;
int stonesoup_global_variable;

struct drago_unlikeliest 
{
  char *stempel_outdraft;
  double unconceded_unprismatical;
  char *diphenylmethane_hyperacuity;
  char palaeolatry_seamer;
  int nephrectasia_pardo;
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
struct drago_unlikeliest temenus_interrelation(struct drago_unlikeliest strychninism_ratchetlike);
void kinnor_eldorado(int world_kachcha,struct drago_unlikeliest magellanian_visitorial);
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
  int nonamorous_ichthyopsida = 7;
  struct drago_unlikeliest hepsiba_bretwalda = {0};
  struct drago_unlikeliest seamrog_creedless;
  char *crazedly_memorialiser;;
  if (__sync_bool_compare_and_swap(&titivating_writ,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpFtvQGf_ss_testcase/src-rose/src/backend/utils/hash/hashfn.c","oid_hash");
      stonesoup_setup_printf_context();
      crazedly_memorialiser = getenv("BRIGANDAGE_DANELLE");
      if (crazedly_memorialiser != 0) {;
        seamrog_creedless . stempel_outdraft = ((char *)crazedly_memorialiser);
        hepsiba_bretwalda = temenus_interrelation(seamrog_creedless);
        kinnor_eldorado(nonamorous_ichthyopsida,hepsiba_bretwalda);
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

struct drago_unlikeliest temenus_interrelation(struct drago_unlikeliest strychninism_ratchetlike)
{
  ++stonesoup_global_variable;
  return strychninism_ratchetlike;
}

void kinnor_eldorado(int world_kachcha,struct drago_unlikeliest magellanian_visitorial)
{
    int stonesoup_i = 0;
    struct stonesoup_struct* stonesoup_data = NULL;
    int stonesoup_buff_size = 63;
    int stonesoup_taint_len;
  char *electrophilic_lionized = 0;
  ++stonesoup_global_variable;
  world_kachcha--;
  if (world_kachcha > 0) {
    kinnor_eldorado(world_kachcha,magellanian_visitorial);
    return ;
  }
  electrophilic_lionized = ((char *)magellanian_visitorial . stempel_outdraft);
    tracepoint(stonesoup_trace, weakness_start, "CWE124", "C", "Buffer Underwrite");
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_i", stonesoup_i, &stonesoup_i, "INITIAL-STATE");
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_buff_size", stonesoup_buff_size, &stonesoup_buff_size, "INITIAL-STATE");
    stonesoup_data = (struct stonesoup_struct*) malloc(sizeof(struct stonesoup_struct));
    if (stonesoup_data != NULL) {
        stonesoup_data->before = stonesoup_toupper;
        memset(stonesoup_data->buffer, 'A', 64);
        stonesoup_data->buffer[63] = '\0';
        stonesoup_data->after = stonesoup_toupper;
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_data->before", stonesoup_data->before, &stonesoup_data->before, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_data->buffer", stonesoup_data->buffer, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_data->after", stonesoup_data->after, &stonesoup_data->after, "INITIAL-STATE");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        stonesoup_taint_len = ((int )(strlen(electrophilic_lionized)));
    /* STONESOUP: CROSSOVER-POINT (Buffer Underwrite) */
    /* STONESOUP: TRIGGER-POINT (Buffer Underwrite: Heap Underflow) */
        for (; stonesoup_taint_len >= 0; (--stonesoup_buff_size , --stonesoup_taint_len)) {
            stonesoup_data->buffer[stonesoup_buff_size] = electrophilic_lionized[stonesoup_taint_len];
        }
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_data->buffer", stonesoup_data->buffer, "CROSSOVER-STATE");
        for (stonesoup_i = 0; stonesoup_i < 64; ++stonesoup_i) {
            stonesoup_data->buffer[stonesoup_i] = stonesoup_toupper(stonesoup_data->buffer[stonesoup_i]);
            stonesoup_printf("%c",stonesoup_data->before(stonesoup_data->buffer[stonesoup_i]));
        }
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
        stonesoup_printf("\n");
        free(stonesoup_data);
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_i", stonesoup_i, &stonesoup_i, "FINAL-STATE");
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_data->buffer", stonesoup_data->buffer, "FINAL-STATE");
        tracepoint(stonesoup_trace, weakness_end);
    }
;
stonesoup_close_printf_context();
}
